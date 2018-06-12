#include "MainScene.h"
#include "ConnectfailScene.h"
#include "SimpleAudioEngine.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <cstring>
#include <Notice.h>

#include "msg.h"
#include "post.h"

USING_NS_CC;

double MainScene::BASIC_SPEED = 2.5;
int MainScene::SPEED_RATIO = 1;  // 1 if not running
const int LITTLE_MAP_SIZE = 512;

int cnt_666 = 0;

struct Box {
    int x, y;
    int weapon1;
    int weapon2;
    int bullet1;
    int bullet2;
    int pillNum[4];
    int shield;
    Box(int x = -1, int y = -1, int w1 = 0, int w2 = 0, int b1 = 0, int b2 = 0, int p0 = 0, int p1 = 0, int p2 = 0, int p3 = 0, int s = 0) :
    x(x), y(y), weapon1(w1), weapon2(w2), bullet1(b1), bullet2(b2), shield(s) {
        pillNum[0] = p0;
        pillNum[1] = p1;
        pillNum[2] = p2;
        pillNum[3] = p3;
    }
};

std::vector<Box> Box_ve;
float MainScene::SafezoneScaleSize[4] = { 9,4.5,2.25,1.125 };

Scene* MainScene::createScene() {
    return MainScene::create();
}

const string PillName[4] = { "others/medical_kit.png", "others/first_aid.png", "others/enr_drink.png", "others/bandage.png" };

