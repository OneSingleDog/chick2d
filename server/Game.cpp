#include "Game.h"

Game::Game(){
    for(int i = 0;i < MAXPLAYER;++ i){
        player[i] = NULL;
    }

    for(int i = 0;i < BOXBEGIN;++ i){
        box[i] = NULL;
    }
    wall = NULL;

    Gamebegintime = 0;
}

void Game::InitGame(unsigned player_X[MAXPLAYER],unsigned player_Y[MAXPLAYER],unsigned player_ID[MAXPLAYER],unsigned box_X[BOXBEGIN],unsigned box_Y,unsigned box_ID[BOXBEGIN],char _wall[MAP_WIDTH][MAP_LENGTH]){
    for(int i = 0;i < MAXPLAYER;++ i){
        player[i] = new Player(player_X[i],palyer_Y[i]);
        player[i] -> InitalPlayer(player_ID[i]);
        box[i] = new Box(box_X[i],box_Y[i],box_ID[i]);
        box[i] -> InitBoxByRandom();
        wall = new Wall(_wall);
    }
    Gamebegintime = clock();
}
