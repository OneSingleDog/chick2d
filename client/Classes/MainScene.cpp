//
//  MainScene.cpp
//  chick2d
//
//  Created by 侯宇蓬 on 2018/4/24.
//

#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <cstring>
#include <Notice.h>

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
    int pill1;
    int pill2;
    int shield;
    Box(int x = -1, int y = -1, int w1 = 0, int w2 = 0, int b1 = 0, int b2 = 0, int p1 = 0, int p2 = 0, int s = 0) :
    x(x), y(y), weapon1(w1), weapon2(w2), bullet1(b1), bullet2(b2), pill1(p1), pill2(p2), shield(s) {}
};

std::vector<Box> Box_ve;

Scene* MainScene::createScene() {
    return MainScene::create();
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    
    playerID = 0;
    isOpenSight = false;
    isOpenBox = false;
    isOpenMap = false;
    
    Box_ve.push_back(Box(64, 64, 1, 0, 60, 0, 1, 2, 100));
    Box_ve.push_back(Box(80, 64, 1, 0, 60, 0, 1, 2, 100));
    Box_ve.push_back(Box(64, 80, 1, 0, 60, 0, 1, 2, 100));

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
    glass->setPosition(player->getPosition());
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
        Point ori((player->getPosition() + direction * 6.1).x, (player->getPosition() + direction * 6.1).y);
        
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
    }
}

// 0 -> can move
// 1 -> cannot move
int MainScene::judgePlayerPosition(Point position) {
    Point tileCoord = this->tileCoordFromPosition(position);
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
    if (status == 0) {//not begiin
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        already = cocos2d::Label::createWithTTF(" ", "fonts/Marker Felt.ttf", 20);
        //Notice->setString("xixixi");
        already->setString(std::to_string(ready_person) + "/ 4");
        already->setPosition(player->getPosition().x + visibleSize.width / 2-20, player->getPosition().y + visibleSize.height / 2-20);
        addChild(already);
    }
    else if (status == 1) {
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

void MainScene::OpenBox(int boxID) {
    log("open Box:%d", boxID);
    glass->setPosition(player->getPosition());
    glass->setVisible(true);
    isOpenBox = true;
    player->hideStatus();
    fog->setVisible(false);
    
    // create Menu
//    Vector<MenuItem*> MenuItems;
//
//    for(int i = 0; i < (int)Box_ve.size(); ++i) {
//
//    }
    //auto myMenu = Menu::create();
    // creating a menu with a single item
    
    // create a menu item by specifying images
    
    //auto closeItem = MenuItemImage::create("StartGameNormal.png", "StartGameSelected.png",
//                                           [&](Ref* sender){
//                                               log("I'm touched!!!");
//                                           });
    
//    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//    auto closeItem = MenuItemImage::create(
//                                           "StartGameNormal.png",
//                                           "StartGameSelected.png",
//                                           CC_CALLBACK_1(MainScene::func, this));
//
//    auto menu = Menu::create(closeItem, NULL);
//    closeItem->setPosition(Vec2::ZERO);
//    menu->setPositionZ(1);
//    menu->setPosition(Vec2::ZERO);
//    this->addChild(menu, 1);
//    log("%f", menu->getPositionZ());
}

void MainScene::closeBox() {
    glass->setVisible(false);
    isOpenBox = false;
    player->showStatus();
    fog->setVisible(true);
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

void MainScene::ReadyCallback() {
    to_ready->setVisible(false);
    Point cur_pos = player->getPosition();
}