// on "init" you need to initialize your instance
bool MainScene::init()
{
    
    playerID = 0;
    isOpenSight = false;
    isOpenBox = false;
    isOpenMap = false;
    
    Box_ve.push_back(Box(80, 64, 1, 1, 60, 30,  1, 0, 1, 0, 100));
    Box_ve.push_back(Box(64, 80, 1, 0, 60, 0, 1, 1, 1, 1, 100));

    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    player = new Soldier(0);
	for (int i = 0;i<SOLDIER_NUM;++i)enemy[i] = new Soldier(1);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // create Main Map using Map.tmx
    MainMap = TMXTiledMap::create("map/Map4.tmx");
    
    // Get background layer
    //background = MainMap->getLayer("Background");
    //house1 = MainMap->getLayer("house1");
    collidable = MainMap->getLayer("Collidable");
    collidable->setVisible(false);
    
    // Get object group
    TMXObjectGroup *objects = MainMap->getObjectGroup("object");
    CCASSERT(NULL != objects, "'object' object group not found");
    
    // Get object
    auto playerShowUpPoint = objects->getObject("start");
    CCASSERT(!playerShowUpPoint.empty(), "start object not found");
    
    int x = playerShowUpPoint["x"].asInt();
    int y = playerShowUpPoint["y"].asInt();
    
    // create player and set position
    player->create();
    player->setPosition(x + MainMap->getTileSize().width / 2,
                        y + MainMap->getTileSize().height / 2);
    
    
    
    addChild(MainMap, -5);
    player->addChild(this);
    setViewPointCenter(player->getPosition());

	//create enemy and set position
	for(int i=0;i<SOLDIER_NUM;++i)
		{
		enemy[i]->create();
		enemy[i]->setPosition(0,0);
		enemy[i]->setVisible(false);
		enemy[i]->addChild(this);
		}
    
    // mouse create
    mouse = Sprite::create("others/mouse.png");
    mouse->setScale(0.04);
    addChild(mouse, 20);
    
    // fog create
    fog = Sprite::create("others/fog.png");
    fog->setPosition(player->getPosition());
    fog->setScale(2);
    addChild(fog, 5);
    
    // sight create
    sight = Sprite::create("others/sight.png");
    sight->setPosition(player->getPosition());
    addChild(sight, 10);
    sight->setVisible(false);
    
    // boxes create
    for(auto it = Box_ve.begin(); it != Box_ve.end(); ++it) {
        auto tmpb = Sprite::create("others/box.png");
        addChild(tmpb, -1);
        float w = MainMap->getTileSize().width;
        tmpb->setPosition((*it).x * w, (*it).y * w);
    }
    
    /*
        Mouse Event
     */
    
    // mouse listener
    auto myMouseListener = EventListenerMouse::create();
    
    // mouse move
    myMouseListener->onMouseMove = [=](Event *event) {
        this->setMousePosition(event);
        Vec2 delta = mouse->getPosition() - player->getPosition();
        delta.normalize();
        float angle = 180 * (delta.x < 0) + (delta.x > 0 ? -1 : 1) * asin(delta.y) / acos(0.0) * 90; // get the rotate angle
        player->setRotation(angle);
        fog->setRotation(angle);
    };
    
    // mouse down
    myMouseListener->onMouseDown = [=](Event *event) {
        log("Fire! %f", this->getPositionZ());
        EventMouse *e = (EventMouse*)event;
        int op = (int)e->getMouseButton();
        // 0 -> left button
        // 1 -> right button
        if(op == 1) {
            if(SPEED_RATIO != 1) { return; }
            if(!isOpenSight) {
                openSight();
            } else {
                closeSight();
            }
        } else {
            if(SPEED_RATIO != 1) { return; }
            player->Shoot();
            player->makeWave(this);
        }
    };
    
    // bound
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myMouseListener, this);

    /*
        End of Mouse Event
        Keyboard Event
     */
    
    // creating a keyboard event listener
    auto myKeyboardListener = EventListenerKeyboard::create();
    myKeyboardListener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
    myKeyboardListener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
    
    // bound
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myKeyboardListener, this);
    
    /*
        End of Keyboard Event
        Schedule of Action Move
     */
    
    auto size = Director::getInstance()->getWinSize();
    
    schedule(schedule_selector(MainScene::myMoveAction), (float)(1.0 / 60), kRepeatForever, 0);
	schedule(schedule_selector(MainScene::try_receive), (float)(1.0/60), kRepeatForever, 0);
    
    glass = Sprite::create("others/glass.png");
    addChild(glass);
    glass->setPosition(visibleSize / 2);
    glass->setVisible(false);
    
    littleMap = Sprite::create("others/littlemap.png");
    addChild(littleMap, 30);
    littleMap->setPosition(player->getPosition());
    littleMap->setVisible(false);
    littleMap->setScale(0.5);
    
    littlePoint = Sprite::create("others/point.png");
    addChild(littlePoint, 30);
    littlePoint->setPosition(player->getPosition());
    littlePoint->setVisible(false);
    
    to_ready = cocos2d::Label::createWithTTF("Choose a position and press space to get ready", "fonts/Marker Felt.ttf", 50);
    to_ready->setTextColor(Color4B::BLACK);
    to_ready->setPosition(player->getPosition().x, player->getPosition().y + visibleSize.height / 2 - 30);
    addChild(to_ready);

	already = cocos2d::Label::createWithTTF(" ", "fonts/Marker Felt.ttf", 40);
	already->setTextColor(Color4B::BLACK);
	//already->setString(std::to_string(ready_person) + "/ 4");
	already->setPosition(Vec2::ZERO);
	addChild(already,2);
	already->setVisible(false);
	
	Safe_Zone = cocos2d::Sprite::create("others/poison_range.png");
	Safe_Zone->setPosition(MainMap->getMapSize().width / 2  * 32, MainMap->getMapSize().height / 2 *  32);
	//Safe_Zone->setPosition(MainMap->getMapSize().width / 2 * 32-100, MainMap->getMapSize().height / 2 * 32-100);
	Safe_Zone->setScale(9);
	//Safe_Zone->setScale(4.5);
	addChild(Safe_Zone);

	littleSafeZone = Sprite::create("others/poison_range.png");
	addChild(littleSafeZone, 30);
	littleSafeZone->setPosition(player->getPosition());
	littleSafeZone->setScale(1.11);
	littleSafeZone->setVisible(false);

	Medical_kit = Sprite::create(PillName[0]);
	addChild(Medical_kit);
	Medical_kit->setScale(0.3);
	Medical_kit->setPosition(player->getPosition().x + visibleSize.width / 2 - 85, player->getPosition().y - visibleSize.height / 2+200);

	First_aid = Sprite::create(PillName[1]);
	addChild(First_aid);
	First_aid->setScale(0.6);
	First_aid->setPosition(player->getPosition().x + visibleSize.width / 2 - 85, player->getPosition().y - visibleSize.height / 2 + 135);

    Drink = Sprite::create(PillName[2]);
    addChild(Drink);
    Drink->setScale(0.25);
    Drink->setPosition(player->getPosition().x + visibleSize.width / 2 - 85, player->getPosition().y - visibleSize.height / 2 + 30);
    
	Bandage = Sprite::create(PillName[3]);
	addChild(Bandage);
	Bandage->setScale(0.23);
	Bandage->setPosition(player->getPosition().x + visibleSize.width / 2 - 85, player->getPosition().y - visibleSize.height / 2 + 85);

	Medical_cnt = cocos2d::Label::createWithTTF("0", "fonts/Marker Felt.ttf", 30);
	Medical_cnt->setTextColor(Color4B::BLACK);
	Medical_cnt->setPosition(player->getPosition().x + visibleSize.width / 2 -35, player->getPosition().y - visibleSize.height / 2 + 200);
	addChild(Medical_cnt);

	Firstaid_cnt=cocos2d::Label::createWithTTF("0", "fonts/Marker Felt.ttf", 30);
	Firstaid_cnt->setTextColor(Color4B::BLACK);
	Firstaid_cnt->setPosition(player->getPosition().x + visibleSize.width / 2 - 35, player->getPosition().y - visibleSize.height / 2 + 135);
	addChild(Firstaid_cnt);

	Bandage_cnt = cocos2d::Label::createWithTTF("0", "fonts/Marker Felt.ttf", 30);
	Bandage_cnt->setTextColor(Color4B::BLACK);
	Bandage_cnt->setPosition(player->getPosition().x + visibleSize.width / 2 - 35, player->getPosition().y - visibleSize.height / 2 + 80);
	addChild(Bandage_cnt);

	Drink_cnt = cocos2d::Label::createWithTTF("0", "fonts/Marker Felt.ttf", 30);
	Drink_cnt->setTextColor(Color4B::BLACK);
	Drink_cnt->setPosition(player->getPosition().x + visibleSize.width / 2 - 35, player->getPosition().y - visibleSize.height / 2 + 30);
	addChild(Drink_cnt);
	
	DeadLayer = cocos2d::Sprite::create("others/glass.png");
	DeadLayer->setPosition(player->getPosition());
	addChild(DeadLayer,40);
	DeadLayer->setVisible(false);

	Warning = cocos2d::Sprite::create("others/warn.png");
	Warning->setPosition(player->getPosition().x - 250, player->getPosition().y - visibleSize.height / 2 + 30);
	addChild(Warning);
	Warning->setScale(0.15);
	Warning->setVisible(false);
    
    Healing = cocos2d::Sprite::create("others/heal.png");
    Healing->setPosition(Warning->getPosition() + Vec2(0, visibleSize.height / 16));
    Healing->setScale(0.15);
    addChild(Healing);
    Healing->setVisible(true);

	Notice = cocos2d::Label::createWithTTF("", "fonts/Marker Felt.ttf", 30);
	Notice->setTextColor(Color4B::BLACK);
	Notice->setPosition(player->getPosition().x + visibleSize.width / 2 - 200, player->getPosition().y + visibleSize.height / 2 - 80);
	addChild(Notice);

	Remain = cocos2d::Label::createWithTTF("4 / 4", "fonts/Marker Felt.ttf", 50);
	Remain->setTextColor(Color4B::ORANGE);
	Remain->setPosition(player->getPosition().x + visibleSize.width / 2 - 100, player->getPosition().y + visibleSize.height / 2 - 40);
	addChild(Remain);
	Remain->setVisible(false);

    Wflag = Aflag = Sflag = Dflag = false;
    
    return true;
}

