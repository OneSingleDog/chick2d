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
        
    cocos2d::Vec2 direction;
    bool isOpenSight;

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
};

#endif /* MainScene_h */
