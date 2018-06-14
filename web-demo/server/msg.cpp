#include"msg.h"
#include<cstdio>

int msgcnt[32];
int connected;
#define max_connect 2

void initial_game()
{
	memset(msgcnt, 0, sizeof(msgcnt));
	connected = 0;
	return;
}


std::string login(const c_s_msg&m,int id)
{
	printf("%s logined as %d\n", m.s+6, id);
	++connected;
	return std::string(m.s+6);
}

s_c_msg login_ok(int id)
{
	s_c_msg ans;
	sprintf_s(ans.s, 1024, "%d:login succeeded,%d has connected", id,connected);
	return ans;
}

void merge(const c_s_msg&m,int id)
{
	if(connected>=max_connect)++msgcnt[id];
	printf("%d:%s\n", id, m.s);
	return;
}

bool alive(int id)
{
	return msgcnt[id]<10;
}

s_c_msg info(int id)
{
	s_c_msg ans;
	if (connected<max_connect)sprintf_s(ans.s, 1024, "not started, %d has connected",connected);
	else if(alive(id))sprintf_s(ans.s, 1024, "%d has sent %d messages", id, msgcnt[id]);
	else sprintf_s(ans.s, 1024, "end!%d has sent %d messages", id, msgcnt[id]);
	return ans;
}