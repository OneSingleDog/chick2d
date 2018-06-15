#include "cocos2d.h"
#include "ConnectfailScene.h"



cocos2d::Scene* ConnectfailScene::createScene()
{
	return ConnectfailScene::create();
}


void ConnectfailScene::menuCloseCallback(Ref* pSender)
{
	cocos2d::Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

bool ConnectfailScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	auto back = cocos2d::Sprite::create("Background.jpg");
	back->setPosition(510, 400);
	this->addChild(back, 1);


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

	// create menu, it's an autorelease object
	auto menu = cocos2d::Menu::create(closeItem, NULL);
	menu->setPosition(cocos2d::Vec2::ZERO);
	this->addChild(menu, 1);

	auto label = cocos2d::Label::createWithTTF("Chick2d", "fonts/Marker Felt.ttf", 180);

	// position the label on the center of the screen
	label->setPosition(cocos2d::Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height * 3 / 4));

	// add the label as a child to this layer
	this->addChild(label, 1);
	
	auto label2 = cocos2d::Label::createWithTTF("Connection Fail", "fonts/Marker Felt.ttf", 100);
	label2->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(label2, 1);
	label2->setTextColor(cocos2d::Color4B::RED);

	return true;
}
