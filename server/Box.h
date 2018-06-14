#ifndef __BOX_H__
#define __BOX_H__

#include "Player.h"
#include "node.h"

class Box:public node{
private:
	unsigned Boxid;
	unsigned weapon_amount;
	unsigned pill_amount;

	Weapon* weaponone;
 	Weapon* weapontwo;

	unsigned Pill_One_Amount;
	unsigned Pill_Two_Amount;
	unsigned Pill_Three_Amount;
	unsigned Pill_Four_Amount;

	double Armornaijiu;				//if 0 no armor
public:
	Box(int _x,int _y,int _id):node(_x,_y),Boxid(_id){}

	void InitBoxByRandom();
	void InitBoxByPlayer(Player*_player);

	unsigned GetId()const{return Boxid;}
	double GetArmorNaijiu()const{return Armornaijiu;}
	unsigned GetWeaponAmount()const{return weapon_amount;}
	unsigned GetPillAmount()const{return pill_amount;}
	unsigned GetPillOneAmount()const{return Pill_One_Amount;}
	unsigned GetPillTwoAmount()const{return Pill_Two_Amount;}
	unsigned GetPillThreeAmount()const{return Pill_Three_Amount;}
	unsigned GetPillFourAmount()const{return Pill_Four_Amount;}
	Weapon* GetWeaponOne()const{return weaponone;}
	Weapon* GetWeaponTwo()const{return weapontwo;}

	//take away pills
	void TakePillOne(unsigned num);
	void TakePillTwo(unsigned num);
	void TakePillThree(unsigned num);
	void TakePillFour(unsigned num);

	void TakeWeaponOneBul(unsigned num);
	void TakeWeaponTwoBul(unsigned num);
	void TakeWeaponOne(Weapon* _weapon);
	void TakeWeaponTwo(Weapon* _weapon);

	//take away the armor and leave the armor of oldnaijiu
	void TakeArmor(double oldnaijiu);
};


#endif
