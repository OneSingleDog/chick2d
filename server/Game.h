#ifndef __GAME_H__
#define __GAME_H__

#include "Wall.h"
#include "Box.h"
#include "msg.h"
#include <ctime>

//#define MAXPLAYER 4
//#define MAXBOX 30
//#define MAXLEVEL 10
//#define BEGINBOX 20
//#define PLAYERSIZE 16
//#define WALLSIZE 16
//defined in msg.h

class Game{
private:
    Player* player[MAXPLAYER];
    Box* box[MAXBOX];
    Wall* wall;

	int box_X[BEGINBOX], box_Y[BEGINBOX];
	double poison_DMG[MAXLEVEL];//ÿms�˺�
	int poison_TIME[MAXLEVEL];
	int poison_SIZE[MAXLEVEL];

    bool ShootSuccess[MAXPLAYER];

	int poison_LEVEL;
	int poison_X, poison_Y;
	int BoxNumber;
    clock_t Gamebegintime;
	bool started;
	int connected;
	int living_count;

	void change_poison();
	bool Die(int player_id);

	void Shoot(int player_id, double angle,unsigned nowtime);
public:
	s_c_msg output;
    Game();
    void InitGame();
	string login(const c_s_msg&msg, int player_id);
	s_c_msg&info(int player_id);
	void merge(const c_s_msg&msg, int player_id);
	bool alive(int player_id);
	void EndGame();
	~Game();
};

#endif
