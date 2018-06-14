#include "Soldier.h"
#include <algorithm>
#include <cstdio>
#include <string>
#include <cmath>
USING_NS_CC;
using std::string;

Soldier::Soldier(int tp):type(tp) {
    body = arm = weapon = fire = shield = blood = MainWin = SubWin = mainWeaponShow = subWeaponShow = nullptr;
    shieldText = mainWeaponText = subWeaponText = nullptr;
    existLife = 100;
    shieldVal = 0;
    MainWeapon = 4;
    SubWeapon = 4;
    mainCurBulletNum = 0;
    mainTotBulletNum = 0;
    subCurBulletNum = 0;
    subTotBulletNum = 0;
    
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
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/wave.plist");
    for(int i = 1; i <= 6; ++i) {
        string name = std::to_string(i) + ".png";
        SpWave.pushBack(cocos2d::AnimationFrame::create(cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name.c_str()), 1, ValueMap()));
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
    if(MainWin != nullptr) { delete MainWin; }
    if(SubWin != nullptr) { delete SubWin; }
    if(mainWeaponShow != nullptr) { delete mainWeaponShow; }
    if(subWeaponShow != nullptr) { delete subWeaponShow; }
    if (circle != nullptr) { delete circle; }
    if(mainWeaponText != nullptr) { delete mainWeaponText; }
    if(subWeaponText != nullptr) { delete subWeaponText; }
	if (User_tag!=nullptr) { delete User_tag; }
}

string Soldier::armName[NUM_OF_WEAPON] = { "player/arm1.png", "player/arm1.png", "player/arm1.png", "player/arm1.png", "player/arm0.png" };
string Soldier::weaponName[NUM_OF_WEAPON] = { "player/weapon0_lookover.png", "player/weapon1_lookover.png", "player/weapon2_lookover.png", "player/weapon3_lookover.png", "player/blank.png" };
string Soldier::fireName[NUM_OF_WEAPON] = { "player/blank.png", "player/blank.png", "player/blank.png", "player/blank.png", "player/blank.png" };
string Soldier::weaponShowName[NUM_OF_WEAPON] = { "player/weaponshow0.png", "player/weaponshow1.png", "player/weaponshow2.png", "player/weaponshow3.png", "player/blank.png" };
int Soldier::maxBullet[NUM_OF_WEAPON] = { 5, 15, 2, 7, 0 };
float Soldier::circleSize[NUM_OF_WEAPON] = { 1.1, 0.61, 0.3, 0.4, 0.0001 };

void Soldier::create() {
    body = Sprite::create("player/body.png");
    arm = Sprite::create(Soldier::armName[MainWeapon]);
    weapon = Sprite::create(Soldier::weaponName[MainWeapon]);
    fire = Sprite::create("player/blank.png");
    circle = Sprite::create("player/circle.png");
    circle->setScale(Soldier::circleSize[MainWeapon]);

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
    
    MainWin = Sprite::create("others/win.png");
    MainWin->setScale(2.9, 1);
    SubWin = Sprite::create("others/win.png");
    SubWin->setScale(2.9, 1);
    
    mainWeaponShow = Sprite::create(Soldier::weaponShowName[MainWeapon]);
    subWeaponShow = Sprite::create(Soldier::weaponShowName[SubWeapon]);
    
    mainWeaponText = Label::createWithTTF((std::to_string(mainCurBulletNum) + "/" + std::to_string(mainTotBulletNum)).c_str(), "fonts/Marker Felt.ttf", 20);
    mainWeaponText->setTextColor(Color4B::BLACK);
    
    subWeaponText = Label::createWithTTF((std::to_string(subCurBulletNum) + "/" + std::to_string(subTotBulletNum)).c_str(), "fonts/Marker Felt.ttf", 20);
    subWeaponText->setTextColor(Color4B::BLACK);

	User_tag = Label::createWithTTF("", "fonts/Marker Felt.ttf", 20);

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
    circle->setPosition(x, y);

    auto size = Director::getInstance()->getWinSize();
    
    blood->setPosition(this->getPosition() - Vec2(0, size.height / 5 * 2)); //设置框的位置
    for(int i = 0; i < 3; ++i) {
        progress[i]->setPosition(this->getPosition() - Vec2(0, size.height / 5 * 2));
    }
    
    shield->setPosition(Vec2(x, y) - Vec2(-size.width * 0.97 / 5, size.height / 2.175));
    shieldText->setPosition(shield->getPosition());
    
    MainWin->setPosition(shield->getPosition() + Vec2(-2.5 * size.width / 8, 0));
    SubWin->setPosition(shield->getPosition() + Vec2(-size.width / 8, 0));
    
    mainWeaponShow->setPosition(MainWin->getPosition());
    subWeaponShow->setPosition(SubWin->getPosition());
    
    auto minisize = MainWin->getContentSize();
    
    mainWeaponText->setPosition(MainWin->getPosition() + Vec2(minisize.width, -minisize.height / 4 * 0.9));
    subWeaponText->setPosition(SubWin->getPosition() + Vec2(minisize.width, -minisize.height / 4 * 0.9));

	User_tag->setPosition(x, y+30);
}

