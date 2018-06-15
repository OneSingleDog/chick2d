#ifndef MainScene_h
#define MainScene_h

#include "cocos2d.h"
#include "Soldier.h"
#include "ui/CocosGUI.h"

#include "msg.h"

USING_NS_CC;

struct Box {
	int x, y, type;
	int weapon1;
	int weapon2;
	int bullet1;
	int bullet2;
	int pillNum[4];
	int shield;

	Box(int x = -1, int y = -1, int type = 0, int w1 = 0, int w2 = 0, int b1 = 0, int b2 = 0, int *p = NULL, int s = 0):
		x(x), y(y), type(type), weapon1(w1), weapon2(w2), bullet1(b1), bullet2(b2), shield(s) {
		pillNum[0] = p[0];
		pillNum[1] = p[1];
		pillNum[2] = p[2];
		pillNum[3] = p[3];
		}

	bool set(int w1 = 0, int w2 = 0, int b1 = 0, int b2 = 0, int *p = NULL, int s = 0)
		{
		bool flag = false;
		if (weapon1!=w1) { weapon1 = w1;flag = true; }
		if (weapon2!=w2) { weapon2 = w2;flag = true; }
		if (bullet1!=b1) { bullet1 = b1;flag = true; }
		if (bullet2!=b2) { bullet2 = b2; flag = true; }
		if (pillNum[0]!=p[0]) { pillNum[0] = p[0];flag = true; }
		if (pillNum[1]!=p[1]) { pillNum[1] = p[1];flag = true; }
		if (pillNum[2]!=p[2]) { pillNum[2] = p[2];flag = true; }
		if (pillNum[3]!=p[3]) { pillNum[3] = p[3];flag = true; }
		if (shield!=s) { shield = s;flag = true; };
		return flag;
		}

	Vec2 getPosition()
		{
		return Vec2(x, y);
		}
};

class MainScene : public cocos2d::Scene
{
private:
	cocos2d::Size visibleSize = Director::getInstance()->getWinSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	static const int SOLDIER_NUM = 4;
	static const int LITTLE_MAP_SIZE = 512;
	static const int BOX_NUM = 30;
	const double OK_OPEN_BOX = 32.0;

	static const int NUM_OF_PILL = 4;

	string MainScene::boxWeapon[Soldier::NUM_OF_WEAPON] = {"box/weaponshow0.png", "box/weaponshow1.png", "box/weaponshow2.png", "box/weaponshow3.png", "box/bottom.png"};
	string MainScene::boxPill[NUM_OF_PILL] = {"box/pill0.png", "box/pill1.png", "box/pill2.png", "box/pill3.png"};
	string audioName[Soldier::NUM_OF_WEAPON] = {"music/effect/weaponaudio0.mp3", "music/effect/weaponaudio1.mp3", "music/effect/weaponaudio2.mp3", "music/effect/weaponaudio3.mp3"};

	std::vector<Box> Box_ve;

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
	cocos2d::Sprite *littleAirdrop[MAXLEVEL];
	int AirdropNum[MAXLEVEL];
	cocos2d::Sprite *Safe_Zone;
	cocos2d::Sprite *littleSafeZone;
	cocos2d::Sprite *Medical_kit;
	cocos2d::Sprite *First_aid;
	cocos2d::Sprite *Bandage;
	cocos2d::Sprite *Drink;

	cocos2d::Sprite *DeadLayer;
	cocos2d::Sprite *Warning;
    cocos2d::Sprite *Healing;

	cocos2d::Sprite *Box_pic[BOX_NUM];

    cocos2d::Menu *boxMenu;
        
    cocos2d::Vec2 direction;
    bool isOpenSight;
    bool isOpenBox;
    bool isOpenMap;

	bool isOnline;
	bool isRunning;
	bool isEnd;

    cocos2d::MenuItemImage* GoGameItem;
    cocos2d::Label *Notice;
    cocos2d::Label * to_ready;
    cocos2d::Label * already;
	cocos2d::Label * Medical_cnt;
	cocos2d::Label * Firstaid_cnt;
	cocos2d::Label * Bandage_cnt;
	cocos2d::Label * Drink_cnt;
	cocos2d::Label * Remain;
	cocos2d::Label * Ping_time;

	static float SafezoneScaleSize[4];
	static const string PillName[4];

    int playerID;
	int OpenBoxID;
    
	Soldier*enemy[SOLDIER_NUM];
    
    bool Wflag, Aflag, Sflag, Dflag;

	bool start_fight = false;   // if start playing "fight.mp3"
    
    int totBoxNum;
	int totAirdrop;

	int max_player;
	clock_t wait_time;

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
   
    void show_notice(std::string killevent);
	void show_begin(int status, int ready_person);
	void show_remain(int life_cnt);
    void ReadyCallback();

	void setSafeZone(cocos2d::Point new_center, int size);
	void FinalScene(std::string Uername, int rank, int kill_num, int ifwinner);

	void try_receive(float dt);
    
    void set_pill(int pill_now[]);
	void update_box();
    
    void menuCloseCallback(Ref* pSender);
};

#endif /* MainScene_h */
