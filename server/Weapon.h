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
	void  InitWeapon(unsigned type); // 武器的初始化
	float GetRandomFireAngle(float curangle); // 根据玩家选定的射击角在武器偏角范围内随机生成最终角度
	void  LoadBegin(unsigned bgtime);  // 装弹开始
	void  LoadEnd(unsigned edtime);    // 装弹结束，每次收到包后都要判断
	bool  Fire(unsigned nowtime);      // 开火，返回是否开火成功
	void  Exchange();				   // 当玩家调用更换武器时，调用该函数，主要是shut掉换弹操作
	void  PickBullet(unsigned BulletCount); // 捡子弹，子弹会加入备弹中
	bool  IsLoadingBullet()const{return IsLoading}
	//bool CanShoot()const{return canshoot;}
	float GetAngle()const{return angle;} // 获取武器的偏角
	//void SetAngle(float _angle){angle = _angle;}
	float GetShootDelay()const{return shootdelay;} // 获取射击间隔
	float GetLoadTime()const{return loadtime;} // 获取换弹时间
	float GetDamage()const{return damage;}   // 获取伤害值
	float GetDistance()const{return distance;} // 获取射程
	unsigned GetType()const{return WeaponType;} // 获取武器的type
	unsigned GetCurMaxBullet()const{return CurMaxBullet;} // 获取弹夹容量
	unsigned GetCurBullet()const{return CurBullet;} // 获取当前弹夹内子弹数
	unsigned GetTotalBullet()const{return TotalBullet;} // 获取总共子弹数
	unsigned GetBackupBullet()const{return BackupBullet;} // 获取备弹数
	// 设置共有多少子弹，主要用于玩家死亡时和箱子中的随机生成
	void SetTotalBullet(unsigned num){TotalBullet = num;BackupBullet = num;CurBullet = 0;}

private:
	unsigned WeaponType;    // 武器种类
	unsigned CurMaxBullet;  // 弹夹容量
	unsigned CurBullet;     // 当前弹夹内子弹数
	unsigned TotalBullet;   // 总共的子弹数
	unsigned BackupBullet;  // 备弹数

	bool IsLoading;         // 标志正在进行装弹操作
	unsigned Loadbegintime; // 记录开始装弹的时间

	//bool IsShooting;
	unsigned Shootbegintime; // 记录上次射击的时间

	float distance;         // 武器的射程
	float damage;			// 武器的伤害
	float loadtime;			// 装弹时间
	float shootdelay;		// 射击间隔
	float angle;            // 射击偏角
};


#endif
