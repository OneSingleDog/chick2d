#include "boost.h"
#include "msg.h"
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

const size_t s_c_size = sizeof(s_c_msg);
const size_t c_s_size = sizeof(c_s_msg);

io_service service;

pthread_mutex_t mutex_boost;
pthread_mutex_t mutex_cocos;

bool login_succeeded;

s_c_msg s2c;
c_s_msg c2s;

class talk_to_svr: public boost::enable_shared_from_this<talk_to_svr>, boost::noncopyable {
	typedef talk_to_svr self_type;

private:
	ip::tcp::socket sock_;
	char read_buffer_[s_c_size];
	char write_buffer_[c_s_size];
	bool started_;
	std::string username_;
	clock_t last_sent_;

	talk_to_svr(const std::string & username): sock_(service), started_(true), username_(username), last_sent_(0) {}

	void start(ip::tcp::endpoint ep) 
		{
		sock_.async_connect(ep, MEM_FN1(on_connect, _1));
		}

	size_t read_complete(const boost::system::error_code &err, size_t bytes) {
		if (err) return 0;
		if (bytes>=s_c_size)return 0;
		return s_c_size-bytes;
		}

public:
	typedef boost::system::error_code error_code;
	typedef boost::shared_ptr<talk_to_svr> ptr;

	static ptr start(ip::tcp::endpoint ep, const std::string & username) {
		ptr new_(new talk_to_svr(username));
		new_->start(ep);
		return new_;
		}

	void stop() {
		if (!started_) return;
		started_ = false;
		sock_.close();
		}

	bool started() { return started_; }

private:
	void on_connect(const error_code & err) {
		if (!err)
			{
			#ifdef DEBUGVS
			OutputDebugPrintf("Connected ok\n");//连接成功
			#endif
			login_succeeded = true;
			pthread_mutex_unlock(&mutex_cocos);
			pthread_mutex_lock(&mutex_boost);
			do_write(c2s);//登陆信息
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

	void on_read(const error_code & err, size_t bytes) {
		if (err) stop();
		if (!started()) return;

		memcpy_s(&s2c, s_c_size, read_buffer_, s_c_size);
		pthread_mutex_unlock(&mutex_cocos);

		pthread_mutex_lock(&mutex_boost);
		//clock_t now = clock();
		//if (last_sent_)
		//	{
		//	int delay=now-last_sent_;
		//	if (delay<MinPing)Sleep(MinPing-delay);
		//	now = clock();
		//	delay = now-last_sent_;
		//	OutputDebugPrintf("delay:%dms\n", delay);
		//	}
		//last_sent_ = now;
		do_write(c2s);
		}

	void on_write(const error_code & err, size_t bytes){
		if (err) stop();
		if (!started()) return;
		do_read(); 
		}

private:
	void do_read() {
		async_read(sock_, buffer(read_buffer_), MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
		}

	void do_write(const c_s_msg& m) {
		if (!started()) return;
		memcpy_s(write_buffer_, c_s_size, &m, c_s_size);
		sock_.async_write_some(buffer(write_buffer_, c_s_size), MEM_FN2(on_write, _1, _2));
		}

};

std::string login_username;
std::string login_host;
std::string login_port;

void*boost_main(void*)
{
	talk_to_svr::ptr ptr;
	pthread_mutex_lock(&mutex_boost);
	#ifdef DEBUGVS
	OutputDebugPrintf("%s %s %s\n", login_username.c_str(), login_host.c_str(), login_port.c_str());
	#endif
	ptr = talk_to_svr::start(ip::tcp::endpoint(ip::address::from_string(login_host.c_str()), stoi(login_port)), login_username);
	service.run();
	ptr.reset();
	//system("pause");
    return NULL;
}

