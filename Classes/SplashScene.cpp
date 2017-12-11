#include "SplashScene.h"
#include "MenuScene.h"
#include "Inventories.h"
#include "PlayerPrefs.h"
#include "FoodFactory.h"
#include "GameUser.h"
#include "SimpleAudioEngine.h"
#include "GameChoice.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "ui/UIVideoPlayer.h"
#endif


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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	auto player = experimental::ui::VideoPlayer::create();
	player->setFileName("gui/splash.ogg");
	player->play();
	_duration = 6;
#else
	auto splashImage = ImageView::create("gui/Logo.png");
	addChild(splashImage);
	splashImage->setPosition(visibleSize / 2);
	_duration = 1;
#endif

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
		PlayerPrefs::getInstance().saveAddedPowerups();

		PlayerPrefs::getInstance().doneFirstRun();
	}
	else
	{
		PlayerPrefs::getInstance().loadCoin();
		PlayerPrefs::getInstance().loadFoods();
		PlayerPrefs::getInstance().loadCurrentKitchen();
		PlayerPrefs::getInstance().loadUnlockedKitchens();
		PlayerPrefs::getInstance().loadAddedPowerups();
	}

	return true;
}

void SplashScene::onEnter()
{
	Layer::onEnter();

	bool on = PlayerPrefs::getInstance().getVolume();
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(on ? GameChoice::getInstance().getMusicVolume() : 0);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(on ? GameChoice::getInstance().getEffectVolume() : 0);

	scheduleOnce(CC_SCHEDULE_SELECTOR(SplashScene::goMenu), _duration);
}

void SplashScene::goMenu(float dt)
{
	auto scene = TransitionFade::create(.5f, MenuScene::createSceneData());
	Director::getInstance()->replaceScene(scene);
}