void Soldier::addChild(Scene *scene, int level) {
    scene->addChild(body);
    scene->addChild(weapon);
    scene->addChild(arm);
    scene->addChild(fire);

	scene->addChild(circle, 1);
	scene->addChild(blood);
	for(int i = 0; i < 3; ++i) {
		scene->addChild(progress[i]);
	}
    
	scene->addChild(shield);
	scene->addChild(shieldText);
	scene->addChild(MainWin);
	scene->addChild(SubWin);
    
	scene->addChild(mainWeaponShow);
	scene->addChild(subWeaponShow);
    
	scene->addChild(mainWeaponText);
	scene->addChild(subWeaponText);
	scene->addChild(User_tag);

	if (type == 1) {
		circle->setVisible(false);
		blood->setVisible(false);
		for (int i = 0; i < 3; ++i) {
			progress[i]->setVisible(false);
		}
		shield->setVisible(false);
		shieldText->setVisible(false);
		MainWin->setVisible(false);
		SubWin->setVisible(false);

		mainWeaponShow->setVisible(false);
		subWeaponShow->setVisible(false);

		mainWeaponText->setVisible(false);
		subWeaponText->setVisible(false);
		
	}
	
}

Point Soldier::getPosition() {
    return body->getPosition();
}

float Soldier::getRotation() {
    return arm->getRotation();
}

void Soldier::changeWeapon() {
    arm->stopAllActions();
}

void Soldier::changeBullet() {
    if(MainWeapon == 4) {   // no weapon
        return;
    }
	if (mainCurBulletNum==maxBullet[MainWeapon])return;
	if (mainTotBulletNum==0)return;
	arm->stopAllActions();
    // create aninmation
    auto animation = Animation::create(SpFrs, 0.1);
    // create act
    auto animate= Animate::create(animation);
    // run
    
    auto seq = Sequence::create(animate, NULL);
    
    arm->runAction(seq);
}

void Soldier::Shoot() {
    arm->stopAllActions();

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

    // Just show animation
//    --mainCurBulletNum;
//    mainWeaponText->setString(std::to_string(mainCurBulletNum) + "/" + std::to_string(mainTotBulletNum));
    
    // run it
    auto seq = Sequence::create(animate, NULL);
    
    fire->runAction(seq);
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
    circle->setVisible(false);
    // create aninmation
    auto animation = Animation::create(SpExploit, 0.1);
    // create act
    auto animate= Animate::create(animation);
    // run
    
	auto callbackDying = CallFunc::create([=](){
		this->setVisible(false);
		});

	auto seq = Sequence::create(animate, callbackDying, NULL);
    body->runAction(seq);
}

