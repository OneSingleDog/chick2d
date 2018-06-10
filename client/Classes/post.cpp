#include<pthread.h>
#include"msg.h"

extern pthread_mutex_t mutex_boost;
extern pthread_mutex_t mutex_cocos;

extern s_c_msg s2c;
extern c_s_msg c2s;
c_s_msg to_be_sent;

bool received()
{
	return pthread_mutex_trylock(&mutex_cocos)!=EBUSY;
}

void write()
{
	c2s = to_be_sent;
	pthread_mutex_unlock(&mutex_boost);
}