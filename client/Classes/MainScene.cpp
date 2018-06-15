#include "MainScene.h"
#include "ConnectfailScene.h"
#include "SimpleAudioEngine.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <cstring>
#include <ctime>
#include <cstdlib>

#include "post.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

double MainScene::BASIC_SPEED = 2.5;
int MainScene::SPEED_RATIO = 1;  // 1 if not running

int cnt_666 = 0;

float MainScene::SafezoneScaleSize[4] = { 9,4.5,2.25,1.125 };

Scene* MainScene::createScene() {
    return MainScene::create();
}

const string MainScene::PillName[4] = { "others/medical_kit.png", "others/first_aid.png", "others/enr_drink.png", "others/bandage.png" };

bool MainScene::init()
{
	wait_time = 0;
    totBoxNum = 0;
	totAirdrop = 0;
    playerID = 0;
	OpenBoxID = -1;
    isOpenSight = false;
    isOpenBox = false;
    isOpenMap = false;

	isOnline = false;
	isRunning = false;
	isEnd = false;
    
    if ( !Scene::init() )
    {
        return false;
    }
    
    player = new Soldier(0);
	for (int i = 0;i<SOLDIER_NUM;++i)enemy[i] = new Soldier(1);

    // create Main Map using Map.tmx
    MainMap = TMXTiledMap::create("map/Map.tmx");
    
    //Get background layer
    collidable = MainMap->getLayer("Collidable");
    collidable->setVisible(false);
    
    srand(time(NULL));

	//random initial player position
	int xx;
	int yy;
	while (true)
		{
		xx = rand()%((MAP_RBOUND-MAP_LBOUND)*TILESIZE)+MAP_LBOUND*TILESIZE+TILESIZE/2;
		yy = rand()%((MAP_UBOUND-MAP_DBOUND)*TILESIZE)+MAP_DBOUND*TILESIZE+TILESIZE/2;;
		Point tileCoord = this->tileCoordFromPosition(Vec2(xx-TILESIZE/2,yy-TILESIZE/2));
		int tileGid = collidable->getTileGIDAt(tileCoord);// contervt to tiled site

		if (tileGid > 0)
			{ //in the collidable layer
			Value prop = MainMap->getPropertiesForGID(tileGid);
			ValueMap propValueMap = prop.asValueMap();

			std::string collision = propValueMap["collidable"].asString();

			if (collision=="true")continue;
			}

		tileCoord = this->tileCoordFromPosition(Vec2(xx-TILESIZE/2, yy+TILESIZE/2));
		tileGid = collidable->getTileGIDAt(tileCoord);// contervt to tiled site

		if (tileGid > 0)
			{ //in the collidable layer
			Value prop = MainMap->getPropertiesForGID(tileGid);
			ValueMap propValueMap = prop.asValueMap();

			std::string collision = propValueMap["collidable"].asString();

			if (collision=="true")continue;
			}

		tileCoord = this->tileCoordFromPosition(Vec2(xx+TILESIZE/2, yy-TILESIZE/2));
		tileGid = collidable->getTileGIDAt(tileCoord);// contervt to tiled site

		if (tileGid > 0)
			{ //in the collidable layer
			Value prop = MainMap->getPropertiesForGID(tileGid);
			ValueMap propValueMap = prop.asValueMap();

			std::string collision = propValueMap["collidable"].asString();

			if (collision=="true")continue;
			}

		tileCoord = this->tileCoordFromPosition(Vec2(xx+TILESIZE/2, yy+TILESIZE/2));
		tileGid = collidable->getTileGIDAt(tileCoord);// contervt to tiled site

		if (tileGid > 0)
			{ //in the collidable layer
			Value prop = MainMap->getPropertiesForGID(tileGid);
			ValueMap propValueMap = prop.asValueMap();

			std::string collision = propValueMap["collidable"].asString();

			if (collision=="true")continue;
			}
		break;
		}

    // create player and set position
    player->create();
    player->setPosition(xx, yy);
    
	extern string login_username;
	player->setusername(login_username);
	#ifdef _DEBUG
	log("%s", login_username.c_str());
	#endif

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
    addChild(mouse, 20);
    
    // fog create
    fog = Sprite::create("others/fog.png");
    fog->setPosition(player->getPosition());
    fog->setScale(2);
    addChild(fog, -1);
    
    // sight create
    sight = Sprite::create("others/sight.png");
    sight->setPosition(player->getPosition());
    addChild(sight, 10);
    sight->setVisible(false);
   
	glass = Sprite::create("others/glass.png");
	addChild(glass, 40);
	glass->setPosition(visibleSize/2);
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
	to_ready->setPosition(player->getPosition().x, player->getPosition().y+visibleSize.height/2-30);
	addChild(to_ready);

	already = cocos2d::Label::createWithTTF(" ", "fonts/Marker Felt.ttf", 40);
	already->setTextColor(Color4B::BLACK);
	//already->setString(std::to_string(ready_person) + "/ 4");
	already->setPosition(Vec2::ZERO);
	addChild(already);
	already->setVisible(false);

	Safe_Zone = cocos2d::Sprite::create("others/poison_range.png");
	Safe_Zone->setPosition(MainMap->getMapSize().width/2*32, MainMap->getMapSize().height/2*32);
	//Safe_Zone->setPosition(MainMap->getMapSize().width / 2 * 32-100, MainMap->getMapSize().height / 2 * 32-100);
	Safe_Zone->setScale(9);
	//Safe_Zone->setScale(4.5);
	addChild(Safe_Zone, -4);

	littleSafeZone = Sprite::create("others/poison_range.png");
	addChild(littleSafeZone, 30);
	littleSafeZone->setPosition(player->getPosition());
	littleSafeZone->setScale(1.11);
	littleSafeZone->setVisible(false);

	Medical_kit = Sprite::create(PillName[0]);
	addChild(Medical_kit);
	Medical_kit->setScale(0.3);
	Medical_kit->setPosition(player->getPosition().x+visibleSize.width/2-85, player->getPosition().y-visibleSize.height/2+200);

	First_aid = Sprite::create(PillName[1]);
	addChild(First_aid);
	First_aid->setScale(0.6);
	First_aid->setPosition(player->getPosition().x+visibleSize.width/2-85, player->getPosition().y-visibleSize.height/2+135);

	Drink = Sprite::create(PillName[2]);
	addChild(Drink);
	Drink->setScale(0.25);
	Drink->setPosition(player->getPosition().x+visibleSize.width/2-85, player->getPosition().y-visibleSize.height/2+85);

	Bandage = Sprite::create(PillName[3]);
	addChild(Bandage);
	Bandage->setScale(0.23);
	Bandage->setPosition(player->getPosition().x+visibleSize.width/2-85, player->getPosition().y-visibleSize.height/2+30);

	Medical_cnt = cocos2d::Label::createWithTTF("0", "fonts/Marker Felt.ttf", 30);
	Medical_cnt->setTextColor(Color4B::BLACK);
	Medical_cnt->setPosition(player->getPosition().x+visibleSize.width/2-35, player->getPosition().y-visibleSize.height/2+200);
	addChild(Medical_cnt);

	Firstaid_cnt = cocos2d::Label::createWithTTF("0", "fonts/Marker Felt.ttf", 30);
	Firstaid_cnt->setTextColor(Color4B::BLACK);
	Firstaid_cnt->setPosition(player->getPosition().x+visibleSize.width/2-35, player->getPosition().y-visibleSize.height/2+135);
	addChild(Firstaid_cnt);

	Bandage_cnt = cocos2d::Label::createWithTTF("0", "fonts/Marker Felt.ttf", 30);
	Bandage_cnt->setTextColor(Color4B::BLACK);
	Bandage_cnt->setPosition(player->getPosition().x+visibleSize.width/2-35, player->getPosition().y-visibleSize.height/2+30);
	addChild(Bandage_cnt);

	Drink_cnt = cocos2d::Label::createWithTTF("0", "fonts/Marker Felt.ttf", 30);
	Drink_cnt->setTextColor(Color4B::BLACK);
	Drink_cnt->setPosition(player->getPosition().x+visibleSize.width/2-35, player->getPosition().y-visibleSize.height/2+80);
	addChild(Drink_cnt);

	DeadLayer = cocos2d::Sprite::create("others/glass.png");
	DeadLayer->setPosition(player->getPosition());
	addChild(DeadLayer, 40);
	DeadLayer->setVisible(false);

	Warning = cocos2d::Sprite::create("others/warn.png");
	Warning->setPosition(player->getPosition().x-250, player->getPosition().y-visibleSize.height/2+30);
	addChild(Warning);
	Warning->setScale(0.15);
	Warning->setVisible(false);

	Healing = cocos2d::Sprite::create("others/heal.png");
	Healing->setPosition(Warning->getPosition()+Vec2(0, visibleSize.height/16));
	addChild(Healing);
	Healing->setScale(0.15);
	Healing->setVisible(false);

	Notice = cocos2d::Label::createWithTTF("", "fonts/Marker Felt.ttf", 30);
	Notice->setTextColor(Color4B::BLACK);
	Notice->setAnchorPoint(Vec2(1, 1));
	Notice->setPosition(player->getPosition().x+visibleSize.width/2-30, player->getPosition().y+visibleSize.height/2-100);
	addChild(Notice);

	Remain = cocos2d::Label::createWithTTF("", "fonts/Marker Felt.ttf", 50);
	Remain->setTextColor(Color4B::ORANGE);
	Remain->setPosition(player->getPosition().x+visibleSize.width/2-100, player->getPosition().y+visibleSize.height/2-40);
	addChild(Remain);
	Remain->setVisible(false);

	Ping_time = cocos2d::Label::createWithTTF("", "fonts/Marker Felt.ttf", 20);
	Ping_time->setTextColor(Color4B::BLACK);
	Ping_time->setPosition(player->getPosition().x-visibleSize.width/2+50, player->getPosition().y+visibleSize.height/2-20);
	addChild(Ping_time);

	//Mouse Event

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
		if (player->dead())return;
        EventMouse *e = (EventMouse*)event;
        int op = (int)e->getMouseButton();
        // 0 -> left button
        // 1 -> right button
        if(op == 1) {
            if(SPEED_RATIO != 1) { return; }
            if(isOpenBox) { return; }
            if(!player->ableToOpenSight()) { return; }
            if(!isOpenSight) {
                openSight();
            } else {
                closeSight();
            }
        } else {
            if(SPEED_RATIO != 1) { return; }
			if (isOpenBox) { return; }

			float angle = player->getRotation();
			angle = 360-angle;
			if (angle>360) { angle -= 360; }
			angle = angle/180*acos(-1.0);

			extern c_s_msg to_be_sent;
			to_be_sent.ShootAngle = angle;
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
    
    schedule(schedule_selector(MainScene::myMoveAction), (float)(1.0 / 60), kRepeatForever, 0);
	schedule(schedule_selector(MainScene::try_receive), (float)(1.0/60), kRepeatForever, 0);
    //end of schedule

    Wflag = Aflag = Sflag = Dflag = false;
    
    return true;
}

void MainScene::openSight() {
    if(isOpenMap) {
        closeMap();
    }
    player->hideStatus();
	player->show_circle();
    
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

	Medical_kit->setVisible(false);
	Medical_cnt->setVisible(false);
	First_aid->setVisible(false);
	Firstaid_cnt->setVisible(false);
	Drink->setVisible(false);
	Drink_cnt->setVisible(false);
	Bandage->setVisible(false);
	Bandage_cnt->setVisible(false);

	Warning->setVisible(false);
	Healing->setVisible(false);
	Remain->setVisible(false);
	Notice->setVisible(false);
    Ping_time->setVisible(false);

    isOpenSight = true;
}

void MainScene::closeSight() {
    player->showStatus();
    
    setViewPointCenter(player->getPosition());
    fog->setPosition(player->getPosition());
    sight->setVisible(false);
    fog->setVisible(true);
    isOpenSight = false;

	Medical_kit->setVisible(true);
	Medical_cnt->setVisible(true);
	First_aid->setVisible(true);
	Firstaid_cnt->setVisible(true);
	Drink->setVisible(true);
	Drink_cnt->setVisible(true);
	Bandage->setVisible(true);
	Bandage_cnt->setVisible(true);

	Remain->setVisible(true);
	Notice->setVisible(true);
    Ping_time->setVisible(true);
}

void MainScene::setMousePosition(cocos2d::Event *event) {
    EventMouse *e = (EventMouse*)event;
    auto viewpos = this->getPosition();
    float x = e->getCursorX() - viewpos.x;
    float y = e->getCursorY() - viewpos.y;
    mouse->setPosition(x, y);
}

void MainScene::myMoveAction(float dt) {

    direction = Vec2::ZERO;
    if(Wflag) { direction.y += BASIC_SPEED; }
    if(Aflag) { direction.x -= BASIC_SPEED; }
    if(Sflag) { direction.y -= BASIC_SPEED; }
    if(Dflag) { direction.x += BASIC_SPEED; }
	if (player!=nullptr && fabs(direction.length())>1e-6) {
		extern c_s_msg to_be_sent;
		to_be_sent.curetype = 0;
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

        to_ready->setPosition(player->getPosition().x, player->getPosition().y + visibleSize.height / 2 - 30);
        littleMap->setPosition(player->getPosition());
        
        float xrate = player->getPosition().x / (MainMap->getTileSize().width * MainMap->getMapSize().width);
        float yrate = player->getPosition().y / (MainMap->getTileSize().height * MainMap->getMapSize().width);
        
        Vec2 delta = LITTLE_MAP_SIZE * Vec2(xrate - 0.5, yrate - 0.5);
        littlePoint->setPosition(delta + player->getPosition());

		for(int i=0;i<totAirdrop;++i)
			littleAirdrop[i]->setPosition(littleMap->getPosition()+(Box_ve[AirdropNum[i]].getPosition()-MainMap->getMapSize()*16)/8);
        
        Medical_kit->setPosition(player->getPosition().x + visibleSize.width / 2 - 85, player->getPosition().y - visibleSize.height / 2 + 200);
        First_aid->setPosition(player->getPosition().x + visibleSize.width / 2 - 85, player->getPosition().y - visibleSize.height / 2 + 135);
        Bandage->setPosition(player->getPosition().x + visibleSize.width / 2 - 85, player->getPosition().y - visibleSize.height / 2 + 30);
        Drink->setPosition(player->getPosition().x + visibleSize.width / 2 - 85, player->getPosition().y - visibleSize.height / 2 + 85);
        
        Medical_cnt->setPosition(player->getPosition().x + visibleSize.width / 2 - 35, player->getPosition().y - visibleSize.height / 2 + 200);
        Firstaid_cnt->setPosition(player->getPosition().x + visibleSize.width / 2 - 35, player->getPosition().y - visibleSize.height / 2 + 135);
        Bandage_cnt->setPosition(player->getPosition().x + visibleSize.width / 2 - 35, player->getPosition().y - visibleSize.height / 2 + 30);
        Drink_cnt->setPosition(player->getPosition().x + visibleSize.width / 2 - 35, player->getPosition().y - visibleSize.height / 2 + 80);
		
		Notice->setPosition(player->getPosition().x + visibleSize.width / 2 - 200, player->getPosition().y + visibleSize.height / 2 - 80);
		Remain->setPosition(player->getPosition().x + visibleSize.width / 2 - 100, player->getPosition().y + visibleSize.height / 2 - 40);

 		Warning->setPosition(player->getPosition().x - 250, player->getPosition().y - visibleSize.height / 2 + 30);    
        Healing->setPosition(Warning->getPosition() + Vec2(0, visibleSize.height / 16));

		Ping_time->setPosition(player->getPosition().x-visibleSize.width/2+50, player->getPosition().y+visibleSize.height/2-20);

		if (Notice!=nullptr)
			{
			Notice->setPosition(player->getPosition().x+visibleSize.width/2-30, player->getPosition().y+visibleSize.height/2-100);
			}
    }
    
    littleSafeZone->setPosition(littleMap->getPosition()+(Safe_Zone->getPosition()-MainMap->getMapSize()*16)/8);
    
    // fog of war
        float angle = player->getRotation();
    angle = 360 - angle;
    if(angle > 360) {
        angle -= 360;
    }
    angle = angle / 180 * acos(-1.0);
    float A = cos(angle), B = sin(angle), C = 150 - cos(angle) * player->getPosition().x - sin(angle) * player->getPosition().y;
    for(int i = 0; i < SOLDIER_NUM; ++i) {
        if(i == playerID || !isRunning) {
            enemy[i]->setVisible(false);
        } else if(!enemy[i]->dead()) {
            if(cos(angle) * (enemy[i]->getPosition().x - player->getPosition().x) + sin(angle) * (enemy[i]->getPosition().y - player->getPosition().y) +150 < 0) {   // outside
                enemy[i]->setVisible(false);
            } else {
                enemy[i]->setVisible(true);
            }
        }
    }
}

// 0 -> can move
// 1 -> cannot move
int MainScene::judgePlayerPosition(Point position) {
	static const int judgesize = 14;
    Point tileCoord = this->tileCoordFromPosition(position+Vec2(judgesize, judgesize));
    int tileGid = collidable->getTileGIDAt(tileCoord);// contervt to tiled site
    
    if (tileGid > 0) { //in the collidable layer
        Value prop = MainMap->getPropertiesForGID(tileGid);
        ValueMap propValueMap = prop.asValueMap();
        
        std::string collision = propValueMap["collidable"].asString();
        
        if (collision == "true") {
            return 1;
        }
    }

	tileCoord = this->tileCoordFromPosition(position+Vec2(-judgesize, judgesize));
	tileGid = collidable->getTileGIDAt(tileCoord);// contervt to tiled site

	if (tileGid > 0)
		{ //in the collidable layer
		Value prop = MainMap->getPropertiesForGID(tileGid);
		ValueMap propValueMap = prop.asValueMap();

		std::string collision = propValueMap["collidable"].asString();

		if (collision=="true")
			{
			return 1;
			}
		}

	tileCoord = this->tileCoordFromPosition(position+Vec2(judgesize, -judgesize));
	tileGid = collidable->getTileGIDAt(tileCoord);// contervt to tiled site

	if (tileGid > 0)
		{ //in the collidable layer
		Value prop = MainMap->getPropertiesForGID(tileGid);
		ValueMap propValueMap = prop.asValueMap();

		std::string collision = propValueMap["collidable"].asString();

		if (collision=="true")
			{
			return 1;
			}
		}

	tileCoord = this->tileCoordFromPosition(position+Vec2(-judgesize, -judgesize));
	tileGid = collidable->getTileGIDAt(tileCoord);// contervt to tiled site

	if (tileGid > 0)
		{ //in the collidable layer
		Value prop = MainMap->getPropertiesForGID(tileGid);
		ValueMap propValueMap = prop.asValueMap();

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
	int y = ((MainMap->getMapSize().height * MainMap->getTileSize().height)-pos.y)/MainMap->getTileSize().height;
    
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
	extern c_s_msg to_be_sent;
    switch (op) {
        case 146:   // W
			if (player->dead())break;
            Wflag = true;
			to_be_sent.curetype = -1;
            break;
            
        case 142:   // S
			if (player->dead())break;
            Sflag = true;
			to_be_sent.curetype = -1;
            break;
            
        case 124:   // A
			if (player->dead())break;
            Aflag = true;
			to_be_sent.curetype = -1;
            break;
            
        case 127:   // D
			if (player->dead())break;
            Dflag = true;
			to_be_sent.curetype = -1;
            break;
            
        case 140:   // Q
			if (player->dead())break;
			if(!isRunning) { break; }
            if(SPEED_RATIO != 1) { break; }
            if(isOpenSight) {
                closeSight();
            }
            player->changeWeapon();
			to_be_sent.curetype = 0;
			to_be_sent.Load = 0;
			to_be_sent.Exchange ^= 1;
            break;
            
        case 141:   // R
			if (player->dead())break;
			if (!isRunning) { break; }
            if(SPEED_RATIO != 1) { break; }
			to_be_sent.curetype = 0;
			to_be_sent.Load = 1;
            player->changeBullet();
            break;
            
        case 12:    // LS
			if (player->dead())break;
            MainScene::SPEED_RATIO = 2;
            break;
            
        //case 134:   // K
        //    player->getHurted(20);
        //    break;
            
        //case 135:   // L
        //    player->increaseShieldVal(20);
        //    break;
            
        case 82:    // 6
			if (player->dead())break;
            ++cnt_666;
            if(cnt_666 >= 3) {
                BASIC_SPEED = 20;
            }
            break;
            
        case 129:   // F
			if (player->dead())break;
			if (!isRunning) { break; }
            if(isOpenBox) {
                closeBox();
            } else {
                CheckBoxes();
            }
            
            break;

        case 136:   // M
			if (player->dead())break;
			if (isOpenSight||isOpenBox)return;
            if(isOpenMap) {
                closeMap();
            } else {
                openMap();
            }
            break;
        
		case 77:    // 1
			if (player->dead())break;
			if (!isRunning) { break; }
			if (Wflag||Aflag||Sflag||Dflag)break;
			to_be_sent.curetype = 1;
			break;

		case 78:    // 2
			if (player->dead())break;
			if (!isRunning) { break; }
			if (Wflag||Aflag||Sflag||Dflag)break;
			to_be_sent.curetype = 2;
			break;

		case 79:    // 3
			if (player->dead())break;
			if (!isRunning) { break; }
			if (Wflag||Aflag||Sflag||Dflag)break;
			to_be_sent.curetype = 3;
			break;

		case 80:    // 4
			if (player->dead())break;
			if (!isRunning) { break; }
			if (Wflag||Aflag||Sflag||Dflag)break;
			to_be_sent.curetype = 4;
			break;

        case 59: // space
			if (player->dead())break;
            MainScene::ReadyCallback();
            break;

        default:
			#ifdef _DEBUG
            log("keyboard -> %d", op);
			#endif
            break;
    }
}

void MainScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
    int op = (int)keyCode;
    switch (op) {
        case 146:   // W
			if (player->dead())break;
            Wflag = false;
            break;
            
        case 142:   // S
			if (player->dead())break;
            Sflag = false;
            break;
            
        case 124:   // A
			if (player->dead())break;
            Aflag = false;
            break;
            
        case 127:   // D
			if (player->dead())break;
            Dflag = false;
            break;
            
        case 12:    // LS
			if (player->dead())break;
            MainScene::SPEED_RATIO = 1;
            break;
            
        default:
            break;
    }
}

void MainScene::setViewPointCenter(Point position) {
    // middle of the window
    int x = MAX(position.x, visibleSize.width / 2);
    int y = MAX(position.y, visibleSize.height / 2);
	x = MIN(x, (MainMap->getMapSize().width * MainMap->getTileSize().width)-visibleSize.width/2);
	y = MIN(y, (MainMap->getMapSize().height * MainMap->getTileSize().height)-visibleSize.height/2);
    auto actualPosition = Point(x, y);

    auto centerOfView = Point(visibleSize.width / 2, visibleSize.height / 2);
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
	std::string tem = std::to_string(life_cnt)+" / "+std::to_string(max_player);
    Remain->setString(tem);
    
    if(life_cnt <= 2 && !start_fight) { // start playing "fight.mp3"
        start_fight = true;
        auto audio = SimpleAudioEngine::getInstance();
        audio->playBackgroundMusic("music/fight.mp3");
    }
}

void MainScene::show_begin(int status,int ready_person) {
	if (status==-1)
		{
		already->setPosition(player->getPosition().x+visibleSize.width/2-100, player->getPosition().y+visibleSize.height/2-120);
		}
    else if (status == 0) {//not begin
		already->setString(std::to_string(ready_person)+"/ "+std::to_string(max_player));
		already->setVisible(true);
    }
    else {
        already->setVisible(false);
    }
}

void MainScene::CheckBoxes() {
    float MAXDis = 1000000000;
    int finalID = -1;
    for(int i = 0; i < (int)Box_ve.size(); ++i) {
        Box& B = Box_ve[i];
		Vec2 pos = Vec2(B.x, B.y);
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

void MainScene::OpenBox(int boxID) {
    if(isOpenSight) {
        closeSight();
    }
    
	#ifdef _DEBUG
    log("open Box:%d", boxID);
	#endif

    glass->setVisible(true);
    isOpenBox = true;
	OpenBoxID = boxID;
    player->hideStatus();
    fog->setVisible(false);
    
    auto oriPos = this->getPosition();
    this->setPosition(Vec2::ZERO);
    MainMap->setPosition(oriPos);
    Safe_Zone->setVisible(false);

	Notice->setVisible(false);
	to_ready->setVisible(false);
	already->setVisible(false);
	Medical_cnt->setVisible(false);
	Medical_kit->setVisible(false);
	Firstaid_cnt->setVisible(false);
	First_aid->setVisible(false);
	Bandage->setVisible(false);
	Bandage_cnt->setVisible(false);
	Drink->setVisible(false);
	Drink_cnt->setVisible(false);
	Remain->setVisible(false);
	Ping_time->setVisible(false);
	Healing->setVisible(false);
	Warning->setVisible(false);

	for (int i = 0;i<totBoxNum;++i)
		Box_pic[i]->setVisible(false);

	for (int i = 0;i<max_player;++i)
		enemy[i]->setVisible(false);

	update_box();
}

void MainScene::update_box(){
	if (!isOpenBox)return;
	if (OpenBoxID==-1)return;

	Box& B = Box_ve[OpenBoxID];

	if (boxMenu!=nullptr)
		{
		this->removeChild(boxMenu);
		}
	/*
	Menu of things in the box
	*/

	// creating a Menu from a Vector of items
	Vector<MenuItem*> MenuItems;

	extern c_s_msg to_be_sent;

	if (B.weapon1 != -1)
		{
		auto tmpItem = MenuItemImage::create(boxWeapon[B.weapon1], "box/blank.png",
											 [&](Ref* sender){
			log("Pick weapon %d", B.weapon1);
			if (~to_be_sent.BoxId&&to_be_sent.BoxId!=OpenBoxID)return;
			to_be_sent.BoxId = OpenBoxID;
			to_be_sent.PickWp1 = true;
			});
		tmpItem->setPosition(-visibleSize.width/6, visibleSize.height/4);
		tmpItem->setTag(0);
		auto tmpText = Label::createWithTTF(std::to_string(B.bullet1), "fonts/Marker Felt.ttf", 20);
		tmpItem->addChild(tmpText);
		tmpText->setPosition(tmpItem->getContentSize().width/4*3, tmpItem->getContentSize().height/4);
		tmpText->setTextColor(Color4B::BLACK);
		MenuItems.pushBack(tmpItem);
		}

	if (B.weapon2 != -1)
		{
		auto tmpItem = MenuItemImage::create(boxWeapon[B.weapon2], "box/blank.png",
											 [&](Ref* sender){
			log("Pick weapon %d", B.weapon2);
			if (~to_be_sent.BoxId&&to_be_sent.BoxId!=OpenBoxID)return;
			to_be_sent.BoxId = OpenBoxID;
			to_be_sent.PickWp2 = true;
			});
		tmpItem->setPosition(-visibleSize.width/6, visibleSize.height/4-visibleSize.height/8);
		tmpItem->setTag(1);
		auto tmpText = Label::createWithTTF(std::to_string(B.bullet2), "fonts/Marker Felt.ttf", 20);
		tmpItem->addChild(tmpText);
		tmpText->setPosition(tmpItem->getContentSize().width/4*3, tmpItem->getContentSize().height/4);
		tmpText->setTextColor(Color4B::BLACK);
		MenuItems.pushBack(tmpItem);
		}

	for (int i = 0; i < 4; ++i) if (B.pillNum[i]!=0)
		{
		auto tmpItem = MenuItemImage::create(boxPill[i], "box/blank.png",
											 [&, i](Ref* sender){
			log("Pick %d pill %d", B.pillNum[i], i);
			if (~to_be_sent.BoxId&&to_be_sent.BoxId!=OpenBoxID)return;
			to_be_sent.BoxId = OpenBoxID;
			to_be_sent.PickPill[i] = true;
			});
		tmpItem->setPosition(visibleSize.width/6, visibleSize.height/4-i*visibleSize.height/8);
		tmpItem->setTag(2+i);
		auto tmpText = Label::createWithTTF(std::to_string(B.pillNum[i]), "fonts/Marker Felt.ttf", 20);
		tmpItem->addChild(tmpText);
		tmpText->setPosition(tmpItem->getContentSize().width/4*3, tmpItem->getContentSize().height/4);
		tmpText->setTextColor(Color4B::BLACK);
		MenuItems.pushBack(tmpItem);
		}

	if (B.shield!=0)
		{
		auto tmpItem = MenuItemImage::create("box/shield.png", "box/blank.png",
											 [&](Ref* sender){
			log("Pick shield with val %f", (float)B.shield);
			if (~to_be_sent.BoxId&&to_be_sent.BoxId!=OpenBoxID)return;
			to_be_sent.BoxId = OpenBoxID;
			to_be_sent.PickArmor = true;
			});
		tmpItem->setPosition(-visibleSize.width/6, visibleSize.height/4-3*visibleSize.height/8);
		tmpItem->setTag(6);
		auto tmpText = Label::createWithTTF(std::to_string(B.shield), "fonts/Marker Felt.ttf", 20);
		tmpItem->addChild(tmpText);
		tmpText->setPosition(tmpItem->getContentSize().width/4*3, tmpItem->getContentSize().height/4);
		tmpText->setTextColor(Color4B::BLACK);
		MenuItems.pushBack(tmpItem);
		}

	// bullet1
	if (B.bullet1!=0)
		{
		auto tmpItem = MenuItemImage::create("box/bullet.png", "box/littleblank.png",
											 [&](Ref* sender){
			log("Pick bullet %d", B.bullet1);
			if (~to_be_sent.BoxId&&to_be_sent.BoxId!=OpenBoxID)return;
			to_be_sent.BoxId = OpenBoxID;
			to_be_sent.PickBl1 = true;
			});
		tmpItem->setPosition(-visibleSize.width/3.5, visibleSize.height/4);
		tmpItem->setTag(7);
		MenuItems.pushBack(tmpItem);
		}

	// bullet2
	if (B.bullet2!=0)
		{
		auto tmpItem = MenuItemImage::create("box/bullet.png", "box/littleblank.png",
											 [&](Ref* sender){
			log("Pick bullet %d", B.bullet2);
			if (~to_be_sent.BoxId&&to_be_sent.BoxId!=OpenBoxID)return;
			to_be_sent.BoxId = OpenBoxID;
			to_be_sent.PickBl2 = true;
			});
		tmpItem->setPosition(-visibleSize.width/3.5, visibleSize.height/4-visibleSize.height/8);
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
	OpenBoxID = -1;
    player->showStatus();
    fog->setVisible(true);
    
    auto oriPos = MainMap->getPosition();
    this->setPosition(oriPos);
    MainMap->setPosition(Vec2::ZERO);
    Safe_Zone->setVisible(true);
    fog->setPosition(player->getPosition());
    
    this->removeChild(boxMenu);
	boxMenu = nullptr;

	Notice->setVisible(true);
	Medical_cnt->setVisible(true);
	Medical_kit->setVisible(true);
	Firstaid_cnt->setVisible(true);
	First_aid->setVisible(true);
	Bandage->setVisible(true);
	Bandage_cnt->setVisible(true);
	Drink->setVisible(true);
	Drink_cnt->setVisible(true);
	Remain->setVisible(true);
	Ping_time->setVisible(true);

	for (int i = 0;i<totBoxNum;++i)
		Box_pic[i]->setVisible(true);

	for (int i = 0;i<max_player;++i)
		if(i!=playerID&&!enemy[i]->dead())
			enemy[i]->setVisible(true);

}

void MainScene::openMap() {
    littleMap->setVisible(true);
    littlePoint->setVisible(true);
	for (int i = 0;i<totAirdrop;++i)
		littleAirdrop[i]->setVisible(true);
    isOpenMap = true;
    
    float xrate = player->getPosition().x / (MainMap->getTileSize().width * MainMap->getMapSize().width);
    float yrate = player->getPosition().y / (MainMap->getTileSize().height * MainMap->getMapSize().width);
    
    Vec2 delta = LITTLE_MAP_SIZE * Vec2(xrate - 0.5, yrate - 0.5);
    littlePoint->setPosition(delta + player->getPosition());
	for (int i = 0;i<totAirdrop;++i)
		littleAirdrop[i]->setPosition(littleMap->getPosition()+(Box_ve[AirdropNum[i]].getPosition()-MainMap->getMapSize()*16)/8);
	
	littleSafeZone->setVisible(true);
}

void MainScene::closeMap() {
    littleMap->setVisible(false);
    littlePoint->setVisible(false);
	for (int i = 0;i<totAirdrop;++i)
		littleAirdrop[i]->setVisible(false);
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
	if (isEnd)return;
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
	wait_time = clock();
	show_begin(-1, 0);
}


void MainScene::FinalScene(std::string Username, int rank, int kill_num, int ifwinner) {
    // Audio
    auto audio = SimpleAudioEngine::getInstance();
    audio->stopBackgroundMusic();
    if(rank == 1) {
        audio->playBackgroundMusic("music/winner.mp3");
    } else {
        audio->playBackgroundMusic("music/losser.mp3");
    }

	//close the windows
	auto oriPos = this->getPosition();
	this->setPosition(Vec2::ZERO);
	MainMap->setPosition(oriPos);
	Safe_Zone->setVisible(false);
	player->setVisible(false);
	Remain->setVisible(false);
	Notice->setVisible(false);
	Ping_time->setVisible(false);
	Safe_Zone->setVisible(false);

	to_ready->setVisible(false);
	already->setVisible(false);
	Medical_cnt->setVisible(false);
	Medical_kit->setVisible(false);
	Firstaid_cnt->setVisible(false);
	First_aid->setVisible(false);
	Bandage->setVisible(false);
	Bandage_cnt->setVisible(false);
	Drink->setVisible(false);
	Drink_cnt->setVisible(false);
	Healing->setVisible(false);
	Warning->setVisible(false);


	DeadLayer->setVisible(true);
	fog->setVisible(false);

	std::string Remind[2] = {"WINNER WINNER,CHICKEN DINNER!","BETTER LUCK FOR NEXT TIME"};

	auto show_User = cocos2d::Label::createWithTTF(Username, "fonts/Marker Felt.ttf", 30);
	show_User->setAnchorPoint(Vec2::ZERO);
	show_User->setPosition(60, visibleSize.height-60);
	addChild(show_User, 45);

	auto show_res = cocos2d::Label::createWithTTF(Remind[ifwinner], "fonts/Marker Felt.ttf", 50);
	show_res->setAnchorPoint(Vec2::ZERO);
	show_res->setPosition(60, visibleSize.height-140);
	addChild(show_res, 45);
	show_res->setTextColor(Color4B::YELLOW);

	auto show_rank = cocos2d::Label::createWithTTF("Rank: "+std::to_string(rank)+"     Kill "+std::to_string(kill_num)+" player", "fonts/Marker Felt.ttf", 40);
	show_rank->setAnchorPoint(Vec2::ZERO);
	show_rank->setPosition(60, visibleSize.height-200);
	addChild(show_rank, 45);

	DeadLayer->setPosition(visibleSize/2);

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(MainScene::menuCloseCallback, this));
	float x = 0;
	float y = -200;
	closeItem->setPosition(cocos2d::Vec2(x, y));
	closeItem->setScaleX(1.5);
	closeItem->setScaleY(1.5);
	//addChild(closeItem,45);

	auto finalMenu = Menu::create(closeItem, NULL);
	this->addChild(finalMenu, 45);
}

void MainScene::menuCloseCallback(Ref* pSender) {
    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void MainScene::set_pill(int pill_now[]) {
    Medical_cnt->setString(std::to_string(pill_now[0]));
    Firstaid_cnt->setString(std::to_string(pill_now[1]));
    Drink_cnt->setString(std::to_string(pill_now[2]));
    Bandage_cnt->setString(std::to_string(pill_now[3]));
}

void MainScene::try_receive(float dt)
{
	static clock_t last_time;
	static int check_cnt;
	if (!isOnline)return;
	if (wait_time>0&&(clock()-wait_time)/CLOCKS_PER_SEC>5)
		{
		isOnline = false;
		isEnd = true;
		close_socket();
		auto scene = ConnectfailScene::createScene();
		Director::getInstance()->replaceScene(scene);
		return;
		}
	else wait_time = 0;
	if (!received())return;
	if (check_cnt<0||check_cnt>60)check_cnt = 0;
	if (check_cnt<60)++check_cnt;
	else
		{
		check_cnt = 0;
		Ping_time->setString("Ping:"+std::to_string((clock()-last_time)*1000/60/CLOCKS_PER_SEC)+"ms");
		last_time = clock();
		}
	if (socket_closed())
		{
		isOnline = false;
		isEnd = true;
		auto scene = ConnectfailScene::createScene();
		Director::getInstance()->replaceScene(scene);
		return;
		}
	extern s_c_msg s2c;
	if (!isRunning)
		{
		playerID = s2c.infoy;
		max_player = s2c.live_count;
		if (s2c.type)
			{
			isRunning = true;
            auto audio = SimpleAudioEngine::getInstance();
            audio->stopBackgroundMusic();
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
	if (!isRunning);
	else if (s2c.type==2||s2c.type==3)
		{
		isOnline = false;
		isRunning = false;
		isEnd = true;
		player->setHP(0);
		Wflag = Aflag = Sflag = Dflag = false;
		if (isOpenSight)closeSight();
		if (isOpenBox)closeBox();
		if (isOpenMap)closeMap();
		close_socket();
		if (s2c.type==2)
			player->isDying();
		FinalScene(string(s2c.user_name[playerID]), s2c.infoy, s2c.infox, (s2c.type==3)?0:1);
		}
	else
		{

		if (s2c.Poison_LEVEL)
			{
			setSafeZone(Vec2(s2c.Poison_X*2, s2c.Poison_Y*2), s2c.Poison_LEVEL);
			}

		int NowBoxNum = Box_ve.size();
		for (int i = 0;i<NowBoxNum;++i)
			{
			if(Box_ve[i].set(s2c.Boxes[i].Wp1Type, s2c.Boxes[i].Wp2Type, s2c.Boxes[i].Wp1Bullets, s2c.Boxes[i].Wp2Bullets, s2c.Boxes[i].PillAmount, (int)(std::ceil(s2c.Boxes[i].Armor)+0.01))&&isOpenBox&&i==OpenBoxID)update_box();
			}
		for (int i = NowBoxNum;i<s2c.Boxnum;++i)
			{
			Box_ve.push_back(Box(s2c.Boxes[i].x*2, s2c.Boxes[i].y*2, s2c.Boxes[i].type, s2c.Boxes[i].Wp1Type, s2c.Boxes[i].Wp2Type, s2c.Boxes[i].Wp1Bullets, s2c.Boxes[i].Wp2Bullets, s2c.Boxes[i].PillAmount, (int)(std::ceil(s2c.Boxes[i].Armor)+0.01)));
			}
            
        while(totBoxNum < s2c.Boxnum) {
            // draw boxes
			if (Box_ve[totBoxNum].type)
				{
				auto tmpbox = Sprite::create("others/airdrop.png");
				tmpbox->setPosition(Box_ve[totBoxNum].x, Box_ve[totBoxNum].y);
				addChild(tmpbox, -2);
				Box_pic[totBoxNum] = tmpbox;
				auto tmppoint = Sprite::create("others/littleairdrop.png");
				tmppoint->setPosition(player->getPosition());
				addChild(tmppoint, 30);
				tmppoint->setVisible(false);
				AirdropNum[totAirdrop] = totBoxNum;
				littleAirdrop[totAirdrop] = tmppoint;
				++totAirdrop;
				}
			else
				{
				auto tmpbox = Sprite::create("others/box.png");
				addChild(tmpbox, -2);
				tmpbox->setPosition(Box_ve[totBoxNum].x, Box_ve[totBoxNum].y);
				Box_pic[totBoxNum] = tmpbox;
				}
            
            ++totBoxNum;
        }

		if (!isOpenBox&&!isOpenSight)
			{
            // audio
                if(player->getLife() - s2c.currenthp > 10) {
                    auto audio = SimpleAudioEngine::getInstance();
                    audio->playEffect("music/effect/hurt.mp3");
                }
            player->setHP(s2c.currenthp);
			player->setShield(s2c.Armornaijiu);
			Warning->setVisible(s2c.inpoison);
			Healing->setVisible(s2c.IsCuring);
			}

		set_pill(s2c.PillAmount);
            
		if (~s2c.SubWeaponType)
			player->setSubWeapon(s2c.SubWeaponType);
		else
			player->setSubWeapon(4);
		player->setBullet(s2c.MainWeaponCurBullet, s2c.MainWeaponBackupBullet, s2c.SubWeaponCurBullet, s2c.SubWeaponBackupBullet);

		show_remain(s2c.live_count);
            
		for (int i = 0;i<SOLDIER_NUM;++i)
			{
			if (i==playerID)continue;
			if (enemy[i]->dead())continue;
			enemy[i]->setPosition(s2c.x[i]*2+16, s2c.y[i]*2+16);
			enemy[i]->setRotation(s2c.face_angle[i]);
			if (~s2c.MainWeaponType[i])
				enemy[i]->setMainWeapon(s2c.MainWeaponType[i]);
			else
				enemy[i]->setMainWeapon(4);
            if (s2c.Firing[i]) {
                enemy[i]->Shoot();
				enemy[i]->makeWave(this);
            }
			if (s2c.Isdead[i])
				{
				enemy[i]->setHP(0);
                enemy[i]->isDying();    // show exploit animation
				if (s2c.BeKilledByPlayerId[i]>=0)show_notice(string(s2c.user_name[i])+" was killed by "+string(s2c.user_name[s2c.BeKilledByPlayerId[i]]));
				else if (s2c.BeKilledByPlayerId[i]==-1)show_notice(string(s2c.user_name[i])+" was killed out of safe zone");
				else show_notice(string(s2c.user_name[i])+" has disconnected");
				}
			}
		if (~s2c.MainWeaponType[playerID])
			player->setMainWeapon(s2c.MainWeaponType[playerID]);
		else
			player->setMainWeapon(4);
        if(s2c.Firing[playerID]) {
            player->Shoot();
            auto audio = SimpleAudioEngine::getInstance();
            int weapon = s2c.MainWeaponType[playerID];
            if(weapon != 4) {
                audio->playEffect(audioName[weapon].c_str());
            }
        }
		extern c_s_msg to_be_sent;
		to_be_sent.face_angle = player->getRotation();
		to_be_sent.type = 1;
		to_be_sent.x = (player->getPosition().x-16)/2;
		to_be_sent.y = (player->getPosition().y-16)/2;
		}
	write();
}