void MainScene::openSight() {
    if(isOpenMap) {
        closeMap();
    }
    player->hideStatus();
    
    float angle = -player->getRotation();
    if(angle < 0) { angle += 360; }
    angle = angle / 180.0 * acos(-1.0);
    
    float d = MainMap->getTileSize().width * 18;
    Vec2 ve(d * cos(angle), d * sin(angle));
    
    this->setPosition(this->getPosition() - ve);
    auto c = this->getCameras();
    
    mouse->setPosition(mouse->getPosition() + ve);
    sight->setPosition(player->getPosition() + ve);
    sight->setVisible(true);
    fog->setVisible(false);
    isOpenSight = true;
}

void MainScene::closeSight() {
    player->showStatus();
    
    setViewPointCenter(player->getPosition());
    fog->setPosition(player->getPosition());
    sight->setVisible(false);
    fog->setVisible(true);
    isOpenSight = false;
}


void MainScene::setMousePosition(cocos2d::Event *event) {
    EventMouse *e = (EventMouse*)event;
    auto viewpos = this->getPosition();
    float x = e->getCursorX() - viewpos.x;
    float y = e->getCursorY() - viewpos.y;
    mouse->setPosition(x, y);
}

void MainScene::myMoveAction(float dt) {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    direction = Vec2::ZERO;
    if(Wflag) { direction.y += BASIC_SPEED; }
    if(Aflag) { direction.x -= BASIC_SPEED; }
    if(Sflag) { direction.y -= BASIC_SPEED; }
    if(Dflag) { direction.x += BASIC_SPEED; }
	if (player!=nullptr && fabs(direction.length())>1e-6) {
		bool xflag = false, yflag = false;
		Point ori((player->getPosition()+direction*2.0).x, (player->getPosition()+direction*2.0).y);

		if (this->judgePlayerPosition(ori)!=0)
			{
			if (this->judgePlayerPosition(Point(player->getPosition().x, ori.y))==0)
				{
				yflag = true;
				}
			if (this->judgePlayerPosition(Point(ori.x, player->getPosition().y))==0)
				{
				xflag = true;
				}
			}
		else
			{
			xflag = yflag = true;
			}
		if (BASIC_SPEED>=10) { xflag = yflag = true; }
		Point new_pos((player->getPosition()+direction*SPEED_RATIO).x, (player->getPosition()+direction*SPEED_RATIO).y);
		if (xflag==false) { new_pos.x = player->getPosition().x; }
		if (yflag==false) { new_pos.y = player->getPosition().y; }
		player->setPosition(new_pos.x, new_pos.y);
        setViewPointCenter(player->getPosition());
        if(isOpenSight) {
            closeSight();
        }
        if(isOpenBox) {
            closeBox();
        }
        if (Notice != nullptr) {
            Notice->setPosition(player->getPosition().x + visibleSize.width / 2 - 200, player->getPosition().y + visibleSize.height / 2 - 20);
        }
        //Notice->setPosition(player->getPosition().x + visibleSize.width / 2 - 10, player->getPosition().y + visibleSize.height / 2 - 10);
        to_ready->setPosition(player->getPosition().x, player->getPosition().y + visibleSize.height / 2 - 30);
        littleMap->setPosition(player->getPosition());
        
        float xrate = player->getPosition().x / (MainMap->getTileSize().width * MainMap->getMapSize().width);
        float yrate = player->getPosition().y / (MainMap->getTileSize().height * MainMap->getMapSize().width);
        
        Vec2 delta = LITTLE_MAP_SIZE * Vec2(xrate - 0.5, yrate - 0.5);
        littlePoint->setPosition(delta + player->getPosition());
        
        Medical_kit->setPosition(player->getPosition().x + visibleSize.width / 2 - 85, player->getPosition().y - visibleSize.height / 2 + 200);
        First_aid->setPosition(player->getPosition().x + visibleSize.width / 2 - 85, player->getPosition().y - visibleSize.height / 2 + 135);
        Bandage->setPosition(player->getPosition().x + visibleSize.width / 2 - 85, player->getPosition().y - visibleSize.height / 2 + 85);
        Drink->setPosition(player->getPosition().x + visibleSize.width / 2 - 85, player->getPosition().y - visibleSize.height / 2 + 30);
        
        Medical_cnt->setPosition(player->getPosition().x + visibleSize.width / 2 - 35, player->getPosition().y - visibleSize.height / 2 + 200);
        Firstaid_cnt->setPosition(player->getPosition().x + visibleSize.width / 2 - 35, player->getPosition().y - visibleSize.height / 2 + 135);
        Bandage_cnt->setPosition(player->getPosition().x + visibleSize.width / 2 - 35, player->getPosition().y - visibleSize.height / 2 + 80);
        Drink_cnt->setPosition(player->getPosition().x + visibleSize.width / 2 - 35, player->getPosition().y - visibleSize.height / 2 + 30);
		
		Warning->setPosition(player->getPosition().x - 250, player->getPosition().y - visibleSize.height / 2 + 30);
		Notice->setPosition(player->getPosition().x + visibleSize.width / 2 - 200, player->getPosition().y + visibleSize.height / 2 - 80);
		Remain->setPosition(player->getPosition().x + visibleSize.width / 2 - 100, player->getPosition().y + visibleSize.height / 2 - 40);

        
        Healing->setPosition(Warning->getPosition() + Vec2(0, visibleSize.height / 16));
    }
    
    littleSafeZone->setPosition(littleMap->getPosition()+(Safe_Zone->getPosition()-MainMap->getMapSize()*16)/8);
}

