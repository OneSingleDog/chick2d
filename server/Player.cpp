#include "Player.h"

void Player::InitalPlayer(int _player_id){
	player_id = _player_id;
	KillAmount = 0;
	Isdead = false;
//	BagIsFull = false;

	Player_Current_Hp = 100.0;
	Player_Total_Hp = 100.0;
	Hppercentage = 1.0;

	//bagamount = FULLBAG;
	//Player_Speed = DEFAULTSPEED;
	IsCuring = false;
	//IsLoading = false;
	HaveArmor = false;
	Armornaijiu = 0;

	//PlayerDirection = 1;

	MainWeapon = NULL;
	SubWeapon = NULL;

	PillsOne = 0;
	PillsTwo = 0;
	PillsThree = 0;
	PillsFour = 0;
}

void Player::ChangePosition(unsigned x,unsigned y){
	SetX(x);
	SetY(y);
}

void Player::CureStart(unsigned type,unsigned sttime){
	if(type == 0)return;
	switch(type){
		case 1:
			if(PillsOne == 0)return;
			IsCuring = true;
			curestarttime = sttime;
			curetype = type;
			break;
		case 2:
			if(PIllsTwo == 0)return;
			if(Player_Current_Hp >= 75.0)return;
			IsCuring = true;
			curestarttime = sttime;
			curetype = type;
			break;
		case 3:
			if(PillsThree == 0)return;
			IsCuring = true;
			curestarttime = sttime;
			curetype = type;
			break;
		case 4:
			if(PillsFour == 0)return;
			if(Player_Current_Hp >= 75.0)return ;
			IsCuring = true;
			curestarttime = sttime;
			curetype = type;
			break;
	}
	return;
}

void Player::CureEnd(unsigned edtime){
	if(!IsCuring)return;
	switch(curetype){
		case 1:
		    if(edtime - curestarttime < PILLONEDELAY)return;
			IsCuring = false;
			curetype = 0;
			Player_Current_Hp = 100.0;
			SetHpPercent();
			PillsOne --;
			break;
		case 2:
			if(edtime - curestarttime < PILLTWODELAY)return;
			IsCuring = false;
			curetype = 0;
			Player_Current_Hp = 75.0;
			SetHpPercent();
			PIllsTwo --;
			break;
		case 3:
			if(edtime - curestarttime < PILLTHREEDELAY)return;
			IsCuring = false;
			curetype = 0;
			Player_Current_Hp = Player_Current_Hp <= 90 ? Player_Current_Hp+10 : 100;
			SetHpPercent();
			PillsThree --;
			break;
		case 4:
		    if(edtime -curestarttime < PILLFOURDELAY)return;
			IsCuring = false;
			curetype = 0;
			Player_Current_Hp = Player_Current_Hp <= 65.0 ? Player_Current_Hp + 10 : 75;
			SetHpPercent();
			PillsFour --;
			break;
	}
	return ;
}

void Player::BeAttack(float damage){
	float coverdamage;
	if(damage * 0.4 >= Armornaijiu){
		coverdamage = 0.4 * Armornaijiu;
		Armornaijiu --;
		HaveArmor = false;
	}
	else {
		coverdamage = 0.4*damage;
	}
	float realdamage = damage - coverdamage;
	if(Player_Current_Hp >= realdamage)Player_Current_Hp -= realdamage;
	else {
		Player_Current_Hp = 0.0;
		Isdead = true;
	}
	SetHpPercent();
}

void Player::LossHp(float damage){
	if(Player_Current_Hp > damage)Player_Current_Hp -= damage;
	else {
		Player_Current_Hp = 0;
		Isdead = true;
	}
	SetHpPercent();
}

bool Player::Shoot(unsigned targetX,unsigned target Y,unsigned nowtime)
{
	float shootangle = 0.0;
	float dis = (float)sqrt((targetX - GetX())*(targetX - GetX()) + (targetY - GetY())*(targetY - GetY()));
	float disX = (float)(targetX - GetX());
	float disY = (float)(targetY - GetY());
	shootangle = (float)acos(dis/disX);
	if(disY < 0)shootangle = 2*PI - shootangle;
	if(MainWeapon == NULL)return false;
	shootangle = MainWeapon -> GetRandomFireAngle();
	ShootAngle = shootangle;
	//shootangle = MainWeapon
	return MainWeapon -> Fire(nowtime);
}

void Player::PickPillOne(unsigned num){
	PillsOne += num;
}

void Player::PickPillTwo(unsigned num){
	PillsTwo += num;
}

void Player::PickPillThree(unsigned num){
	PillsThree += num;
}

void Player::PickPillFour(unsigned num){
	PillsFour += num;
}

Weapon* Player::PickWeapon(Weapon* _weapon){
	if(_weapon == NULL)return NULL;
	bool flag = false;
	if(MainWeapon == NULL){
		MainWeapon = _weapon;
		_weapon = NULL;
	}
	//else if(SubWeapon == NULL)SubWeapon = _weapon;
	else{
		if(MainWeapon -> GetType() == _weapon -> GetType()){
			MainWeapon -> PickBullet(_weapon -> GetTotalBullet());
			_weapon -> SetTotalBullet(0);
		}
		else flag = true;
	}
	if(!flag)return _weapon;
	if(SubWeapon == NULL){
		SubWeapon = _weapon;
		_weapon = NULL;
	}
	else{
		if(SubWeapon -> GetType() == _weapon -> GetType()){
			SubWeapon -> PickBullet(_weapon -> GetTotalBullet());
			_weapon -> SetTotalBullet(0);
		}
		else {
			Weapon* temp;
			SubWeapon -> SetTotalBullet(SubWeapon->GetTotalBullet());
			SubWeapon = temp;
			SubWeapon = _weapon;
			_weapon = temp;
		}
	}
	return _weapon;
}

void Player::ExchangeWeapon(){
	IsCuring = false;
	Weapon* temp = NULL;
	if(MainWeapon != NULL)MainWeapon -> Exchange();
	temp = MainWeapon;
	MainWeapon = SubWeapon;
	SubWeapon = temp;
}

bool Player::LoadBullet(unsigend nowtime){
	if(MainWeapon == NULL)return false;
	if(MainWeapon -> GetBackupBullet() == 0)return false;
	MainWeapon -> LoadBegin(nowtime);
	return true;
}

// void Player::LoadBegin(unsigned sttime){
//
// }
