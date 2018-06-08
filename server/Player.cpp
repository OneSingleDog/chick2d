#include "Player.h"

void Player::InitalPlayer(int _player_id,string _username){
	player_id = _player_id;
	killer_id = 0;
	username = _username;
	KillAmount = 0;
	Isdead = false;

	Player_Current_Hp = 100.0;
	Player_Total_Hp = 100.0;
	Hppercentage = 1.0;

	IsCuring = false;

	Armornaijiu = 0;

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
			if(PillsTwo == 0)return;
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
			PillsTwo --;
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

void Player::BeAttack(double damage,int from){
	double coverdamage = damage*0.4;
	if (coverdamage>=Armornaijiu)Armornaijiu = 0;
	else Armornaijiu -= coverdamage;
	double realdamage = damage - coverdamage;
	if(Player_Current_Hp >= realdamage)Player_Current_Hp -= realdamage;
	else {
		Player_Current_Hp = 0.0;
		Isdead = true;
		killer_id = from;
	}
	SetHpPercent();
}

void Player::LossHp(double damage){
	if(Player_Current_Hp > damage)Player_Current_Hp -= damage;
	else {
		Player_Current_Hp = 0;
		Isdead = true;
		killer_id = -1;
	}
	SetHpPercent();
}

bool Player::Shoot(unsigned nowtime)
{
	//double shootangle = 0.0;
	//double dis = (double)sqrt((targetX - GetX())*(targetX - GetX()) + (targetY - GetY())*(targetY - GetY()));
	//double disX = (double)(targetX - GetX());
	//double disY = (double)(targetY - GetY());
	//shootangle = (double)acos(dis/disX);
	//if(disY < 0)shootangle = 2*PI - shootangle;
	if(MainWeapon == NULL)return false;
	//ShootAngle = shootangle;
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
			Weapon* temp=0;
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

bool Player::LoadBullet(unsigned nowtime){
	if(MainWeapon == NULL)return false;
	if(MainWeapon -> GetBackupBullet() == 0)return false;
	MainWeapon -> LoadBegin(nowtime);
	return true;
}
