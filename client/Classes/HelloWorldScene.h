#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

class HelloWorld : public cocos2d::Scene
{

private:

	cocos2d::ui::TextField* username;
	cocos2d::ui::TextField* serverip;
	cocos2d::ui::TextField* serverport;

public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	void StartCallback(cocos2d::Ref* pSender);
	void textFieldEvent_username(Ref * pSender, cocos2d::ui::TextField::EventType type);
	void textFieldEvent_userip(Ref * pSender, cocos2d::ui::TextField::EventType type);
	void textFieldEvent_userport(Ref * pSender, cocos2d::ui::TextField::EventType type);

	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);
};


#endif // __HELLOWORLD_SCENE_H__
