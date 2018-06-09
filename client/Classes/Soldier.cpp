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
#include <cmath>
USING_NS_CC;
using std::string;

Soldier::Soldier() {
    body = arm = weapon = fire = shield = blood = nullptr;
    MainWeapon = 0;
    shieldVal = 0;
    SubWeapon = 1;
    existLife = 100;
    shieldVal = 0;
    
    // animation
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/changeBullet.plist");
    for(int i = 704; i <= 735; ++i) {
        if(i >= 708 && i <= 710)continue;
        if(i == 716 || i == 730)continue;
        string name = std::to_string(i) + ".png";
        SpFrs.pushBack(AnimationFrame::create(SpriteFrameCache::getInstance()->getSpriteFrameByName(name.c_str()), 1, ValueMap()));
    }
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/exploit.plist");
    for(int i = 539; i <= 553; ++i) {
        string name = std::to_string(i) + ".png";
        SpExploit.pushBack(cocos2d::AnimationFrame::create(cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name.c_str()), 1, ValueMap()));
    }
}

Soldier::~Soldier() {
    if(body != nullptr) { delete body; }
    if(arm != nullptr) { delete arm; }
    if(weapon != nullptr) { delete weapon; }
    if(fire != nullptr) { delete fire; }
    if(blood != nullptr) { delete blood; }
    if(shield != nullptr) { delete shield; }
    if(shieldText != nullptr) { delete shieldText; }
}

string Soldier::armName[2] = { "player/arm0.png", "player/arm1.png" };
string Soldier::weaponName[2] = { "player/blank.png", "player/weapon1.png" };
string Soldier::fireName[2] = { "player/blank.png", "player/fire1.png" };

void Soldier::create() {
    body = Sprite::create("player/body.png");
    arm = Sprite::create(Soldier::armName[MainWeapon]);
    weapon = Sprite::create(Soldier::weaponName[MainWeapon]);
    fire = Sprite::create("player/blank.png");
    
    blood = Sprite::create("others/bar.png");   //创建进度框
    blood->setScale(0.7);
    auto sprBlood0 = Sprite::create("others/bloodG.png");  //创建血条
    auto sprBlood1 = Sprite::create("others/bloodY.png");
    auto sprBlood2 = Sprite::create("others/bloodR.png");
    progress[0] = ProgressTimer::create(sprBlood0);        //创建progress对象
    progress[1] = ProgressTimer::create(sprBlood1);        //创建progress对象
    progress[2] = ProgressTimer::create(sprBlood2);        //创建progress对象

    for(int i = 0; i < 3; ++i) {
        progress[i]->setType(ProgressTimer::Type::BAR);        //类型：条状
        progress[i]->setScale(0.7);
        //从右到左减少血量
        progress[i]->setMidpoint(Point(0,0.5));     //如果是从左到右的话，改成(1,0.5)即可
        progress[i]->setBarChangeRate(Point(1,0));
        progress[i]->setVisible(false);
    }
    
    this->updateBlood();
    
    shield = Sprite::create("others/shield.png");
    shield->setScale(0.1);
    shieldText = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 20);
}

void Soldier::updateBlood() {
    for(int i = 0; i < 3; ++i) {
        progress[i]->setPercentage(existLife);  //这里是百分制显示
    }
    progress[0]->setVisible(existLife >= 75);
    progress[1]->setVisible(existLife < 75 && existLife >= 25);
    progress[2]->setVisible(existLife < 25);
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

    auto size = Director::getInstance()->getWinSize();
    
    blood->setPosition(this->getPosition() - Vec2(0, size.height / 5 * 2)); //设置框的位置
    for(int i = 0; i < 3; ++i) {
        progress[i]->setPosition(this->getPosition() - Vec2(0, size.height / 5 * 2));
    }
    
    shield->setPosition(Vec2(x, y) - Vec2(-size.width * 0.97 / 5, size.height / 2.175));
    shieldText->setPosition(shield->getPosition());
}

void Soldier::addChild(Scene *scene, int level) {
    scene->addChild(body);
    scene->addChild(weapon);
    scene->addChild(arm);
    scene->addChild(fire);
    
    scene->addChild(blood);
    for(int i = 0; i < 3; ++i) {
        scene->addChild(progress[i]);
    }
    
    scene->addChild(shield);
    scene->addChild(shieldText);
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

void Soldier::getHurted(float delta) {
    float d = MIN(existLife, delta);
    existLife = MAX(0, existLife - delta + d * 0.2 * (fabs(shieldVal) > 1e-6));
    updateBlood();
    shieldVal -= d * 0.2;
    if(shieldVal < 0) { shieldVal = 0; }
    shieldText->setString(std::to_string( (int)(std::ceil(shieldVal) + 0.01) ));
    
    if( fabs(existLife) < 1e-6 ) {
        isDying();
    }
}

void Soldier::increaseShieldVal(float delta) {
    shieldVal += delta;
    if(shieldVal > 100) { shieldVal = 100; }
    shieldText->setString(std::to_string( (int)(std::ceil(shieldVal) + 0.01) ));
}

void Soldier::isDying() {
    arm->setVisible(false);
    weapon->setVisible(false);
    // create aninmation
    auto animation = Animation::create(SpExploit, 0.1);
    // create act
    auto animate= Animate::create(animation);
    // run
    body->runAction(animate);
}
