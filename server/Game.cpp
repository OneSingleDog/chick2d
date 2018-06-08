#include "Game.h"
#include <cstdlib>
#include <cstdio>

Game::Game(){
	FILE*config;
	fopen_s(&config, "config.txt", "r");
	int tmp;
	wall = new Wall;
	for (int i = 0;i<MAP_WIDTH;++i)
		for (int j = 0;j<MAP_WIDTH;++j)
			{
			fscanf_s(config, "%d", &tmp);
			wall->Set(i, j, tmp);
			}
	for (int i = 0;i<BEGINBOX;++i)
		fscanf_s(config, "%d%d", box_X+i, box_Y+i);
	for (int i = 0;i<MAXLEVEL;++i)
		fscanf_s(config, "%lf%d", poison_DMG+i, poison_TIME+i);
	fclose(config);
}

Game::~Game()
	{
	EndGame();
	delete wall;
	}

void Game::InitGame(){
	BoxNumber = BEGINBOX;
	poison_X = 0;
	poison_Y = 0;
	poison_LEVEL = 0;
	started = false;
	connected = 0;
	for (int i = 0;i<BEGINBOX;++i)
		{
		box[i] = new Box(box_X[i], box_Y[i], i);
		box[i]->InitBoxByRandom();
		}
    Gamebegintime = clock();
	srand(time(0));
}

void Game::EndGame(){
	if (!started)return;
	for (int i = 0;i<MAXPLAYER;++i)if (player[i])delete(player[i]);
	for (int i = 0;i<BoxNumber;++i)if (box[i])delete(box[i]);
	BoxNumber = 0;
	poison_LEVEL = 0;
	started = false;
	connected = 0;
}

bool Game::alive(int player_id){
	if (living_count<=1)return false;//ÓÎÏ·½áÊø
	return player[player_id]->JudgeDead();
}

string Game::login(const c_s_msg&msg, int player_id){
	player[player_id] = new Player(msg.x, msg.y);
	string user_name = string(msg.remark);
	player[player_id]->InitalPlayer(player_id, user_name);
	++connected;
	if (connected==MAXPLAYER)
		{
		started = true;
		living_count = MAXPLAYER;
		}
	return user_name;
}

s_c_msg&Game::info(int player_id){
	if (!started)
		{
		output.type = 0;
		output.infox = connected;
		}
	else if (player[player_id]->JudgeDead())
		{
		output.type = 2;
		output.infox = player[player_id]->GetKillAmount();
		output.infoy = player[player_id]->GetKillerId();
		}
	else if (living_count==1)
		{
		output.type = 3;
		output.infox = player[player_id]->GetKillAmount();
		}
	else
		{
		output.type = 1;
		}
	return output;
}

void Game::merge(const c_s_msg&msg, int player_id){
}