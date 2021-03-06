#include"Box.h"

void Box::InitBoxByRandom(){

	type = 0;
    unsigned randtemp = rand()%10;
    if(randtemp < 6)pill_amount = 0;
    else if(randtemp < 9)pill_amount = 1;
    else pill_amount = 2;

    Pill_One_Amount = 0;
    Pill_Two_Amount = 0;
    Pill_Three_Amount = 0;
    Pill_Four_Amount = 0;
    for(unsigned i = 0;i < pill_amount;++ i){
        randtemp = rand()%100;
        if(randtemp < 10)Pill_One_Amount ++;
        else if(randtemp < 35)Pill_Two_Amount ++;
        else if(randtemp < 60)Pill_Three_Amount ++;
        else Pill_Four_Amount ++;
    }

    if(pill_amount == 0){
        weapon_amount = 1;
    }
    else{
        randtemp = rand()%100;
        if(randtemp < 30)weapon_amount = 1;
        else weapon_amount = 0;
    }

    if(weapon_amount == 1){
        weapontwo = NULL;
        randtemp = rand()%100;
        if(randtemp < 10){
            weaponone = new Weapon;
            weaponone -> InitWeapon(SNIPER_RIFLE);
            unsigned temp = rand()%2;
            if(temp == 0)weaponone -> SetTotalBullet(5);
            else weaponone -> SetTotalBullet(10);
        }
        else if(randtemp < 35){
            weaponone = new Weapon;
            weaponone -> InitWeapon(ASSAULT_RIFLE);
            unsigned temp = rand()%2;
            if(temp == 0)weaponone -> SetTotalBullet(20);
            else weaponone -> SetTotalBullet(40);
        }
        else if(randtemp < 70){
            weaponone = new Weapon;
            weaponone -> InitWeapon(HANDGUN);
            unsigned temp = rand()%2;
            if(temp == 0)weaponone -> SetTotalBullet(15);
            else weaponone -> SetTotalBullet(30);
        }
        else {
            weaponone = new Weapon;
            weaponone -> InitWeapon(SHOTGUN);
            unsigned temp = rand()%2;
            if(temp == 0)weaponone -> SetTotalBullet(5);
            else weaponone -> SetTotalBullet(10);
        }
    }
    else{
        weaponone = NULL;
        weapontwo = NULL;
    }

    randtemp = rand()%100;
    if(randtemp < 30)Armornaijiu = 20;
    else Armornaijiu = 0;
}

void Box::InitBoxByAirdrop()
	{
	type = 1;
	weapon_amount = 2;
	pill_amount = 4;
	Pill_One_Amount = 1;
	Pill_Two_Amount = 1;
	Pill_Three_Amount = 1;
	Pill_Four_Amount = 1;
	Armornaijiu = 100;
	weaponone = new Weapon;
	weaponone->InitWeapon(SNIPER_RIFLE);
	weaponone->SetTotalBullet(10);
	weapontwo = new Weapon;
	if (rand()%2)
		{
		weapontwo->InitWeapon(SHOTGUN);
		weapontwo->SetTotalBullet(10);
		}
	else
		{
		weapontwo->InitWeapon(ASSAULT_RIFLE);
		weapontwo->SetTotalBullet(40);
		}
	}

void Box::InitBoxByPlayer(Player*_player){
	type = 0;
    pill_amount = _player -> GetPillOneAmount() + _player -> GetPillTwoAmount();
    pill_amount += _player -> GetPillThreeAmount() + _player -> GetPillFourAmount();
    Pill_One_Amount = _player -> GetPillOneAmount();
    Pill_Two_Amount = _player -> GetPillTwoAmount();
    Pill_Three_Amount = _player -> GetPillThreeAmount();
    Pill_Four_Amount = _player -> GetPillFourAmount();

    if(_player -> GetMainWeapon() != NULL){
        weapon_amount ++;
        weaponone = _player -> GetMainWeapon();
        weaponone -> SetTotalBullet(weaponone -> GetTotalBullet());
    }
    else weaponone = NULL;
    if(_player -> GetSubWeapon() != NULL){
        weapon_amount ++;
        weapontwo = _player -> GetSubWeapon();
        weapontwo -> SetTotalBullet(weapontwo -> GetTotalBullet());
    }
    else weapontwo = NULL;

    Armornaijiu = _player -> GetArmorNaijiu();
}

void Box::TakePillOne(unsigned num){
    if(num > Pill_One_Amount)return ;
    Pill_One_Amount -= num;
}

void Box::TakePillTwo(unsigned num){
    if(num > Pill_Two_Amount)return ;
    Pill_Two_Amount -= num;
}

void Box::TakePillThree(unsigned num){
    if(num > Pill_Three_Amount)return;
    Pill_Three_Amount -= num;
}

void Box::TakePillFour(unsigned num){
    if(num > Pill_Four_Amount)return ;
    Pill_Four_Amount -= num;
}

void Box::TakeWeaponOneBul(unsigned num){
    if(num > weaponone -> GetTotalBullet())return;
    weaponone -> SetTotalBullet(weaponone -> GetTotalBullet() - num);
}

void Box::TakeWeaponTwoBul(unsigned num){
    if(num > weapontwo -> GetTotalBullet())return;
    weapontwo -> SetTotalBullet(weapontwo -> GetTotalBullet() - num);
}

void Box::TakeArmor(double oldnaijiu){
    Armornaijiu = oldnaijiu;
}

void Box::TakeWeaponOne(Weapon* _weapon){
    weaponone = _weapon;
}

void Box::TakeWeaponTwo(Weapon* _weapon){
    weapontwo = _weapon;
}
