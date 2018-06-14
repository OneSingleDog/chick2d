#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Weapon.h"
#include "node.h"
#include <cmath>
#include <string>
using std::string;

#define REDUCTION 0.10
#define PILLONEDELAY 8000
#define PILLTWODELAY 5000
#define PILLTHREEDELAY 3000
#define PILLFOURDELAY 2000

class Player :public node {
public:
	Player(int _x,int _y):node(_x,_y){}
	// 初始化player
	void InitalPlayer(int _player_id,string _username);
	// 改变玩家的坐标
	void ChangePosition(unsigned x,unsigned y);
	// 开始打药 参数：药品的种类、打药开始时间
	void CureStart(unsigned type,unsigned sttime);
	// 打药结束，每次发包都要检测
	void CureEnd(unsigned edtime);
	// 射击，返回是否射击成功
	bool Shoot(unsigned nowtime);
	// 被武器击中，伤害为damage
	void BeAttack(double damage,int from);
	// 换弹，返回是否可以换弹
	bool LoadBullet(unsigned nowtime);
    // 受到电网伤害
	void LossHp(double damage,int disconnect);
	// 换武器
	void ExchangeWeapon();

	//void LoadBegin(unsigned sttime);
    // 捡第一类药品
	void PickPillOne(unsigned num);
	// 捡第二类药品
	void PickPillTwo(unsigned num);
	// 捡第三类药品
	void PickPillThree(unsigned num);
    // 捡第四类药品
	void PickPillFour(unsigned num);
	// 捡武器
	Weapon* PickWeapon(Weapon* _weapon);
	// 捡防弹衣
	void PickArmor(double naijiu){Armornaijiu = naijiu;}
	// 捡子弹
	bool PickBullet(int type, unsigned num);
	// 设置生命值的百分比
	void SetHpPercent(){Hppercentage = Player_Current_Hp / Player_Total_Hp;}
	// 获得玩家的id
	int GetPlayerId()const{return player_id;}
	// 获得击杀总数
	unsigned GetKillAmount()const{return KillAmount;}
	int GetKillerId()const{return killer_id;}
	double GetShootAngle(double _angle){return MainWeapon->GetRandomFireAngle(_angle);}
	// 判断是否死亡
	bool JudgeDead()const{return Isdead;}
	// 获取当前生命值
	double GetPlayerCurrentHP()const{return Player_Current_Hp;}
	// 判断当前是否正在打药
	bool IsCuringNow()const{return IsCuring;}
	// 获取主武器的指针
	Weapon* GetMainWeapon()const{return MainWeapon;}
	// 获取副武器的指针
	Weapon* GetSubWeapon()const{return SubWeapon;}
	// 获取第一类药物的数量
	unsigned GetPillOneAmount()const{return PillsOne;}
	// 获取第二类药物的数量
	unsigned GetPillTwoAmount()const{return PillsTwo;}
	// 获取第三类药物的数量
	unsigned GetPillThreeAmount()const{return PillsThree;}
	// 获取第四类药物的数量
	unsigned GetPillFourAmount()const{ return PillsFour; }
	// 获取防弹衣耐久
	double GetArmorNaijiu()const{return Armornaijiu;}
	// 击杀玩家
	void KillOnePlayer(){KillAmount ++;}
	string GetUserName()const{return username;}
	// 吃毒
	void InPoison(unsigned nowtime,double dmg);
	void OutPoison(){poison_time = -1;};
	bool GetInPoison()const{if(poison_time == -1)return false; else return true;}

	void setFaceAngle(double angle){ FaceAngle = angle; }
	double getFaceAngle(){ return FaceAngle; }
private:
	string username;
	int player_id;			  // 玩家编号
	int killer_id;            // 被谁击杀 -1毒
	unsigned KillAmount;	  // 击杀总数
	bool Isdead;	          // 判断是否死亡
	unsigned curestarttime;	  // 治疗起始时间
	unsigned curetype;		  // 正在使用的药的种类

	int poison_time;     // 上次在毒圈的时间

	double Player_Current_Hp;  // 当前生命值
	double Player_Total_Hp;	  // 总生命值
	double Hppercentage;		  // 生命值总数

	bool IsCuring;		      // 正在打药
	//double ShootAngle;		  // 射击角度

	double Armornaijiu;     // 防弹衣耐久

	Weapon* MainWeapon;		  // 主武器
	Weapon* SubWeapon;		  // 副武器

	unsigned PillsOne;		  // 第一种药品的数量
	unsigned PillsTwo;		  // 第二种药品的数量
	unsigned PillsThree;	  // 第三种药品的数量
	unsigned PillsFour;		  // 第四种药品的数量

	double FaceAngle;
};

#endif
