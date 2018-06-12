#include "HelloWorldScene.h"
#include "ConnectfailScene.h"
#include "SimpleAudioEngine.h"
#include "MainScene.h"

#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

#include "boost.h"

USING_NS_CC;
//USING_NS_CC_EXT;
using namespace std;

Scene* HelloWorld::createScene()
{
	return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// auto back = Sprite::create("scene.jpg");
	// back->setPosition(80, 240);
	// this->addChild(back, 1);



	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// Add Start Game Button
	auto StartGameItem = MenuItemImage::create(
		"StartGameNormal.png",
		"StartGameSelected.png",
		CC_CALLBACK_1(HelloWorld::StartCallback, this));

	if (StartGameItem == nullptr ||
		StartGameItem->getContentSize().width <= 0 ||
		StartGameItem->getContentSize().height <= 0)
	{
		problemLoading("'StartGameNormal.png' and 'StartGameSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width / 2;
		float y = origin.y + visibleSize.height / 4 - 20;
		StartGameItem->setPosition(Vec2(x, y));
		StartGameItem->setScaleX(0.5);
		StartGameItem->setScaleY(0.5);
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, StartGameItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Chick2d"
	// create and initialize a label

	auto label = Label::createWithTTF("Chick2d", "fonts/Marker Felt.ttf", 180);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height * 3 / 4));

		// add the label as a child to this layer
		this->addChild(label, 1);
	}

	auto user_name = Label::create("Username:", "fonts/Marker Felt.ttf",32);
	user_name->setPosition(visibleSize.width / 2 - 130, visibleSize.height / 2+50);
	this->addChild(user_name, 1);
	username = ui::TextField::create("<Please input username here>",
		"fonts/Marker Felt.ttf",
		32);
	username->setPosition(Point(origin.x + visibleSize.width / 2 +130, origin.y + visibleSize.height / 2+50));
	username->addEventListener(CC_CALLBACK_2(HelloWorld::textFieldEvent_username, this));
	this->addChild(username, 1);


	auto server_ip = Label::create("ServerIP:", "fonts/Marker Felt.ttf", 32);
	server_ip->setPosition(visibleSize.width / 2 - 130, visibleSize.height / 2 - 25);
	this->addChild(server_ip, 1);
	serverip = ui::TextField::create("<Please input server-ip here>",
		"fonts/Marker Felt.ttf",
		32);
	serverip->setPosition(Point(origin.x + visibleSize.width / 2 + 130, origin.y + visibleSize.height / 2 - 25));
	serverip->addEventListener(CC_CALLBACK_2(HelloWorld::textFieldEvent_userip, this));
	this->addChild(serverip, 1);


	auto server_port = Label::create("ServerPort:", "fonts/Marker Felt.ttf", 32);
	server_port->setPosition(visibleSize.width / 2 - 130, visibleSize.height / 2 - 100);
	this->addChild(server_port, 1);
	serverport = ui::TextField::create("<Please input server-port here>",
		"fonts/Marker Felt.ttf",
		32);
	serverport->setPosition(Point(origin.x + visibleSize.width / 2 + 130, origin.y + visibleSize.height / 2 - 100));
	serverport->addEventListener(CC_CALLBACK_2(HelloWorld::textFieldEvent_userport, this));
	this->addChild(serverport, 1);


	return true;
}
void HelloWorld::textFieldEvent_username (Ref * pSender, ui::TextField::EventType type)
{
	if (type == ui::TextField::EventType::ATTACH_WITH_IME) {

		ui::TextField * textField = dynamic_cast<ui::TextField*>(pSender);

		Size screenSize = CCDirector::getInstance()->getWinSize();

		textField->runAction(CCMoveTo::create(0.225f, Vec2(screenSize.width / 2.0f + 130, screenSize.height / 2.0f+70)));

		//displayValueLabel->setString(String::createWithFormat("attach with IME")->getCString());

		textField->setMaxLengthEnabled(true); //设置允许最大数目限制

		textField->setMaxLength(20);
		//设置最大允许数目为20

		textField->setPlaceHolder("input text here");  //设置占位符 

		textField->setPlaceHolderColor(Color4B::GRAY); //设置占位符颜色

		textField->setTextColor(Color4B::WHITE);
	}
	else if (type == ui::TextField::EventType::DETACH_WITH_IME) {

		ui::TextField * textField = dynamic_cast<ui::TextField*>(pSender);

		Size screenSize = CCDirector::getInstance()->getWinSize();

		textField->runAction(CCMoveTo::create(0.225f, Vec2(screenSize.width / 2.0f+130, screenSize.height / 2.0f+50)));
	}

}
void HelloWorld::textFieldEvent_userip(Ref * pSender, ui::TextField::EventType type)
{
	if (type == ui::TextField::EventType::ATTACH_WITH_IME) {

		ui::TextField * textField = dynamic_cast<ui::TextField*>(pSender);

		Size screenSize = CCDirector::getInstance()->getWinSize();

		textField->runAction(CCMoveTo::create(0.225f, Vec2(screenSize.width / 2.0f + 130, screenSize.height / 2.0f - 5)));

		//displayValueLabel->setString(String::createWithFormat("attach with IME")->getCString());

		textField->setMaxLengthEnabled(true); //设置允许最大数目限制

		textField->setMaxLength(15);
		//设置最大允许数目为15

		textField->setPlaceHolder("input text here");  //设置占位符 

		textField->setPlaceHolderColor(Color4B::GRAY); //设置占位符颜色

		textField->setTextColor(Color4B::WHITE);
	}
	else if (type == ui::TextField::EventType::DETACH_WITH_IME) {

		ui::TextField * textField = dynamic_cast<ui::TextField*>(pSender);

		Size screenSize = CCDirector::getInstance()->getWinSize();

		textField->runAction(CCMoveTo::create(0.225f, Vec2(screenSize.width / 2.0f + 130, screenSize.height / 2.0f - 25)));
	}
}
void HelloWorld::textFieldEvent_userport(Ref * pSender, ui::TextField::EventType type)
{
	if (type == ui::TextField::EventType::ATTACH_WITH_IME) {

		ui::TextField * textField = dynamic_cast<ui::TextField*>(pSender);

		Size screenSize = CCDirector::getInstance()->getWinSize();

		textField->runAction(CCMoveTo::create(0.225f, Vec2(screenSize.width / 2.0f+130, screenSize.height / 2.0f - 80)));

		//displayValueLabel->setString(String::createWithFormat("attach with IME")->getCString());

		textField->setMaxLengthEnabled(true); //设置允许最大数目限制

		textField->setMaxLength(6);
		//设置最大允许数目为6

		textField->setPlaceHolder("input text here");  //设置占位符 

		textField->setPlaceHolderColor(Color4B::GRAY); //设置占位符颜色

		textField->setTextColor(Color4B::WHITE);
	}
	else if (type == ui::TextField::EventType::DETACH_WITH_IME) {

		ui::TextField * textField = dynamic_cast<ui::TextField*>(pSender);

		Size screenSize = CCDirector::getInstance()->getWinSize();

		textField->runAction(CCMoveTo::create(0.225f, Vec2(screenSize.width / 2.0f+130, screenSize.height / 2.0f -100)));
	}

}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::StartCallback(cocos2d::Ref *pSender) {
	extern pthread_mutex_t mutex_boost;
	extern pthread_mutex_t mutex_cocos;
	extern string login_username;
	extern string login_host;
	extern string login_port;

	extern talk_to_svr::ptr ptr;

	login_username = username->getString();
	login_host = serverip->getString();
	login_port = serverport->getString();

	bool flag = true;
	if (login_username=="NULL")flag = false;
	if (login_username=="")flag = false;
	if (login_host=="")flag = false;
	if (login_port=="")flag = false;
	int len = login_username.length();
	for (int i = 0;i<len;++i)
		if (!flag)break;
		else if (login_username[i]>=48&&login_username[i]<=57||login_username[i]>=65&&login_username[i]<=90||login_username[i]>=97&&login_username[i]<=122||login_username[i]==95)continue;
		else flag = false;
	len = login_host.length();
	for (int i = 0;i<len;++i)
		if (!flag)break;
		else if (login_host[i]>=48&&login_host[i]<=57||login_host[i]==46)continue;
		else flag = false;
	len = login_port.length();
	for (int i = 0;i<len;++i)
		if (!flag)break;
		else if (login_port[i]>=48&&login_port[i]<=57)continue;
		else flag = false;
		
	if (flag)
		{
		pthread_mutex_unlock(&mutex_boost);
		pthread_mutex_lock(&mutex_cocos);
		flag = flag&&ptr->started();
		}
    
    
    
    
    
    
    // LOCAL !!!
    // flag = true;
    
    
    
    
    
    
    
	if (flag)
		{
		auto scene = MainScene::createScene();
		Director::getInstance()->replaceScene(scene);
		}
	else
		{
		auto scene = ConnectfailScene::createScene();
		Director::getInstance()->replaceScene(scene);
		}

}
