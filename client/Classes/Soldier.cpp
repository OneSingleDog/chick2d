//
//  Soldier.cpp
//  chick2d
//
//  Created by 侯宇蓬 on 2018/6/8.
//

#include "Soldier.h"
#include <algorithm>
#include <cstdio>
#include <string>
USING_NS_CC;
using std::string;

Soldier::Soldier() {
    body = arm = weapon = fire = nullptr;
    MainWeapon = 0;
    SubWeapon = 1;
    
    // animation
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/changeBullet.plist");
    for(int i = 704; i <= 735; ++i) {
        if(i >= 708 && i <= 710)continue;
        if(i == 716 || i == 730)continue;
        string name = to_string(i) + ".png";
        SpFrs.pushBack(AnimationFrame::create(SpriteFrameCache::getInstance()->getSpriteFrameByName(name.c_str()), 1, ValueMap()));
    }
}

Soldier::~Soldier() {
    if(body != nullptr) { delete body; }
    if(arm != nullptr) { delete arm; }
    if(weapon != nullptr) { delete weapon; }
    if(fire != nullptr) { delete fire; }
}

string Soldier::armName[2] = { "player/arm0.png", "player/arm1.png" };
string Soldier::weaponName[2] = { "player/blank.png", "player/weapon1.png" };
string Soldier::fireName[2] = { "player/blank.png", "player/fire1.png" };

void Soldier::create() {
    body = Sprite::create("player/body.png");
    arm = Sprite::create(Soldier::armName[MainWeapon]);
    weapon = Sprite::create(Soldier::weaponName[MainWeapon]);
    fire = Sprite::create("player/blank.png");
}

void Soldier::setRotation(float angle) {
    weapon->setRotation(angle);
    arm->setRotation(angle);
    fire->setRotation(angle);
}

void Soldier::setPosition(float x, float y) {
    body->setPosition(x, y);
    weapon->setPosition(x, y);
    arm->setPosition(x, y);
    fire->setPosition(x, y);
}

void Soldier::addChild(Scene *scene, int level) {
    scene->addChild(body);
    scene->addChild(weapon);
    scene->addChild(arm);
    scene->addChild(fire);
}

Point Soldier::getPosition() {
    return body->getPosition();
}

float Soldier::getRotation() {
    return arm->getRotation();
}

void Soldier::changeWeapon() {
    arm->stopAllActions();
    arm->setTexture(Soldier::armName[SubWeapon]);
    weapon->setTexture(Soldier::weaponName[SubWeapon]);
    std::swap(MainWeapon, SubWeapon);
    arm->setPosition(body->getPosition());
}

void Soldier::changeBullet() {
    if(MainWeapon == 0) {   // no weapon
        return;
    }
    // create aninmation
    auto animation = Animation::create(SpFrs, 0.1);
    // create act
    auto animate= Animate::create(animation);
    // run
    arm->runAction(animate);
}

void Soldier::Shoot() {
    if(MainWeapon == 0) {
        return;
    }
    // now lets animate the sprite we moved
    Vector<SpriteFrame*> animFrames;
    animFrames.pushBack(SpriteFrame::create(Soldier::fireName[MainWeapon], Rect(0,0,270,90)));
    animFrames.pushBack(SpriteFrame::create("player/blank.png", Rect(0, 0, 28, 28)));
    
    // create the animation out of the frames
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    Animate* animate = Animate::create(animation);
    
    // run it
    fire->runAction(animate);
}
