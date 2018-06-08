#include "Weapon.h"
#include <stdlib.h>
#include <time.h>

void Weapon::InitWeapon(unsigned type) {
	WeaponType = type;
	CurBullet = 0;
	TotalBullet = 0;
	BackupBullet = 0;

	IsLoading = false;
	Loadbegintime = 0;
	//IsShooting = false;
	Shootbegintime = 0;

	switch (type) {
	case SNIPER_RIFLE:
		CurMaxBullet = SNIPER_RIFLE_CUR_MAX_BUL;
		distance = SNIPER_RIFLE_DISTANCE;
		shootdelay = SNIPER_RIFLE_SHOOT_DELAY;
		angle = SNIPER_RIFLE_ANGLE;
		loadtime = SNIPER_RIFLE_LOAD_TIME;
		damage = SNIPER_RIFLE_DAMAGE;
		break;
	case ASSAULT_RIFLE:
		CurMaxBullet = ASSAULT_RIFLE_CUR_MAX_BUL;
		distance = ASSAULT_RIFLE_DISTANCE;
		shootdelay = ASSAULT_RIFLE_SHOOT_DELAY;
		angle = ASSAULT_RIFLE_ANGLE;
		loadtime = ASSAULT_RIFLE_LOAD_TIME;
		damage = ASSAULT_RIFLE_DAMAGE;
		break;
	case SHOTGUN:
		CurMaxBullet = SHOTGUN_CUR_MAX_BUL;
		distance = SHOTGUN_DISTANCE;
		shootdelay = SHOTGUN_SHOOT_DELAY;
		angle = SHOTGUN_ANGLE;
		loadtime = SHOTGUN_LOAD_TIME;
		damage = SHOTGUN_DAMAGE;
		break;
	case HANDGUN:
		CurMaxBullet = HANDGUN_CUR_MAX_BUL;
		distance = HANDGUN_DISTANCE;
		shootdelay = HANDGUN_SHOOT_DELAY;
		angle = HANDGUN_ANGLE;
		loadtime = HANDGUN_LOAD_TIME;
		damage = HANDGUN_DAMAGE;
		break;
	case FIST:
		CurMaxBullet = 0;
		distance = 0;
		shootdelay = 0;
		angle = 0;
		loadtime = 0;
		damage = 0;
		break;
	}
}
// uncomplete
double Weapon::GetRandomFireAngle(double curangle) {
	//srand((unsigned)time(NULL));
	double anglerange = rand()/(double)(RAND_MAX);
	anglerange *= angle;
	anglerange -= (angle/2);
	return curangle + anglerange;
}

void Weapon::LoadBegin(unsigned bgtime){
	if(IsLoading)return;
	if(BackupBullet == 0)return;
	if(CurBullet == CurMaxBullet)return;
	IsLoading = true;
	Loadbegintime = bgtime;
}

void Weapon::LoadEnd(unsigned edtime){
	if(IsLoading && edtime - Loadbegintime >= loadtime){
		unsigned resbull = CurMaxBullet - CurBullet;
		if(BackupBullet <= resbull){
			CurBullet += BackupBullet;
			BackupBullet = 0;
		}
		else {
			CurBullet = CurMaxBullet;
			BackupBullet -= resbull;
		}
		IsLoading = false;
	}
}

void Weapon::PickBullet(unsigned BulletCount) {
	BackupBullet += BulletCount;
	TotalBullet += BulletCount;
}

bool Weapon::Fire(unsigned nowtime){
	if(nowtime - Shootbegintime <= shootdelay)return false;
	if(CurBullet == 0)return false;
	if(IsLoading)return false;
	Shootbegintime = nowtime;
	CurBullet --;
	TotalBullet --;
	return true;
}

void Weapon::Exchange(){
	IsLoading = false;
}
