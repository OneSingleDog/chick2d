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

class Soldier {
private:
    cocos2d::Sprite *body;
    cocos2d::Sprite *arm;
    cocos2d::Sprite *weapon;
    cocos2d::Sprite *fire;
    int MainWeapon, SubWeapon;
    float existLife;
    
    cocos2d::Vector<cocos2d::AnimationFrame*> SpFrs;
    cocos2d::Vector<cocos2d::AnimationFrame*> SpExploit;

    static string armName[2];
    static string weaponName[2];
    static string fireName[2];

    cocos2d::Sprite *blood;
    cocos2d::ProgressTimer *progress[3];
    cocos2d::Sprite *shield;
    
    float shieldVal;
    
    cocos2d::Label *shieldText;
    
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
};

#endif /* Soldier_h */
