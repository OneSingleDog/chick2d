#ifndef __MSG_H__
#define __MSG_H__

#define MAXPLAYER 4
#define MAXBOX 30
#define MAXLEVEL 10
#define BEGINBOX 20

struct c_s_msg{
	char type;
	int x, y;
	char curetype;//ҩ 0û��
	char BoxId;//������ӣ�-1û��
	char PickPill[4];
	bool PickArmor;
	bool PickWp1;
	bool PickWp2;
	bool Load, Exchange;//װ������ǹ
	double ShootAngle;//����ǣ�����û�з���
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
	};

#endif