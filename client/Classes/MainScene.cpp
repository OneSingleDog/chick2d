#include "MainScene.h"
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
    
    Box_ve.push_back(Box(64, 64, 1, 1, 60, 0, 1, 1, 1, 1, 100));
    Box_ve.push_back(Box(80, 64, 1, 1, 60, 30,  1, 0, 1, 0, 100));
    Box_ve.push_back(Box(64, 80, 1, 0, 60, 0, 1, 1, 1, 1, 100));

    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    player = new Soldier();
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

    if(player != nullptr && fabs(direction.length()) > 1e-6) {
        bool xflag = false, yflag = false;
        Point ori((player->getPosition() + direction*2.0).x, (player->getPosition() + direction*2.0).y);
        
        if (this->judgePlayerPosition(ori) != 0) {
            if(this->judgePlayerPosition(Point(player->getPosition().x, ori.y)) == 0) {
                yflag = true;
            }
            if(this->judgePlayerPosition(Point(ori.x, player->getPosition().y)) == 0) {
                xflag = true;
            }
        } else {
            xflag = yflag = true;
        }
        if(BASIC_SPEED >= 10) { xflag = yflag = true; }
        Point new_pos((player->getPosition() + direction * SPEED_RATIO).x, (player->getPosition() + direction * SPEED_RATIO).y);
        if(xflag == false) { new_pos.x = player->getPosition().x; }
        if(yflag == false) { new_pos.y = player->getPosition().y; }
        player->setPosition(new_pos.x, new_pos.y);
        if (Notice != nullptr)
        {
            Notice->setPosition(player->getPosition().x + visibleSize.width / 2 - 200, player->getPosition().y + visibleSize.height / 2 - 20);
        }
        //Notice->setPosition(player->getPosition().x + visibleSize.width / 2 - 10, player->getPosition().y + visibleSize.height / 2 - 10);
        setViewPointCenter(player->getPosition());
        to_ready->setPosition(player->getPosition().x, player->getPosition().y + visibleSize.height / 2 - 30);
        if(isOpenSight) {
            closeSight();
        }
        if(isOpenBox) {
            closeBox();
        }
        littleMap->setPosition(player->getPosition());
        
        float xrate = player->getPosition().x / (MainMap->getTileSize().width * MainMap->getMapSize().width);
        float yrate = player->getPosition().y / (MainMap->getTileSize().height * MainMap->getMapSize().width);
        
        Vec2 delta = LITTLE_MAP_SIZE * Vec2(xrate - 0.5, yrate - 0.5);
        littlePoint->setPosition(delta + player->getPosition());
		
		littleSafeZone->setPosition(player->getPosition()+Safe_Zone->getPosition() - Vec2(MainMap->getMapSize().width / 2 * 32, MainMap->getMapSize().height / 2 * 32));

		Medical_kit->setPosition(player->getPosition().x + visibleSize.width / 2 - 85, player->getPosition().y - visibleSize.height / 2 + 200);
		First_aid->setPosition(player->getPosition().x + visibleSize.width / 2 - 85, player->getPosition().y - visibleSize.height / 2 + 135);
		Bandage->setPosition(player->getPosition().x + visibleSize.width / 2 - 85, player->getPosition().y - visibleSize.height / 2 + 85);
		Drink->setPosition(player->getPosition().x + visibleSize.width / 2 - 85, player->getPosition().y - visibleSize.height / 2 + 30);

		Medical_cnt->setPosition(player->getPosition().x + visibleSize.width / 2 - 35, player->getPosition().y - visibleSize.height / 2 + 200);
		Firstaid_cnt->setPosition(player->getPosition().x + visibleSize.width / 2 - 35, player->getPosition().y - visibleSize.height / 2 + 135);
		Bandage_cnt->setPosition(player->getPosition().x + visibleSize.width / 2 - 35, player->getPosition().y - visibleSize.height / 2 + 80);
		Drink_cnt->setPosition(player->getPosition().x + visibleSize.width / 2 - 35, player->getPosition().y - visibleSize.height / 2 + 30);

    }
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
            direction.y += BASIC_SPEED;
            break;
            
        case 142:   // S
            direction.y -= BASIC_SPEED;
            break;
            
        case 124:   // A
            direction.x -= BASIC_SPEED;
            break;
            
        case 127:   // D
            direction.x += BASIC_SPEED;
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
            
        case 139: // P
            MainScene::show_notice("kaikai", "xuanxuan");
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
            direction.y -= BASIC_SPEED;
            break;
            
        case 142:   // S
            direction.y += BASIC_SPEED;
            break;
            
        case 124:   // A
            direction.x += BASIC_SPEED;
            break;
            
        case 127:   // D
            direction.x -= BASIC_SPEED;
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

void MainScene::show_notice(std::string killer, std::string be_killed) {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    if (Notice != nullptr) return;
    Notice = cocos2d::Label::createWithTTF("hhhhhh","fonts/Marker Felt.ttf",30);
    //Notice->setString("xixixi");
    Notice->setString(be_killed + " is killed by " + killer);
    Notice->setTextColor(Color4B::BLACK);
    Notice->setPosition(player->getPosition().x + visibleSize.width / 2-200, player->getPosition().y + visibleSize.height / 2-20);
    addChild(Notice);

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
    this->addChild(boxMenu, 1);
    
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
}

void MainScene::closeMap() {
    littleMap->setVisible(false);
    littlePoint->setVisible(false);
    isOpenMap = false;
}

void MainScene::setSafeZone(cocos2d::Point new_center, int size) {
	//all the site should be set between 0 and 128
	Safe_Zone->setPosition(new_center * 32);
	Safe_Zone->setScale(SafezoneScaleSize[size]);
	SafezoneScaleSize[0] = SafezoneScaleSize[size];//the first one is the cur status
	return;
}

void MainScene::ReadyCallback() {
    to_ready->setVisible(false);
	BASIC_SPEED = 0;
	direction = Vec2::ZERO;
    extern c_s_msg to_be_sent;
	extern string login_username;
	to_be_sent.type = 0;
	to_be_sent.x = int(player->getPosition().x)/32;
	to_be_sent.y = int(player->getPosition().y)/32;
	#ifndef MAC
	memcpy_s(to_be_sent.remark, 16, login_username.c_str(), login_username.length());
	#else
	memcpy(to_be_sent.remark, login_username.c_str(), login_username.length());
	#endif
	write();
	isOnline = true;
	show_begin(-1, 0);
}
