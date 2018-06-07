#ifndef __GAME_H__
#define __GAME_H__

#include "Wall.h"
#include "Box.h"
#include <ctime>

#define MAXPLAYER 4
#define BOXBEGIN 20

class Game{
private:
    Player* player[MAXPLAYER];
    Box* box[BOXBEGIN];
    Wall* wall;
    unsigned Gamebegintime;

public:
    Game();
    void InitGame(unsigned player_X[MAXPLAYER],unsigned player_Y[MAXPLAYER],unsigned player_ID[MAXPLAYER],
                    unsigned box_X[BOXBEGIN],unsigned box_Y,unsigned box_ID[BOXBEGIN],char _wall[MAP_WIDTH][MAP_LENGTH]);
};



#endif