// 0 -> can move
// 1 -> cannot move
int MainScene::judgePlayerPosition(Point position) {
    Point tileCoord = this->tileCoordFromPosition(position+Vec2(14,14));
    int tileGid = collidable->getTileGIDAt(tileCoord);// contervt to tiled site
    
    if (tileGid > 0) { //in the collidable layer
        Value prop = MainMap->getPropertiesForGID(tileGid);
        ValueMap propValueMap = prop.asValueMap();
        
        //auto propValueMap = _tiledmap->getPropertiesForGID(tileGid).asValueMap();
        
        std::string collision = propValueMap["collidable"].asString();
        
        if (collision == "true") {
            return 1;
        }
    }

	tileCoord = this->tileCoordFromPosition(position+Vec2(-14, 14));
	tileGid = collidable->getTileGIDAt(tileCoord);// contervt to tiled site

	if (tileGid > 0)
		{ //in the collidable layer
		Value prop = MainMap->getPropertiesForGID(tileGid);
		ValueMap propValueMap = prop.asValueMap();

		//auto propValueMap = _tiledmap->getPropertiesForGID(tileGid).asValueMap();

		std::string collision = propValueMap["collidable"].asString();

		if (collision=="true")
			{
			return 1;
			}
		}

	tileCoord = this->tileCoordFromPosition(position+Vec2(14, -14));
	tileGid = collidable->getTileGIDAt(tileCoord);// contervt to tiled site

	if (tileGid > 0)
		{ //in the collidable layer
		Value prop = MainMap->getPropertiesForGID(tileGid);
		ValueMap propValueMap = prop.asValueMap();

		//auto propValueMap = _tiledmap->getPropertiesForGID(tileGid).asValueMap();

		std::string collision = propValueMap["collidable"].asString();

		if (collision=="true")
			{
			return 1;
			}
		}

	tileCoord = this->tileCoordFromPosition(position+Vec2(-14, -14));
	tileGid = collidable->getTileGIDAt(tileCoord);// contervt to tiled site

	if (tileGid > 0)
		{ //in the collidable layer
		Value prop = MainMap->getPropertiesForGID(tileGid);
		ValueMap propValueMap = prop.asValueMap();

		//auto propValueMap = _tiledmap->getPropertiesForGID(tileGid).asValueMap();

		std::string collision = propValueMap["collidable"].asString();

		if (collision=="true")
			{
			return 1;
			}
		}

    return 0;
}

