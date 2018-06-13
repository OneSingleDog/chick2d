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

	poison_time = -1;
}

void Player::ChangePosition(unsigned x,unsigned y){
	if (x==GetX()&&y==GetY())return;
	IsCuring = false;
	SetX(x);
	SetY(y);
}

void Player::CureStart(unsigned type,unsigned sttime){
	if(type == 0)return;
	switch(type){
		case 1:
			if(PillsOne == 0)return;
			if(Player_Current_Hp == 100.0)return;
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
			if(Player_Current_Hp >= 100.0)return;
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
	IsCuring = false;
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
	IsCuring = false;
	if(MainWeapon == NULL)return false;
	//ShootAngle = shootangle;
	return MainWeapon -> Fire(nowtime);
}

void Player::PickPillOne(unsigned num){
	IsCuring = false;
	PillsOne += num;
}

void Player::PickPillTwo(unsigned num){
	IsCuring = false;
	PillsTwo += num;
}

void Player::PickPillThree(unsigned num){
	IsCuring = false;
	PillsThree += num;
}

void Player::PickPillFour(unsigned num){
	IsCuring = false;
	PillsFour += num;
}

Weapon* Player::PickWeapon(Weapon* _weapon){
	IsCuring = false;
	if (MainWeapon==NULL)
		{
		MainWeapon = _weapon;
		return NULL;
		}
	else if (SubWeapon==NULL)
		{
		SubWeapon = _weapon;
		return NULL;
		}
	Weapon*tmp = MainWeapon;
	tmp->SetTotalBullet(tmp->GetTotalBullet());
	MainWeapon = _weapon;
	return tmp;
}

bool Player::PickBullet(int type, unsigned num){
	if (MainWeapon!=NULL && type==MainWeapon->GetType())MainWeapon->PickBullet(num);
	else if (SubWeapon!= NULL && type==SubWeapon->GetType())SubWeapon->PickBullet(num);
	else return false;
	return true;
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
	IsCuring = false;
	if(MainWeapon == NULL)return false;
	if(MainWeapon -> GetBackupBullet() == 0)return false;
	MainWeapon -> LoadBegin(nowtime);
	return true;
}

void Player::InPoison(unsigned nowtime, double dmg){
	if (poison_time<0)
		{
		poison_time = nowtime;
		return;
		}
	LossHp(dmg*(nowtime-poison_time));
	poison_time = nowtime;
}
