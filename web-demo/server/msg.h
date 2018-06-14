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

void initial_game();
bool alive(int);
s_c_msg info(int);
s_c_msg login_ok(int);
void merge(const c_s_msg&, int);
std::string login(const c_s_msg&, int);