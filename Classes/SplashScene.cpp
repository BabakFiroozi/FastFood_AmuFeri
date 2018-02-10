#include "SplashScene.h"
#include "MenuScene.h"
#include "Inventories.h"
#include "PlayerPrefs.h"
#include "FoodFactory.h"
#include "GameUser.h"
#include "SimpleAudioEngine.h"
#include "GameChoice.h"
#include "Tapligh.h"
#include "Analytics.h"

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
	Tapligh::getInstance().loadAd(Tapligh::UNIT_CODE_1);

	if (!LayerColor::initWithColor(Color4B::BLACK))
		return false;

	Device::setKeepScreenOn(true);

	Vect visibleOrigin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	auto player = experimental::ui::VideoPlayer::create();
	addChild(player);
	player->setName("VideoPlayer");
	player->setPosition(visibleSize / 2);
	player->setFileName("gui/splash.mp4");
	player->play();
	player->setKeepAspectRatioEnabled(true);
	player->setContentSize(visibleSize);
	player->setAnchorPoint(Point::ANCHOR_MIDDLE);
	_duration = 6;
#else
	auto splashImage = ImageView::create("gui/Logo.png");
	addChild(splashImage);
	splashImage->setName("Splash");
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
	LayerColor::onEnter();

	bool on = PlayerPrefs::getInstance().getVolume();
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(on ? GameChoice::getInstance().getMusicVolume() : 0);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(on ? GameChoice::getInstance().getEffectVolume() : 0);

	scheduleOnce(CC_SCHEDULE_SELECTOR(SplashScene::showLogo), _duration);
	scheduleOnce(CC_SCHEDULE_SELECTOR(SplashScene::goMenu), _duration + 3);

	Analytics::getInstance().logEvent("app_start");
	Analytics::getInstance().logEvent("app_open");
}

void SplashScene::onExit()
{
    Layer::onExit();
}

void SplashScene::goMenu(float dt)
{
    Tapligh::getInstance().showAd(Tapligh::UNIT_CODE_1);
	auto scene = TransitionFade::create(.5f, MenuScene::createSceneData());
	Director::getInstance()->replaceScene(scene);
}

void SplashScene::showLogo(float dt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	auto player = static_cast<experimental::ui::VideoPlayer*>(getChildByName("VideoPlayer"));
	player->stop();
	player->removeFromParent();
	//player->runAction(FadeOut::create(.5f));
#else
	auto splashImage = getChildByName("Splash");
	splashImage->removeFromParent();
#endif

	auto logo = ImageView::create("gui/GameLogo.png");
	addChild(logo);
	logo->setPosition(getContentSize() / 2);
	logo->setOpacity(0);
	logo->runAction(FadeIn::create(.5f));
}
