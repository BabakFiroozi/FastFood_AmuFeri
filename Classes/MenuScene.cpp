#include "MenuScene.h"
#include "GameplayScene.h"
#include "GameChoice.h"
#include "ShopScene.h"

USING_NS_CC;
using namespace cocos2d::ui;

MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{
}

bool MenuScene::init(ValueMap& initData)
{
	if (!Layer::init())
		return false;

	FoodFactory::getInstance().initialize();


	_visibleOrigin = Director::getInstance()->getVisibleOrigin();
	_visibleSize = Director::getInstance()->getVisibleSize();

	auto background = ImageView::create("gui/menu/menu_bg.png");
	addChild(background);
	background->setPosition(_visibleSize / 2);

	const int fontSize = 40;

	_playButton = Button::create("gui/menu/button.png");
	background->addChild(_playButton);
	_playButton->setPosition(Vect(_visibleSize.width / 2, _visibleSize.height - 1100));
	_playButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::buttonCallback, this));

	_exitButton = Button::create("gui/menu/exitButton.png");
	background->addChild(_exitButton);
	_exitButton->setPosition(Vect(_exitButton->getContentSize() / 2));
	_exitButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::buttonCallback, this));

	_settingButton = Button::create("gui/menu/settingButton.png");
	background->addChild(_settingButton);
	_settingButton->setPosition(Vect(_visibleSize.width - _settingButton->getContentSize().width / 2, _settingButton->getContentSize().height / 2));
	_settingButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::buttonCallback, this));

	_foodButton = Button::create("gui/menu/shopButton.png", "gui/menu/shopButton_press.png");
	background->addChild(_foodButton);
	_foodButton->setPosition(Vect(_visibleSize.width / 2, _visibleSize.height - _foodButton->getContentSize().height / 2));
	_foodButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::buttonCallback, this));

	auto shopBadge = ImageView::create("gui/menu/shopBadge.png");
	_foodButton->addChild(shopBadge);
	shopBadge->setPosition(_foodButton->getContentSize() / 2 + Size(0, 10));

	auto seq = Sequence::createWithTwoActions(ScaleTo::create(.25f, 1.1f), ScaleTo::create(.25f, 1));
	shopBadge->runAction(RepeatForever::create(seq));

	return true;
}

void MenuScene::onEnter()
{
	Layer::onEnter();
}

void MenuScene::update(float dt)
{
	Layer::update(dt);
}

void MenuScene::buttonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == Widget::TouchEventType::ENDED)
	{
		if (sender == _playButton)
		{
			auto scene = TransitionSlideInR::create(.5f, GameplayScene::createSceneData(1));
			Director::getInstance()->replaceScene(scene);
		}
		if (sender == _foodButton)
		{
			auto scene = TransitionSlideInR::create(.5f, ShopScene::createSceneData(ShopTypes::Food));
			Director::getInstance()->replaceScene(scene);
		}
		if (sender == _exitButton)
		{
			Director::getInstance()->end();
		}
	}
}
