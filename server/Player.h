#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Weapon.h"
#include "node.h"
#include <cmath>
#include <string>
using std::string;

#define REDUCTION 0.10
#define PILLONEDELAY 6000
#define PILLTWODELAY 5000
#define PILLTHREEDELAY 3000
#define PILLFOURDELAY 2000

class Player :public node {
public:
	Player(int _x,int _y):node(_x,_y){}

	void InitalPlayer(int _player_id,string _username);
	void ChangePosition(unsigned x,unsigned y);

	void CureStart(unsigned type,unsigned sttime);
	void CureEnd(unsigned edtime);

	bool Shoot(unsigned nowtime);//return true if open fire
	bool LoadBullet(unsigned nowtime);
	void ExchangeWeapon();

	void BeAttack(double damage,int from);
	void LossHp(double damage,int disconnect);

	void PickPillOne(unsigned num);
	void PickPillTwo(unsigned num);
	void PickPillThree(unsigned num);
	void PickPillFour(unsigned num);
	Weapon* PickWeapon(Weapon* _weapon);
	void PickArmor(double naijiu){Armornaijiu = naijiu;}
	bool PickBullet(int type, unsigned num);

	int GetPlayerId()const{return player_id;}
	unsigned GetKillAmount()const{return KillAmount;}
	int GetKillerId()const{return killer_id;}
	double GetShootAngle(double _angle){return MainWeapon->GetRandomFireAngle(_angle);}
	double GetPlayerCurrentHP()const{return Player_Current_Hp;}
	Weapon* GetMainWeapon()const{return MainWeapon;}
	Weapon* GetSubWeapon()const{return SubWeapon;}
	unsigned GetPillOneAmount()const{return PillsOne;}
	unsigned GetPillTwoAmount()const{return PillsTwo;}
	unsigned GetPillThreeAmount()const{return PillsThree;}
	unsigned GetPillFourAmount()const{ return PillsFour; }
	double GetArmorNaijiu()const{return Armornaijiu;}
	string GetUserName()const{return username;}
	bool GetInPoison()const{if(poison_time == -1)return false; else return true;}

	bool IsCuringNow()const{return IsCuring;}
	bool JudgeDead()const{return Isdead;}

	void KillOnePlayer(){KillAmount ++;}


	void InPoison(unsigned nowtime,double dmg);
	void OutPoison(){poison_time = -1;};

	void setFaceAngle(double angle){ FaceAngle = angle; }
	double getFaceAngle(){ return FaceAngle; }
private:
	string username;
	int player_id;
	int killer_id;

	unsigned KillAmount;
	bool Isdead;
	unsigned curestarttime;
	unsigned curetype;

	int poison_time;//last time in poison

	double Player_Current_Hp;

	bool IsCuring;

	double Armornaijiu;

	Weapon* MainWeapon;
	Weapon* SubWeapon;

	unsigned PillsOne;
	unsigned PillsTwo;
	unsigned PillsThree;
	unsigned PillsFour;

	double FaceAngle;
};

#endif
