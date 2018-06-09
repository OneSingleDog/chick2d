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

USING_NS_CC;

double MainScene::BASIC_SPEED = 2.5;
int MainScene::SPEED_RATIO = 1;  // 1 if not running

Scene* MainScene::createScene() {
    return MainScene::create();
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    isOpenSight = false;
    
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    player = new Soldier();
    
    // create Main Map using Map.tmx
    MainMap = TMXTiledMap::create("map/demo.tmx");
    
    // Get background layer
    background = MainMap->getLayer("Background");
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
    
    
    
    addChild(MainMap, -1);
    player->addChild(this);
    setViewPointCenter(player->getPosition());
    
    // mouse create
    mouse = Sprite::create("others/mouse.png");
    mouse->setScale(0.04);
    addChild(mouse);
    
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
    
    return true;
}

void MainScene::openSight() {
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
        Point new_pos((player->getPosition() + direction * SPEED_RATIO).x, (player->getPosition() + direction * SPEED_RATIO).y);
        if(xflag == false) { new_pos.x = player->getPosition().x; }
        if(yflag == false) { new_pos.y = player->getPosition().y; }
        player->setPosition(new_pos.x, new_pos.y);
        setViewPointCenter(player->getPosition());
        if(isOpenSight) {
            closeSight();
        }
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
            
        case 135:
            player->increaseShieldVal(20);
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
}