void Soldier::hideStatus() {
    blood->setVisible(false);
    for(int i = 0; i < 3; ++i) {
        progress[i]->setVisible(false);
    }
    shield->setVisible(false);
    MainWin->setVisible(false);
    SubWin->setVisible(false);
    shieldText->setVisible(false);
    mainWeaponShow->setVisible(false);
    subWeaponShow->setVisible(false);
    mainWeaponText->setVisible(false);
    subWeaponText->setVisible(false);
	circle->setVisible(false);
}

void Soldier::showStatus() {
    blood->setVisible(true);
    updateBlood();
    shield->setVisible(true);
    MainWin->setVisible(true);
    SubWin->setVisible(true);
    shieldText->setVisible(true);
    mainWeaponShow->setVisible(true);
    subWeaponShow->setVisible(true);
    mainWeaponText->setVisible(true);
    subWeaponText->setVisible(true);
	circle->setVisible(true);
}

void Soldier::setHP(float newVal)
{
	existLife = newVal;
	updateBlood();
}

void Soldier::setShield(float newVal)
{
	shieldVal = newVal;
	shieldText->setString(std::to_string((int)(std::ceil(shieldVal)+0.01)));
}

void Soldier::setVisible(bool flag) {

	body->setVisible(flag);
	weapon->setVisible(flag);
	arm->setVisible(flag);
	fire->setVisible(flag);
	User_tag->setVisible(flag);
	if (type)return;
	circle->setVisible(flag);
	blood->setVisible(flag);
	for (int i = 0; i < 3; ++i) {
		progress[i]->setVisible(flag);
	}
	shield->setVisible(flag);
	shieldText->setVisible(flag);
	MainWin->setVisible(flag);
	SubWin->setVisible(flag);

	mainWeaponShow->setVisible(flag);
	subWeaponShow->setVisible(flag);

	mainWeaponText->setVisible(flag);
	subWeaponText->setVisible(flag);

}


void Soldier::setusername(std::string username) {
	if (User_tag == nullptr) return;
	User_tag->setString(username);
	if (type == 0) User_tag->setTextColor(Color4B::BLACK);
	else User_tag->setTextColor(Color4B::RED);
}

void Soldier::makeWave(cocos2d::Scene *scene) {
    auto wavePoint = Sprite::create("player/blank.png");
    wavePoint->setScale(1.5);
    wavePoint->setPosition(this->getPosition());
    scene->addChild(wavePoint);
    
    // create aninmation
    auto animation = Animation::create(SpWave, 1.0 / 6);
    // create act
    auto animate= Animate::create(animation);
    // run
    
    auto callbackWave = CallFunc::create([=](){
        scene->removeChild(wavePoint);
    });
    
    auto seq = Sequence::create(animate, callbackWave, NULL);
    
    wavePoint->runAction(seq);
}

void Soldier::setSubWeapon(int weaponType) {
    SubWeapon = weaponType;
    subWeaponShow->setTexture(Soldier::weaponShowName[SubWeapon]);
}

void Soldier::setMainWeapon(int weaponType) {
	if (MainWeapon==weaponType)return;
    MainWeapon = weaponType;
    mainWeaponShow->setTexture(Soldier::weaponShowName[MainWeapon]);
    weapon->setTexture(Soldier::weaponName[MainWeapon]);
    arm->setTexture(Soldier::armName[MainWeapon]);
    fire->setTexture(Soldier::fireName[MainWeapon]);
    circle->setScale(Soldier::circleSize[MainWeapon]);
}

void Soldier::setBullet(int Mcur, int Mback, int Scur, int Sback) {
    mainCurBulletNum = Mcur;
    mainTotBulletNum = Mback;
    subCurBulletNum = Scur;
    subTotBulletNum = Sback;
    
    mainWeaponText->setString((std::to_string(mainCurBulletNum) + "/" + std::to_string(mainTotBulletNum)).c_str());
    subWeaponText->setString((std::to_string(subCurBulletNum) + "/" + std::to_string(subTotBulletNum)).c_str());
}

