#ifndef __MSG_H__
#define __MSG_H__

#define MAXPLAYER 4
#define MAXBOX 30
#define MAXLEVEL 10
#define BEGINBOX 20
#define PLAYERSIZE 16
#define WALLSIZE 16

struct c_s_msg{
	char type;//0 登陆 1 正常
	int x, y;
	unsigned char curetype;//药 0没打 客户端处理
	char BoxId;//捡的箱子，-1没捡
	unsigned char PickPill[4];
	unsigned short PickBl1;
	unsigned short PickBl2;
	bool PickArmor;
	bool PickWp1;
	bool PickWp2;
	bool Load, Exchange;//装弹，换枪 客户端处理顺序
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
	int Poison_X;
	int Poison_Y;
	int Poison_Size;
	double currenthp;
	double Armornaijiu;
	int PillAmount[4];
	int SubWeaponType;
	int MainWeaponCurBullet;
	int MainWeaponBackupBullet;
	int SubWeaponCurBullet;
	int SubWeaponBackupBullet;
	int x[MAXPLAYER];
	int y[MAXPLAYER];
	bool IsCuring[MAXPLAYER];
	bool IsLoading[MAXPLAYER];
	bool Firing[MAXPLAYER];	// 表示每个人是不是都在开火
	int MainWeaponType[MAXPLAYER];
	bool Isdead[MAXPLAYER];
	int BeKilledByPlayerId[MAXPLAYER];
	char user_name[MAXPLAYER][16];
	};

#endif