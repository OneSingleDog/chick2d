#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "MainScene.h"


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

//    auto back = Sprite::create("scene.jpg");
//    back->setPosition(80, 240);
//    this->addChild(back, 1);



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
	auto s = ui::TextField::create("<Please input username here>",
		"fonts/Marker Felt.ttf",
		32);
	s->setPosition(Point(origin.x + visibleSize.width / 2 +130, origin.y + visibleSize.height / 2+50));
	s->addEventListener(CC_CALLBACK_2(HelloWorld::textFieldEvent_username, this));
	this->addChild(s, 1);


	auto user_ip = Label::create("ServerIP:", "fonts/Marker Felt.ttf", 32);
	user_ip->setPosition(visibleSize.width / 2 - 130, visibleSize.height / 2 - 25);
	this->addChild(user_ip, 1);
	auto ip = ui::TextField::create("<Please input server-ip here>",
		"fonts/Marker Felt.ttf",
		32);
	ip->setPosition(Point(origin.x + visibleSize.width / 2 + 130, origin.y + visibleSize.height / 2 - 25));
	ip->addEventListener(CC_CALLBACK_2(HelloWorld::textFieldEvent_userip, this));
	this->addChild(ip, 1);
	const std::string username_input = ip->getString();
	//std::cout << username_input << std::endl;


	auto user_port = Label::create("ServerPort:", "fonts/Marker Felt.ttf", 32);
	user_port->setPosition(visibleSize.width / 2 - 130, visibleSize.height / 2 - 100);
	this->addChild(user_port, 1);
	auto port = ui::TextField::create("<Please input server-port here>",
		"fonts/Marker Felt.ttf",
		32);
	port->setPosition(Point(origin.x + visibleSize.width / 2 + 130, origin.y + visibleSize.height / 2 - 100));
	port->addEventListener(CC_CALLBACK_2(HelloWorld::textFieldEvent_userport, this));
	this->addChild(port, 1);


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

		textField->runAction(CCMoveTo::create(0.225f, Vec2(screenSize.width / 2.0f+130, screenSize.height / 2.0f -5)));

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

		textField->runAction(CCMoveTo::create(0.225f, Vec2(screenSize.width / 2.0f+130, screenSize.height / 2.0f - 25)));
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
	auto scene = MainScene::createScene();
	Director::getInstance()->replaceScene(scene);
}
