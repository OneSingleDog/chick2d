#include "Notice.h"

#define GAME_PLAYER 4
void Notice::init(std::string* Username_now) {
	for (int i = 0;i < GAME_PLAYER;i++)
	{
		Username[i] = Username_now[i];
		Survive[i] = true;
	}
	killevent_num = 0;
	memset(killer, 0, sizeof(killer));
	memset(be_killed, 0, sizeof(be_killed));
}

void Notice::deal_kill_event(bool* new_status,int* killer)
{
	int cnt = 0;

	for (int i = 0;i < GAME_PLAYER;i++)
	{
		if (Survive[i] == 0 && new_status[i] == 1)
		{
			cnt++;
			be_killed[cnt] = i;
			killer[cnt] = killer[i];
		}
	}
	killevent_num = cnt;
	for (int i = 0;i < GAME_PLAYER;i++)
	{
		Survive[i] = new_status[i];
	}
}
int Notice::get_survive_num() {
	int cnt = 0;
	for (int i = 0;i < GAME_PLAYER;i++)
	{
		if (Survive[i] == true) cnt++;
	}
	return cnt;
}



int Notice::get_killeventnum() const {
	return killevent_num;
}

std::string Notice::get_killer(int k) {
	if (k > killevent_num)return "Error";
	if (killer[k] == -1) return "Safe Zone";
	return Username[killer[k]];
}

std::string Notice::get_be_killed(int k) {
	if (k > killevent_num)return "Error";
	return Username[be_killed[k]];
}