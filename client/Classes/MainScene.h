#ifndef MainScene_h
#define MainScene_h

#include "cocos2d.h"
#include "Soldier.h"
#include "ui/CocosGUI.h"

class MainScene : public cocos2d::Scene
{
private:
	static const int SOLDIER_NUM = 4;
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
	cocos2d::Sprite *Safe_Zone;
	cocos2d::Sprite *littleSafeZone;
	cocos2d::Sprite *Medical_kit;
	cocos2d::Sprite *First_aid;
	cocos2d::Sprite *Bandage;
	cocos2d::Sprite *Drink;

	cocos2d::Sprite *DeadLayer;
	cocos2d::Sprite *Warning;
    cocos2d::Sprite *Healing;

    cocos2d::Menu *boxMenu;
        
    cocos2d::Vec2 direction;
    bool isOpenSight;
    bool isOpenBox;
    bool isOpenMap;

	bool isOnline;
	bool isRunning;

    cocos2d::MenuItemImage* GoGameItem;
    cocos2d::Label  *Notice;
    cocos2d::Label * to_ready;
    cocos2d::Label * already;
	cocos2d::Label * Medical_cnt;
	cocos2d::Label * Firstaid_cnt;
	cocos2d::Label * Bandage_cnt;
	cocos2d::Label * Drink_cnt;
	cocos2d::Label * Remain;

	 static float SafezoneScaleSize[4];

    int playerID;
    
	Soldier*enemy[SOLDIER_NUM];
    
    bool Wflag, Aflag, Sflag, Dflag;

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
    
    void CheckBoxes();
    void OpenBox(int boxID);
    void closeBox();
    void openMap();
    void closeMap();
    
    //void scheduleBlood(float delta);
    void show_notice(std::string killevent);
	void show_begin(int status, int ready_person);
	void show_remain(int life_cnt);
    void ReadyCallback();

	void setSafeZone(cocos2d::Point new_center, int size);
	void FinalScene(std::string Uername, int rank, int kill_num, int ifwinner);

	void try_receive(float dt);
};

#endif /* MainScene_h */
