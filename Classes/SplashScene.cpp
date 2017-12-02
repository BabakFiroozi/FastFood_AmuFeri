#include "SplashScene.h"
#include "MenuScene.h"
#include "Inventories.h"
#include "PlayerPrefs.h"
#include "FoodFactory.h"
#include "GameUser.h"

USING_NS_CC;
using namespace cocos2d::ui;

SplashScene::SplashScene()
{
}

SplashScene::~SplashScene()
{
}

bool SplashScene::init()
{
	if (!Layer::init())
		return false;

	Vect visibleOrigin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto splashImage = ImageView::create("gui/Logo.png");
	addChild(splashImage);
	splashImage->setPosition(visibleSize / 2);

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
		PlayerPrefs::getInstance().loadCoin();
		PlayerPrefs::getInstance().loadFoods();
		PlayerPrefs::getInstance().loadCurrentKitchen();
		PlayerPrefs::getInstance().loadUnlockedKitchens();
	}

	return true;
}

void SplashScene::onEnter()
{
	Layer::onEnter();

	scheduleOnce(CC_SCHEDULE_SELECTOR(SplashScene::goMenu, this), 1);
}

void SplashScene::goMenu(float dt)
{
	auto scene = TransitionFade::create(.5f, MenuScene::createSceneData());
	Director::getInstance()->replaceScene(scene);
}
