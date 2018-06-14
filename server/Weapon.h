#ifndef __WEAPON_H__
#define __WEAPON_H__
#include <ctime>

#define PI 3.1415926535

#define FIST 5
#define SNIPER_RIFLE 0
#define ASSAULT_RIFLE 1
#define SHOTGUN 2
#define HANDGUN 3

#define SNIPER_RIFLE_DISTANCE 400
#define ASSAULT_RIFLE_DISTANCE 150
#define SHOTGUN_DISTANCE 70
#define HANDGUN_DISTANCE 100

#define SNIPER_RIFLE_SHOOT_DELAY 2500
#define ASSAULT_RIFLE_SHOOT_DELAY 250
#define SHOTGUN_SHOOT_DELAY 750
#define HANDGUN_SHOOT_DELAY 500

#define SNIPER_RIFLE_CUR_MAX_BUL 5
#define ASSAULT_RIFLE_CUR_MAX_BUL 20
#define SHOTGUN_CUR_MAX_BUL 2
#define HANDGUN_CUR_MAX_BUL 7

#define SNIPER_RIFLE_LOAD_TIME 4000
#define ASSAULT_RIFLE_LOAD_TIME 3000
#define SHOTGUN_LOAD_TIME 5000
#define HANDGUN_LOAD_TIME 3000

#define SNIPER_RIFLE_ANGLE 2.0 * PI / 36.0
#define ASSAULT_RIFLE_ANGLE 2.0 * PI / 18.0
#define SHOTGUN_ANGLE 2.0 * PI / 6.0
#define HANDGUN_ANGLE 2.0 * PI / 15.0

#define SNIPER_RIFLE_DAMAGE 100
#define ASSAULT_RIFLE_DAMAGE 30
#define SHOTGUN_DAMAGE 40
#define HANDGUN_DAMAGE 25

#define SHOTGUN_TIMES 3

class Weapon {
public:
	void  InitWeapon(unsigned type);

	double GetRandomFireAngle(double curangle);

	void  LoadBegin(unsigned bgtime);
	void  LoadEnd(unsigned edtime);

	bool  Fire(unsigned nowtime);

	void  Exchange();

	void  PickBullet(unsigned BulletCount);

	bool  IsLoadingBullet()const{return IsLoading;}

	double GetAngle()const{return angle;}
	double GetShootDelay()const{return shootdelay;}
	double GetLoadTime()const{return loadtime;}
	double GetDamage()const{return damage;}
	double GetDistance()const{return distance;}
	unsigned GetType()const{return WeaponType;}
	unsigned GetCurMaxBullet()const{return CurMaxBullet;}
	unsigned GetCurBullet()const{return CurBullet;}
	unsigned GetTotalBullet()const{return TotalBullet;} 
	unsigned GetBackupBullet()const{return BackupBullet;}

	void SetTotalBullet(unsigned num){TotalBullet = num;BackupBullet = num;CurBullet = 0;}//used to put weapon into box

private:
	unsigned WeaponType;
	unsigned CurMaxBullet;
	unsigned CurBullet;
	unsigned TotalBullet; 
	unsigned BackupBullet;

	bool IsLoading;
	unsigned Loadbegintime;

	unsigned Shootbegintime;

	double distance;
	double damage;
	double loadtime;
	double shootdelay;
	double angle;
};


#endif
