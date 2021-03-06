#include "boost.h"
#include <string>

#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

#ifdef DEBUGVS
#include "atlbase.h"
#include "atlstr.h"
void OutputDebugPrintf(const char* strOutputString, ...)
	{
	char strBuffer[4096] = {0};
	va_list vlArgs;
	va_start(vlArgs, strOutputString);
	_vsnprintf_s(strBuffer, sizeof(strBuffer)-1, strOutputString, vlArgs);
	//vsprintf(strBuffer,strOutputString,vlArgs);
	va_end(vlArgs);
	OutputDebugString(CA2W(strBuffer));
	}
#endif


io_service service;

pthread_mutex_t mutex_boost;
pthread_mutex_t mutex_cocos;

s_c_msg s2c;
c_s_msg c2s;

bool canceled;

talk_to_svr::talk_to_svr(const std::string & username): sock_(service), started_(true), username_(username), last_sent_(0) {}

void talk_to_svr::start(ptr& newptr, ip::tcp::endpoint ep, const std::string & username) {
	ptr new_(new talk_to_svr(username));
	new_->start(ep);
	newptr = new_;
	return;
	}

void talk_to_svr::on_connect(const error_code & err) {
	if (!err)
		{
		#ifdef DEBUGVS
		OutputDebugPrintf("Connected ok\n");//connect ok
		#endif
		pthread_mutex_unlock(&mutex_cocos);
		pthread_mutex_lock(&mutex_boost);
		do_write(c2s);//form login information
		}
	else
		{
		#ifdef DEBUGVS
		OutputDebugPrintf("Connect failed\n");
		#endif
		pthread_mutex_unlock(&mutex_cocos);
		stop();
		}
	}

void talk_to_svr::on_read(const error_code & err, size_t bytes) {
	if (err)
		{
		stop();
		#ifdef DEBUGVS
		OutputDebugPrintf("%s\n", err.message().c_str());
		#endif
		}
	if (!started()) return;

    #ifdef MAC
    memcpy(&s2c, read_buffer_, s_c_size);
    #else
	memcpy_s(&s2c, s_c_size, read_buffer_, s_c_size);
    #endif
    
	pthread_mutex_unlock(&mutex_cocos);

	pthread_mutex_lock(&mutex_boost);

	if (canceled)stop();
	
	do_write(c2s);
	}

void talk_to_svr::stop() {
	if (!started_) return;
	started_ = false;
	sock_.close();
    #ifdef DEBUGVS
	OutputDebugPrintf("Socket closed\n");
    #endif
	}

std::string login_username;
std::string login_host;
std::string login_port;
talk_to_svr::ptr ptr;

void*boost_main(void*)
{
	pthread_mutex_lock(&mutex_boost);
	#ifdef DEBUGVS
	OutputDebugPrintf("%s %s %s\n", login_username.c_str(), login_host.c_str(), login_port.c_str());
	#endif
	if (canceled)return NULL;
	talk_to_svr::start(ptr,ip::tcp::endpoint(ip::address::from_string(login_host.c_str()), stoi(login_port)), login_username);
	service.run();
	#ifdef MAC
	pthread_detach(pthread_self());
	#endif
	return NULL;
}
