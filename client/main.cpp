#ifdef WINDOWS
#include "main.h"
#endif
#include "AppDelegate.h"
#include "cocos2d.h"

#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

#include "boost.h"

USING_NS_CC;

#ifndef MAC
void* cocos2d_main(void*)
{
	// create the application instance
	AppDelegate app;
	Application::getInstance()->run();
	return NULL;
}

void*boost_main(void*);

int WINAPI _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	pthread_t cocos2d,boost;
	extern pthread_mutex_t mutex_boost;
	extern pthread_mutex_t mutex_cocos;

	pthread_mutex_init(&mutex_boost, NULL);
	pthread_mutex_init(&mutex_cocos, NULL);

	pthread_mutex_lock(&mutex_boost);
	pthread_mutex_lock(&mutex_cocos);

	pthread_create(&cocos2d, NULL, cocos2d_main, NULL);
	pthread_create(&boost, NULL, boost_main, NULL);

	pthread_join(cocos2d, NULL);

	extern bool canceled;
	canceled= true;
	pthread_mutex_unlock(&mutex_boost);
	extern talk_to_svr::ptr ptr;
	if (ptr.use_count())
		{
		ptr->stop();
		ptr.reset();
		}

	pthread_join(boost, NULL);

	return 0;
}
#else
void*boost_main(void*);

int main(int argc, char *argv[])
	{


	pthread_t boost;
	extern pthread_mutex_t mutex_boost;
	extern pthread_mutex_t mutex_cocos;

	pthread_mutex_init(&mutex_boost, NULL);
	pthread_mutex_init(&mutex_cocos, NULL);

	pthread_mutex_lock(&mutex_boost);
	pthread_mutex_lock(&mutex_cocos);

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&boost, &attr, boost_main, NULL);

	AppDelegate app;
	return Application::getInstance()->run();

	extern bool canceled;
	canceled = true;
	pthread_mutex_unlock(&mutex_boost);
	extern talk_to_svr::ptr ptr;
	if (ptr.use_count())
		{
		ptr->stop();
		ptr.reset();
		}

	return 0;
	}
#endif
