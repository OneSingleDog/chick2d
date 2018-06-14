#ifndef __MSG_H__
#define __MSG_H__

#define MAXPLAYER 5
#define MAXBOX 30
#define MAXLEVEL 5
#define PLAYERSIZE 16
#define WALLSIZE 16
#define BOXSIZE 16

#define MAP_LBOUND 22
#define MAP_RBOUND 109
#define MAP_DBOUND 18
#define MAP_UBOUND 113

#include <cstring>

struct c_s_msg{
	char type;//0 login 1 normal
	int x, y;
	char curetype;//medicine 0 not curing -1 stop
	double face_angle;
	char BoxId;//-1 not pick
	unsigned char PickPill[4];
	bool PickBl1;
	bool PickBl2;
	bool PickArmor;
	bool PickWp1;
	bool PickWp2;
	bool Load, Exchange;
	double ShootAngle;//if negative not shoot
	char remark[16];//username
	c_s_msg(){
		type = 0;
		x = y = 0;
		curetype = 0;
		BoxId = -1;
		PickPill[0] = PickPill[1] = PickPill[2] = PickPill[3] = 0;
		PickBl1 = PickBl2 = 0;
		PickArmor = PickWp1 = PickWp2 = false;
		Load = Exchange = false;
		ShootAngle = -1;
		memset(remark, 0, sizeof(remark));
		}
	};


struct send_box{
	char type;
	int x, y;
	int PillAmount[4];
	double Armor;
	char Wp1Type, Wp2Type;
	int Wp1Bullets, Wp2Bullets;
	};

struct s_c_msg{
	char type;//0 not begin 1 normal 2 dead 3 winner
	char infox, infoy;//ready playerID for 0 /killerID rank for 2 3
	int Boxnum;
	send_box Boxes[MAXBOX];
	int Poison_X;
	int Poison_Y;
	int Poison_LEVEL;
	bool inpoison;
	double currenthp;
	double Armornaijiu;
	int PillAmount[4];
	int SubWeaponType;
	int MainWeaponCurBullet;
	int MainWeaponBackupBullet;
	int SubWeaponCurBullet;
	int SubWeaponBackupBullet;
	bool IsCuring;
	int live_count;//if type 0 max player
	int x[MAXPLAYER];
	int y[MAXPLAYER];
	bool Firing[MAXPLAYER];
	int MainWeaponType[MAXPLAYER];
	bool Isdead[MAXPLAYER];
	int BeKilledByPlayerId[MAXPLAYER];
	double face_angle[MAXPLAYER];
	char user_name[MAXPLAYER][16];
	};

#endif
