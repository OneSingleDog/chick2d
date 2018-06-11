#ifndef Notice_h
#define Notice_h

#include <string>
#include <cstring>

class Notice
{
public:
	void init(std::string* Username);
	int get_survive_num();
	void deal_kill_event(bool* new_status,int* killer);
	int get_killeventnum() const;
	std::string get_killer(int k);//return kth killevent's killer
	std::string get_be_killed(int k);//return kth killevent's be_killed

private:
	bool Survive[4];
	std::string Username[4];
	int killevent_num;
	int killer[5];//kill_event in the period of killer
	int be_killed[5];//kill_event int the period of be_killed

};


#endif // !Notice_h

#pragma once
