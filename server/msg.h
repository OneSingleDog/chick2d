#ifndef __MSG_H__
#define __MSG_H__

#define MAXPLAYER 4
#define MAXBOX 30
#define MAXLEVEL 10
#define BEGINBOX 20
#define PLAYERSIZE 16
#define WALLSIZE 16

struct c_s_msg{
	char type;//0 ��½ 1 ����
	int x, y;
	unsigned char curetype;//ҩ 0û�� �ͻ��˴���
	char BoxId;//�������ӣ�-1û��
	unsigned char PickPill[4];
	unsigned short PickBl1;
	unsigned short PickBl2;
	bool PickArmor;
	bool PickWp1;
	bool PickWp2;
	bool Load, Exchange;//װ������ǹ �ͻ��˴���˳��
	double ShootAngle;//�����ǣ�����û�з���
	char remark[16];
	};


struct send_box{
	int PillAmount[4];
	double Armor;
	char Wp1Type, Wp2Type;
	int Wp1Bullets, Wp2Bullets;
};

struct s_c_msg{
	char type;//0 δ��ʼ 1 ���� 2 ���� 3 ʤ��
	char infox, infoy;//׼���� /��ɱ�� ����ɱ��
	send_box Boxes[MAXBOX];
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
	};

#endif
