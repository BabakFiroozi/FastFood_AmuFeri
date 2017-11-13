#include "MenuScene.h"
#include "GameplayScene.h"
#include "GameChoice.h"
#include "ShopScene.h"
#include "PlayerPrefs.h"
#include "Inventories.h"
#include "GameUser.h"

USING_NS_CC;
using namespace cocos2d::ui;

bool MenuScene::_inited = false;

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

	std::string inventoriesStr = cocos2d::FileUtils::getInstance()->getStringFromFile("inventories.json");
	Inventories::getInstance().initialize(inventoriesStr);

	bool isFirsrRun = PlayerPrefs::getInstance().isFirstRun();
	if (isFirsrRun)
	{
		std::string foodsStr = cocos2d::FileUtils::getInstance()->getStringFromFile("foods.json");
		FoodFactory::getInstance().initialize(foodsStr);
		PlayerPrefs::getInstance().saveFoods();

		Inventories::getInstance().unlockKitchen(KitchenTypes::Kitchen_1);
		PlayerPrefs::getInstance().saveUnlockedKitchens();
		GameUser::getInstance().setCurrentKitchen(KitchenTypes::Kitchen_1);
		PlayerPrefs::getInstance().saveCurrentKitchen();

		PlayerPrefs::getInstance().doneFirstRun();
	}
	else
	{
		if (!_inited)
		{
			PlayerPrefs::getInstance().loadCoin();
			PlayerPrefs::getInstance().loadFoods();
			PlayerPrefs::getInstance().loadCurrentKitchen();
			PlayerPrefs::getInstance().loadUnlockedKitchens();
			_inited = true;
		}
	}


	_visibleOrigin = Director::getInstance()->getVisibleOrigin();
	_visibleSize = Director::getInstance()->getVisibleSize();

	_background = ImageView::create("gui/menu/menu_bg.png");
	addChild(_background);
	_background->setPosition(_visibleSize / 2);

	const int fontSize = 40;

	_playButton = Button::create("gui/menu/button.png");
	_background->addChild(_playButton);
	_playButton->setPosition(Vect(_visibleSize.width / 2, _visibleSize.height - 1100));
	_playButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::buttonCallback, this));

	_exitButton = Button::create("gui/menu/exitButton.png");
	_background->addChild(_exitButton);
	_exitButton->setPosition(Vect(_exitButton->getContentSize() / 2));
	_exitButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::buttonCallback, this));

	_settingButton = Button::create("gui/menu/settingButton.png");
	_background->addChild(_settingButton);
	_settingButton->setPosition(Vect(_visibleSize.width - _settingButton->getContentSize().width / 2, _settingButton->getContentSize().height / 2));
	_settingButton->addTouchEventListener(CC_CALLBACK_2(MenuScene::buttonCallback, this));

	_foodButton = Button::create("gui/menu/shopButton.png", "gui/menu/shopButton_press.png");
	_background->addChild(_foodButton);
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