Point MainScene::tileCoordFromPosition(Point pos)
{
    int x = pos.x / MainMap->getTileSize().width;
    int y = ((MainMap->getMapSize().height * MainMap->getTileSize().height) - pos.y) /
    MainMap->getTileSize().height;
    
    return Point(x, y);
}

/*
 *  146  W
 *  142  S
 *  124  A
 *  127  D
 *  140  Q
 *  141  R
 *  125  B
 *  136  M
 *  129  F
 *  12   Left Shift
 *  134  K
 */
void MainScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    int op = (int)keyCode;
    switch (op) {
        case 146:   // W
            Wflag = true;
            break;
            
        case 142:   // S
            Sflag = true;
            break;
            
        case 124:   // A
            Aflag = true;
            break;
            
        case 127:   // D
            Dflag = true;
            break;
            
        case 140:   // Q
            if(SPEED_RATIO != 1) { break; }
            if(isOpenSight) {
                closeSight();
            }
            player->changeWeapon();
            break;
            
        case 141:   // R
            if(SPEED_RATIO != 1) { break; }
            player->changeBullet();
            break;
            
        case 12:    // LS
            MainScene::SPEED_RATIO = 2;
            break;
            
        case 134:   // K
            player->getHurted(20);
            break;
            
        case 135:   // L
            player->increaseShieldVal(20);
            break;
            
        case 82:    // 6
            ++cnt_666;
            if(cnt_666 >= 3) {
                BASIC_SPEED = 20;
            }
            break;
            
        case 129:   // F
            //log("%f %f", player->getPosition().x, player->getPosition().y);
            if(isOpenBox) {
                closeBox();
            } else {
                CheckBoxes();
                //log("%f %f", boxMenu->getPosition().x, boxMenu->getPosition().y);
            }
            
            break;
//
//        case 19:
//            OpenBox(0);
//            break;
            
        case 136:   // M
            if(isOpenMap) {
                closeMap();
            } else {
                openMap();
            }
            break;
        

        case 59: // space
            MainScene::ReadyCallback();
            break;

        default:
            log("keyboard -> %d", op);
            break;
    }
}

void MainScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    int op = (int)keyCode;
    switch (op) {
        case 146:   // W
            Wflag = false;
            break;
            
        case 142:   // S
            Sflag = false;
            break;
            
        case 124:   // A
            Aflag = false;
            break;
            
        case 127:   // D
            Dflag = false;
            break;
            
        case 12:    // LS
            MainScene::SPEED_RATIO = 1;
            break;
            
        default:
            break;
    }
}

