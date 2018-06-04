#include"msg.h"
#include<cstdio>
#include<ctime>

bool started;

c_s_msg login(const std::string&username)
{
	c_s_msg ans;
	sprintf_s(ans.s,1024, "%s", ("login+"+username).c_str());
	return ans;
}

bool paint(const s_c_msg&m)
{
	printf("%s\n",m.s);
	if (!started&&m.s[0]!='n'&&m.s[1]!=':')
		{
		printf("Start!\n");
		started = true;
		}
	return !(m.s[0]=='e'&&m.s[1]=='n'&&m.s[2]=='d');
}

c_s_msg update(const std::string&username)
{
	c_s_msg ans;
	time_t t = time(0);
	struct tm local;
	localtime_s(&local, &t);
	sprintf_s(ans.s,1024, "%s+%d:%d:%d", ("data+"+username).c_str(),local.tm_hour,local.tm_min,local.tm_sec);
	return ans;
}