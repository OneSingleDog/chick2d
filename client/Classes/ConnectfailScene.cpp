#include "cocos2d.h"
#include "ConnectfailScene.h"



cocos2d::Scene* ConnectfailScene::createScene()
{
	return ConnectfailScene::create();
}


void ConnectfailScene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	cocos2d::Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
}

bool ConnectfailScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	//auto back = cocos2d::Sprite::create("scene.jpg");
	//back->setPosition(80, 240);
	//this->addChild(back, 1);



	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = cocos2d::MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(ConnectfailScene::menuCloseCallback, this));


	float x = visibleSize.width / 2;
	float y = visibleSize.height / 2 - 200;
	closeItem->setPosition(cocos2d::Vec2(x, y));
	closeItem->setScaleX(1.5);
	closeItem->setScaleY(1.5);

	/*
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
	}*/

	// create menu, it's an autorelease object
	auto menu = cocos2d::Menu::create(closeItem, NULL);
	menu->setPosition(cocos2d::Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Chick2d"
	// create and initialize a label

	auto label = cocos2d::Label::createWithTTF("Chick2d", "fonts/Marker Felt.ttf", 180);

	// position the label on the center of the screen
	label->setPosition(cocos2d::Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height * 3 / 4));

	// add the label as a child to this layer
	this->addChild(label, 1);
	
	auto label2 = cocos2d::Label::createWithTTF("Connection Fail", "fonts/Marker Felt.ttf", 100);
	label2->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(label2, 1);

	return true;
}