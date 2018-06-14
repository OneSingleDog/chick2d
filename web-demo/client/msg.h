#include<string>
#include<cstring>

struct c_s_msg{
	char s[1024];
	c_s_msg(){ memset(s, 0, sizeof(s)); }
	};

struct s_c_msg{
	char s[1024];
	s_c_msg(){ memset(s, 0, sizeof(s)); }
	};

c_s_msg login(const std::string&);
bool paint(const s_c_msg&);
c_s_msg update(const std::string&);