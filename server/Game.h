#ifndef __GAME_H__
#define __GAME_H__

#include "Wall.h"
#include "Box.h"
#include "msg.h"
#include <ctime>

#define MAXPLAYER 4
#define MAXBOX 30
#define MAXLEVEL 10
#define BEGINBOX 20

class Game{
private:
    Player* player[MAXPLAYER];
    Box* box[MAXBOX];
    Wall* wall;

	int box_X[BEGINBOX], box_Y[BEGINBOX];
	double poison_DMG[MAXLEVEL];
	int poison_TIME[MAXLEVEL];

	int poison_LEVEL;
	int poison_X, poison_Y;
	int BoxNumber;
    unsigned Gamebegintime;
	bool started;

public:
    Game();
    void InitGame();
	string login(const c_s_msg&msg, int player_id);
	s_c_msg info(int player_id);
	void merge(const c_s_msg&msg, int player_id);
	bool alive(int player_id);
	void EndGame();
	~Game();
};

#endif