// need to learn
void MainScene::setViewPointCenter(Point position) {
    auto winSize = Director::getInstance()->getWinSize();
    
    // middle of the window
    int x = MAX(position.x, winSize.width / 2);
    int y = MAX(position.y, winSize.height / 2);
    x = MIN(x, (MainMap->getMapSize().width * MainMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (MainMap->getMapSize().height * MainMap->getTileSize().height) - winSize.height / 2);
    auto actualPosition = Point(x, y);

    auto centerOfView = Point(winSize.width / 2, winSize.height / 2);
    auto viewPoint = centerOfView - actualPosition;
    auto delta = viewPoint - this->getPosition();
    this->setPosition(viewPoint);
    if(mouse != nullptr)mouse->setPosition(mouse->getPosition() - delta);
    if(fog != nullptr)fog->setPosition(fog->getPosition() - delta);
    if (GoGameItem != nullptr)GoGameItem->setPosition(GoGameItem->getPosition() - delta);
}

void MainScene::show_notice(std::string killevent) {
    if (Notice == nullptr) return;
    Notice->setString(killevent);
}

void MainScene::show_remain(int life_cnt) {
	if (Remain == nullptr) return;
	Remain->setVisible(true);
	std::string tem = std::to_string(life_cnt) + " / 4";
	Remain->setString(tem);
}

void MainScene::show_begin(int status,int ready_person) {
	if (status==-1)
		{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		already->setPosition(player->getPosition().x+visibleSize.width/2-100, player->getPosition().y+visibleSize.height/2-120);
		}
    else if (status == 0) {//not begin
        already->setString(std::to_string(ready_person) + "/ 4");
		already->setVisible(true);
    }
    else {
        already->setVisible(false);
    }
}

const float OK_OPEN_BOX = 32;

void MainScene::CheckBoxes() {
    float MAXDis = 1000000000;
    int finalID = -1;
    for(int i = 0; i < (int)Box_ve.size(); ++i) {
        Box& B = Box_ve[i];
        float w = MainMap->getTileSize().width;
        Vec2 pos = Vec2(B.x * w, B.y * w);
        //log("Box:%d x->%f, y->%f", i, B.x*w, B.y*w);
        float len = (pos - player->getPosition()).length();
        if(len < OK_OPEN_BOX) {
            if (len < MAXDis) {
                MAXDis = len;
                finalID = i;
            }
        }
    }
    if(finalID != -1) {
        OpenBox(finalID);
    }
}

string boxWeapon[NUM_OF_WEAPON] = { "", "box/weaponshow1.png" };
string boxPill[4] = { "box/pill0.png", "box/pill1.png", "box/pill2.png", "box/pill3.png" };
string boxSheild = "shield.png";

void MainScene::OpenBox(int boxID) {
    Box& B = Box_ve[boxID];
    
    log("open Box:%d", boxID);
    glass->setVisible(true);
    isOpenBox = true;
    player->hideStatus();
    fog->setVisible(false);
    
    auto oriPos = this->getPosition();
    this->setPosition(Vec2::ZERO);
    MainMap->setPosition(oriPos);
    Safe_Zone->setVisible(false);
        
    /*
        Menu of things in the box
    */
    
    auto visibleSize = Director::getInstance()->getWinSize();
    
    // creating a Menu from a Vector of items
    Vector<MenuItem*> MenuItems;

    
    if(B.weapon1 != 0) {
        auto tmpItem = MenuItemImage::create(boxWeapon[B.weapon1], "box/blank.png",
                                             [&](Ref* sender){
                                                 log("Pick weapon %d", B.weapon1);
                                                 boxMenu->removeChildByTag(0);
                                                 boxMenu->removeChildByTag(7);
                                             });
        tmpItem->setPosition(-visibleSize.width / 6, visibleSize.height / 4);
        tmpItem->setTag(0);
        auto tmpText = Label::createWithTTF(std::to_string(B.bullet1), "fonts/Marker Felt.ttf", 20);
        tmpItem->addChild(tmpText);
        tmpText->setPosition(tmpItem->getContentSize().width / 4 * 3, tmpItem->getContentSize().height / 4);
        tmpText->setTextColor(Color4B::BLACK);
        MenuItems.pushBack(tmpItem);
    }
    
    if(B.weapon2 != 0) {
        auto tmpItem = MenuItemImage::create(boxWeapon[B.weapon2], "box/blank.png",
                                             [&](Ref* sender){
                                                 log("Pick weapon %d", B.weapon2);
                                                 boxMenu->removeChildByTag(1);
                                                 boxMenu->removeChildByTag(8);
                                             });
        tmpItem->setPosition(-visibleSize.width / 6, visibleSize.height / 4 - visibleSize.height / 8);
        tmpItem->setTag(1);
        auto tmpText = Label::createWithTTF(std::to_string(B.bullet2), "fonts/Marker Felt.ttf", 20);
        tmpItem->addChild(tmpText);
        tmpText->setPosition(tmpItem->getContentSize().width / 4 * 3, tmpItem->getContentSize().height / 4);
        tmpText->setTextColor(Color4B::BLACK);
        MenuItems.pushBack(tmpItem);
    }
    
    for(int i = 0; i < 4; ++i) if(B.pillNum[i] != 0) {
        auto tmpItem = MenuItemImage::create(boxPill[i], "box/blank.png",
                                             [&, i](Ref* sender){
                                                 log("Pick %d pill %d", B.pillNum[i], i);
                                                 boxMenu->removeChildByTag(i + 2);
                                             });
        tmpItem->setPosition(visibleSize.width / 6, visibleSize.height / 4 - i * visibleSize.height / 8);
        tmpItem->setTag(2 + i);
        auto tmpText = Label::createWithTTF(std::to_string(B.pillNum[i]), "fonts/Marker Felt.ttf", 20);
        tmpItem->addChild(tmpText);
        tmpText->setPosition(tmpItem->getContentSize().width / 4 * 3, tmpItem->getContentSize().height / 4);
        tmpText->setTextColor(Color4B::BLACK);
        MenuItems.pushBack(tmpItem);
    }
    
    if(B.shield != 0) {
        auto tmpItem = MenuItemImage::create("box/shield.png", "box/blank.png",
                                             [&](Ref* sender){
                                                 log("Pick shield with val %f", (float)B.shield);
                                                 boxMenu->removeChildByTag(6);
                                             });
        tmpItem->setPosition(-visibleSize.width / 6, visibleSize.height / 4 - 3 * visibleSize.height / 8);
        tmpItem->setTag(6);
        auto tmpText = Label::createWithTTF(std::to_string(B.shield), "fonts/Marker Felt.ttf", 20);
        tmpItem->addChild(tmpText);
        tmpText->setPosition(tmpItem->getContentSize().width / 4 * 3, tmpItem->getContentSize().height / 4);
        tmpText->setTextColor(Color4B::BLACK);
        MenuItems.pushBack(tmpItem);
    }
    
    // bullet1
    if(B.bullet1 != 0) {
        auto tmpItem = MenuItemImage::create("box/bullet.png", "box/littleblank.png",
                                             [&](Ref* sender){
                                                 log("Pick bullet %d", B.bullet1);
                                                 boxMenu->removeChildByTag(7);
                                             });
        tmpItem->setPosition(-visibleSize.width / 3.5, visibleSize.height / 4);
        tmpItem->setTag(7);
        MenuItems.pushBack(tmpItem);
    }
    
    // bullet2
    if(B.bullet2 != 0) {
        auto tmpItem = MenuItemImage::create("box/bullet.png", "box/littleblank.png",
                                             [&](Ref* sender){
                                                 log("Pick bullet %d", B.bullet2);
                                                 boxMenu->removeChildByTag(8);
                                             });
        tmpItem->setPosition(-visibleSize.width / 3.5, visibleSize.height / 4 - visibleSize.height / 8);
        tmpItem->setTag(8);
        MenuItems.pushBack(tmpItem);
    }
    
    /* repeat for as many menu items as needed */
    
    boxMenu = Menu::createWithArray(MenuItems);
    this->addChild(boxMenu, 100);
    
    /*
        End of menu
     */
}

void MainScene::closeBox() {
    glass->setVisible(false);
    isOpenBox = false;
    player->showStatus();
    fog->setVisible(true);
    
    auto oriPos = MainMap->getPosition();
    this->setPosition(oriPos);
    MainMap->setPosition(Vec2::ZERO);
    Safe_Zone->setVisible(true);
    fog->setPosition(player->getPosition());
    
    this->removeChild(boxMenu);
}

void MainScene::openMap() {
    littleMap->setVisible(true);
    littlePoint->setVisible(true);
    isOpenMap = true;
    
    float xrate = player->getPosition().x / (MainMap->getTileSize().width * MainMap->getMapSize().width);
    float yrate = player->getPosition().y / (MainMap->getTileSize().height * MainMap->getMapSize().width);
    
    Vec2 delta = LITTLE_MAP_SIZE * Vec2(xrate - 0.5, yrate - 0.5);
    littlePoint->setPosition(delta + player->getPosition());
	
	littleSafeZone->setVisible(true);
}

void MainScene::closeMap() {
    littleMap->setVisible(false);
    littlePoint->setVisible(false);
    isOpenMap = false;
	littleSafeZone->setVisible(false);
}

void MainScene::setSafeZone(cocos2d::Point new_center, int size) {
	//all the site should be set between 0 and 128
	Safe_Zone->setPosition(new_center);
	Safe_Zone->setScale(SafezoneScaleSize[size]);
	SafezoneScaleSize[0] = SafezoneScaleSize[size];//the first one is the cur status
	littleSafeZone->setScale(1.11*SafezoneScaleSize[0]/9);
	return;
}

void MainScene::ReadyCallback() {
	if (isOnline)return;
    to_ready->setVisible(false);
	BASIC_SPEED = 0;
	direction = Vec2::ZERO;
    extern c_s_msg to_be_sent;
	extern string login_username;
	to_be_sent.type = 0;
	to_be_sent.x = int(player->getPosition().x)/2;
	to_be_sent.y = int(player->getPosition().y)/2;
	#ifndef MAC
	memcpy_s(to_be_sent.remark, 16, login_username.c_str(), login_username.length());
	#else
	memcpy(to_be_sent.remark, login_username.c_str(), login_username.length());
	#endif
	write();
	isOnline = true;
	show_begin(-1, 0);
}


void MainScene::FinalScene(std::string Username, int rank, int kill_num, int ifwinner) {

	DeadLayer->setVisible(true);
	fog->setVisible(false);

	std::string Remind[2] = { "WINNER WINNER,CHICKEN DINNER!","BETTER LUCK FOR NEXT TIME" };

	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto show_User = cocos2d::Label::createWithTTF(Username, "fonts/Marker Felt.ttf", 30);
	show_User->setPosition(player->getPosition().x - visibleSize.width / 2 + 20, player->getPosition().y + visibleSize.height / 2 - 20);
	addChild(show_User);

	auto show_res = cocos2d::Label::createWithTTF(Remind[ifwinner], "fonts/Marker Felt.ttf", 50);
	show_res->setPosition(player->getPosition().x - visibleSize.width / 2 + 20, player->getPosition().y + visibleSize.height / 2 - 50);
	addChild(show_res);
	show_res->setTextColor(Color4B::YELLOW);

	auto show_rank = cocos2d::Label::createWithTTF("Rank: " + std::to_string(rank)+"     Kill "+std::to_string(kill_num)+" player", "fonts/Marker Felt.ttf", 40);
	show_res->setPosition(player->getPosition().x - visibleSize.width / 2 + 20, player->getPosition().y + visibleSize.height / 2 - 100);
	addChild(show_rank);

	//close the windows

}

void MainScene::set_pill(int *pill_now) {
    Medical_cnt->setString(std::to_string(pill_now[0]));
    Firstaid_cnt->setString(std::to_string(pill_now[1]));
    Drink_cnt->setString(std::to_string(pill_now[2]));
    Bandage_cnt->setString(std::to_string(pill_now[3]));
}

void MainScene::try_receive(float dt)
{
	if (!isOnline)return;
	if (socket_closed())
		{
		auto scene = ConnectfailScene::createScene();
		Director::getInstance()->replaceScene(scene);
		return;
		}
	if (!received())return;
	extern s_c_msg s2c;
	if (!isRunning)
		{
		playerID = s2c.infoy;
		if (s2c.type)
			{
			isRunning = true;
			BASIC_SPEED = 2.5;
			for (int i = 0;i<SOLDIER_NUM;++i)
				{
				if (i!=playerID)enemy[i]->setVisible(true);
				enemy[i]->setusername(string(s2c.user_name[i]));
				}
			Remain->setVisible(true);
			}
		show_begin(s2c.type, s2c.infox);
		}
	else if (s2c.type==2||s2c.type==3)
		{
		isOnline = false;
		close_socket();
		FinalScene(string(s2c.user_name[playerID]), s2c.infoy, s2c.infox, s2c.type==3);
		}
	else
		{

		if (s2c.Poison_LEVEL)
			{
			setSafeZone(Vec2(s2c.Poison_X*2, s2c.Poison_Y*2), s2c.Poison_LEVEL);
			}

		player->setHP(s2c.currenthp);
		player->setShield(s2c.Armornaijiu);
		Warning->setVisible(s2c.inpoison);
		Healing->setVisible(s2c.IsCuring);

		set_pill(s2c.PillAmount);
            
        player->setSubWeapon(s2c.SubWeaponType);
		player->setBullet(s2c.MainWeaponCurBullet, s2c.MainWeaponBackupBullet, s2c.SubWeaponCurBullet, s2c.SubWeaponBackupBullet);

		show_remain(s2c.live_count);

		for (int i = 0;i<SOLDIER_NUM;++i)
			{
			if (i==playerID)continue;
			if (enemy[i]->dead())continue;
			enemy[i]->setPosition(s2c.x[i]*2, s2c.y[i]*2);
			enemy[i]->setMainWeapon(s2c.MainWeaponType[i]);
            if (s2c.Firing[i]) {
                enemy[i]->Shoot();
            }
			if (s2c.Isdead[i])
				{
				enemy[i]->setHP(0);
                enemy[i]->isDying();    // show exploit animation
				if (~s2c.BeKilledByPlayerId[i])show_notice(string(s2c.user_name[i])+" was killed by "+string(s2c.user_name[s2c.BeKilledByPlayerId[i]]));
				else show_notice(string(s2c.user_name[i])+" was killed out of safe zone");
				}
			}
        if(s2c.Firing[playerID]) { player->Shoot(); }
		extern c_s_msg to_be_sent;
		to_be_sent.type = 1;
		to_be_sent.x = player->getPosition().x/2;
		to_be_sent.y = player->getPosition().y/2;
		}
	write();
}
