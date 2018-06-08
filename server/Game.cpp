#include "Game.h"
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
	for (int i = 0;i<BEGINBOX;++i)
		{
		box[i] = new Box(box_X[i], box_Y[i], i);
		box[i]->InitBoxByRandom();
		}
    Gamebegintime = clock();
}

void Game::EndGame(){
	if (!started)return;
	for (int i = 0;i<MAXPLAYER;++i)if (player[i])delete(player[i]);
	for (int i = 0;i<BoxNumber;++i)if (box[i])delete(box[i]);
	BoxNumber = 0;
	poison_LEVEL = 0;
	started = false;
}

bool Game::alive(int player_id){
	return player[player_id]->JudgeDead();
}

string Game::login(const c_s_msg&msg, int player_id){
	player[player_id] = new Player(msg.x, msg.y);
	string user_name = string(msg.remark);
	player[player_id]->InitalPlayer(player_id, user_name);
	return user_name;
	if (player_id==MAXPLAYER-1)started = true;
}

s_c_msg Game::info(int player_id){
	return s_c_msg();
}

void Game::merge(const c_s_msg&msg, int player_id){
}