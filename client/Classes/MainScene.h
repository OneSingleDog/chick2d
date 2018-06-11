//
//  MainScene.h
//  chick2d
//
//  Created by 侯宇蓬 on 2018/4/24.
//

#ifndef MainScene_h
#define MainScene_h

#include "cocos2d.h"
#include "Soldier.h"
#include "ui/CocosGUI.h"

const int SOLDIER_NUM = 4;

class MainScene : public cocos2d::Scene
{
private:
    cocos2d::TMXTiledMap *MainMap;
    cocos2d::TMXLayer *background;  // background layer of MainMap
    cocos2d::TMXLayer *collidable;
    Soldier *player;
    cocos2d::Sprite *mouse;
    cocos2d::Sprite *fog;
    cocos2d::Sprite *sight;
    cocos2d::Sprite *glass;
    cocos2d::Sprite *littleMap;
    cocos2d::Sprite *littlePoint;
        
    cocos2d::Vec2 direction;
    bool isOpenSight;
    bool isOpenBox;
    bool isOpenMap;

    cocos2d::MenuItemImage* GoGameItem;
    cocos2d::Label*Notice;
    cocos2d::Label * to_ready;
    cocos2d::Label * already;

    int playerID;
    
    
public:
    static double BASIC_SPEED;
    static int SPEED_RATIO;
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
    
    void setViewPointCenter(cocos2d::Point position);
        
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    void setMousePosition(cocos2d::Event *event);
    void myMoveAction(float dt);
    
    void openSight();
    void closeSight();
    
    int judgePlayerPosition(cocos2d::Point position);
    cocos2d::Point tileCoordFromPosition(cocos2d::Point pos);
    
    void scheduleBlood(float delta);
    void CheckBoxes();
    void OpenBox(int boxID);
    void closeBox();
    void openMap();
    void closeMap();
    
    //void scheduleBlood(float delta);
    void GOCallback(cocos2d::Ref* pSender);
    void show_notice(std::string killer,std::string be_killed);
    void show_begin(int status,int ready_person);
    void ReadyCallback();
};

#endif /* MainScene_h */
