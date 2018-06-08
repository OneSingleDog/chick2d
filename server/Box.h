#ifndef __BOX_H__
#define __BOX_H__

#include "Player.h"
#include "node.h"

class Box:public node{
private:
	unsigned Boxid;						// 箱子id
	unsigned weapon_amount;				// 武器总数
	unsigned pill_amount;				// 药品总数

	Weapon* weaponone;					// 武器1
 	Weapon* weapontwo;					// 武器2

	unsigned Pill_One_Amount;			// 第一种药品的数量
	unsigned Pill_Two_Amount;			// 第二种药品的数量
	unsigned Pill_Three_Amount;			// 第三种药品的数量
	unsigned Pill_Four_Amount;			// 第四种药品的数量

	double Armornaijiu;				// 防弹衣耐久 0为没有防弹衣
public:
	Box(int _x,int _y,int _id):node(_x,_y),Boxid(_id){}
	// 随机生成box
	void InitBoxByRandom();
	// 死了以后掉的box
	void InitBoxByPlayer(Player*_player);
	// 获取box的id
	unsigned GetId()const{return Boxid;}
	// 获取防弹衣耐久
	double GetArmorNaijiu()const{return Armornaijiu;}
	// 获取武器的数量
	unsigned GetWeaponAmount()const{return weapon_amount;}
	// 获取药品的数量
	unsigned GetPillAmount()const{return pill_amount;}
	// 获取第一种药品的数量
	unsigned GetPillOneAmount()const{return Pill_One_Amount;}
	// 获取第二种药品的数量
	unsigned GetPillTwoAmount()const{return Pill_Two_Amount;}
	// 获取第三种药品的数量
	unsigned GetPillThreeAmount()const{return Pill_Three_Amount;}
	// 获取第四种药品的数量
	unsigned GetPillFourAmount()const{return Pill_Four_Amount;}
	// 获取武器1
	Weapon* GetWeaponOne()const{return weaponone;}
	// 获取武器2
	Weapon* GetWeaponTwo()const{return weapontwo;}

	// 拿走num个药品1
	void TakePillOne(unsigned num);
	// 拿走num个药品2
	void TakePillTwo(unsigned num);
	// 拿走num个药品3
	void TakePillThree(unsigned num);
	// 拿走num个药品4
	void TakePillFour(unsigned num);

	// 拿走weapon1 num个子弹
	void TakeWeaponOneBul(unsigned num);
	// 拿走weapon2 num个子弹
	void TakeWeaponTwoBul(unsigned num);
	// 用_weapon 替换weapon1
	void TakeWeaponOne(Weapon* _weapon);
	// 用_weapon 替换weapon2
	void TakeWeaponTwo(Weapon* _weapon);

	// 拿走防弹衣，换下耐久为oldnaijiu的旧防弹衣
	void TakeArmor(double oldnaijiu);
};


#endif
