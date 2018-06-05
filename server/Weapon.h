#ifndef _WEAPON_H_
#define _WEAPON_H_

#define PI 3.1415926535

#define FIST -1
#define SNIPER_RIFLE 0
#define ASSAULT_RIFLE 1
#define SHOTGUN 2
#define HANDGUN 3

#define SNIPER_RIFLE_DISTANCE 100
#define ASSAULT_RIFLE_DISTANCE 50
#define SHOTGUN_DISTANCE 20
#define HANDGUN_DISTANCE 20

#define SNIPER_RIFLE_SHOOT_DELAY 100
#define ASSAULT_RIFLE_SHOOT_DELAY 20
#define SHOTGUN_SHOOT_DELAY 100
#define HANDGUN_SHOOT_DELAY 70

#define SNIPER_RIFLE_CUR_MAX_BUL 10
#define ASSAULT_RIFLE_CUR_MAX_BUL 30
#define SHOTGUN_CUR_MAX_BUL 3
#define HANDGUN_CUR_MAX_BUL 7

#define SNIPER_RIFLE_LOAD_TIME 10
#define ASSAULT_RIFLE_LOAD_TIME 8
#define SHOTGUN_LOAD_TIME 6
#define HANDGUN_LOAD_TIME 6

#define SNIPER_RIFLE_ANGLE 2.0 * PI / 36.0
#define ASSAULT_RIFLE_ANGLE 2.0 * PI / 18.0
#define SHOTGUN_ANGLE 2.0 * PI / 9.0
#define HANDGUN_ANGLE 2.0 * PI / 15.0

#define SNIPER_RIFLE_DAMAGE 100
#define ASSAULT_RIFLE_DAMAGE 40
#define SHOTGUN_DAMAGE 200
#define HANDGUN_DAMAGE 35

class Weapon {
public:
	void  InitWeapon(unsigned type);
	float GetRandomFireAngle(float curangle);
	void  LoadBegin(unsigned bgtime);
	void  LoadEnd(unsigned edtime);
	bool  Fire(unsigned nowtime)
	void  Shoot(float _angle);
	void  PickBullet(unsigned BulletCount);

	bool CanShoot()const{return canshoot;}
	float GetAngle()const{return angle;}
	//void SetAngle(float _angle){angle = _angle;}
	float GetShootDelay()const{return shootdelay;}
	float GetLoadTime()const{return loadtime;}
	float GetDamage()const{return damage;}
	float GetDistance()const{return distance;}
	unsigned GetType()const{return WeaponType;}
	unsigned GetCurMaxBullet()const{return CurMaxBullet;}
	unsigned GetCurBullet()const{return CurBullet;}
	unsigned GetTotalBullet()const{return TotalBullet;}
	unsigned GetBackupBullet()const{return BackupBullet;}

	void SetTotalBullet(unsigned num){TotalBullet = num;BackupBullet = num;CurBullet = 0;}

private:
	unsigned WeaponType;    // ����������
	unsigned CurMaxBullet;  // ��ǰ�����������ӵ���
	unsigned CurBullet;     // ��ǰ���е��ӵ���
	unsigned TotalBullet;   // ��ǰ���ж����ӵ�
	unsigned BackupBullet;

	bool IsLoading;
	unsigned Loadbegintime;

	//bool IsShooting;
	unsigned Shootbegintime;

	float distance;         // ����������
	float damage;			// �������˺�ֵ
	float loadtime;			// ����ʱ��
	float shootdelay;		// ��������
	float angle;            // �����������Ƕ�
	bool  canshoot;			// ���������ı�־λ
};


#endif
