#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Weapon.h"
#include "node.h"

#define REDUCTION 0.10
#define DEFAULTSPEED 10
#define FULLBAG 100
#define PILLONEDELAY 8000
#define PILLTWODELAY 5000
#define PILLTHREEDELAY 3000
#define PILLFOURDELAY 2000

class Player :public node {
public:
	void InitalPlayer(int _player_id);

	void ChangePosition(unsigned x,unsigned y);

	void CureStart(unsigned type，unsigned sttime);

	void CureEnd(unsigned edtime);

	void Shoot(unsigned targetX,unsigned targetY);

	void BeAttack(float damage);

	void ExchangeWeapon();

	void LoadBegin(unsigned sttime);

	void Pick();

	void SetHpPercent(){Hppercentage = Player_Current_Hp / Player_Total_Hp;}

	int GetPlayerId()const{return player_id;}
	unsigned GetKillAmount()const{return KillAmount;}
	bool JudgeDead()const{return Isdead;}
	float GetPlayerCurrentHP()const{return Player_Current_Hp;}
	bool IsCuringNow()const{return IsCuring;}
	Weapon* GetMainWeapon()const{return MainWeapon;}
	Weapon* GetSubWeapon()const{return SubWeapon;}
	unsigned GetPillOneAmount()const{return PillsOne;}
	unsigned GetPillTwoAmount()const{return PillsTwo;}
	unsigned GetPillThreeAmount()const{return PillsThree;}
	unsigned GetPillFourAmount()const{return PillsFour}

private:
	int player_id;			  // 玩家编号
	unsigned KillAmount;	  // 击杀总数
	bool Isdead;	          // 判断是否死亡
	unsigned curestarttime;	  // 治疗起始时间
	unsigned curetype;		  // 正在使用的药的种类
	bool BagIsFull;			  // 标志背包是否已满

	float Player_Current_Hp;  // 当前生命值
	float Player_Total_Hp;	  // 总生命值
	float Hppercentage;		  // 生命值总数


	unsigned bagamount;		  // 背包总量
	float Player_Speed;		  // 玩家移动速度
	bool HaveArmor;			  // 标志是否有防弹衣
	bool IsCuring;		      // 正在打药
	//bool IsLoading;			  // 正在换弹

	float Armornaijiu;     // 防弹衣耐久
//	bool  IsAttack;			  // ¹¥»÷±êÖ¾Î»
//	bool  IsHurt;			  // ±»¹¥»÷±êÖ¾Î»
	unsigned PlayerDirection;  // 玩家方向

	Weapon* MainWeapon;		  // 主武器
	Weapon* SubWeapon;		  // 副武器

	unsigned PillsOne;		  // 第一种药品的数量
	unsigned PillsTwo;		  // 第二种药品的数量
	unsigned PillsThree;	  // 第三种药品的数量
	unsigned PillsFour;		  // 第四种药品的数量
};

#endif
