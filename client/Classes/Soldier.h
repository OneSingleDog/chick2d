//
//  Soldier.h
//  chick2d
//
//  Created by 侯宇蓬 on 2018/6/8.
//

#ifndef Soldier_h
#define Soldier_h

#include "cocos2d.h"
#include <string>
#include <algorithm>

using std::string;

const int NUM_OF_WEAPON = 2;

class Soldier {
private:
    float shieldVal;
    float existLife;
    int MainWeapon, SubWeapon;
    
    int mainCurBulletNum;
    int mainTotBulletNum;
    int subCurBulletNum;
    int subTotBulletNum;

    cocos2d::Sprite *body;
    cocos2d::Sprite *arm;
    cocos2d::Sprite *weapon;
    cocos2d::Sprite *fire;
    cocos2d::Sprite *circle;
   
    cocos2d::Vector<cocos2d::AnimationFrame*> SpFrs;
    cocos2d::Vector<cocos2d::AnimationFrame*> SpExploit;

    static string armName[NUM_OF_WEAPON];
    static string weaponName[NUM_OF_WEAPON];
    static string fireName[NUM_OF_WEAPON];
    static string weaponShowName[NUM_OF_WEAPON];
    static int maxBullet[NUM_OF_WEAPON];
    static float circleSize[NUM_OF_WEAPON];

    cocos2d::Sprite *blood;
    cocos2d::ProgressTimer *progress[3];
    cocos2d::Sprite *shield;
    cocos2d::Sprite *MainWin, *SubWin;
    
    cocos2d::Label *shieldText;
    cocos2d::Sprite *mainWeaponShow;
    cocos2d::Sprite *subWeaponShow;
    cocos2d::Label *mainWeaponText;
    cocos2d::Label *subWeaponText;

public:
    
    Soldier();
    ~Soldier();

    void create();
    
    void setRotation(float angle);
    void setPosition(float x, float y);
    
    void addChild(cocos2d::Scene *scene, int level = 0);
    
    cocos2d::Point getPosition();
    float getRotation();
    
    void changeWeapon();
    void changeBullet();
    void Shoot();

    void updateBlood();
    void getHurted(float delta);
    
    void increaseShieldVal(float delta = 100);
    void isDying();
    
    void hideStatus();
    void showStatus();
};

#endif /* Soldier_h */