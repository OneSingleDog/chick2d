#ifndef __MSG_H__
#define __MSG_H__

#define MAXPLAYER 4
#define MAXBOX 30
#define MAXLEVEL 10
#define BEGINBOX 20

struct c_s_msg{
	char type;
	int x, y;
	char curetype;//药 0没打
	char BoxId;//捡的箱子，-1没捡
	char PickPill[4];
	bool PickArmor;
	bool PickWp1;
	bool PickWp2;
	bool Load, Exchange;//装弹，换枪
	double ShootAngle;//射击角，负数没有发射
	char remark[16];
	};


struct send_box{
	int PillAmount[4];
	double Armor;
	char Wp1Type, Wp2Type;
	int Wp1Bullets, Wp2Bullets;
};

struct s_c_msg{
	char type;//0 未开始 1 正常 2 死亡 3 胜利
	char infox, infoy;//准备数 /击杀数 被击杀者
	send_box Boxes[MAXBOX];
	};

#endif