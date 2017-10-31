#include "PlayerPrefs.h"
#include "cocos2d.h"
#include "GameUser.h"

USING_NS_CC;

PlayerPrefs::PlayerPrefs()
{
}

PlayerPrefs::~PlayerPrefs()
{
}

PlayerPrefs& PlayerPrefs::getInstance()
{
	static PlayerPrefs instance;
	return instance;
}

void PlayerPrefs::loadFoods() const
{
	const char* key = "foodFactory";
	std::string str = UserDefault::getInstance()->getStringForKey(key);
	FoodFactory::getInstance().initialize(str);
}

void PlayerPrefs::saveFoods() const
{
	const char* key = "foodFactory";
	std::string foodsStr = FoodFactory::getInstance().serialize();
	UserDefault::getInstance()->setStringForKey(key, foodsStr);
}

bool PlayerPrefs::isFirstRun() const
{
	const char* key = "isFirstRun";
	return UserDefault::getInstance()->getBoolForKey(key, true);
}

void PlayerPrefs::doneFirstRun() const
{
	const char* key = "isFirstRun";
	UserDefault::getInstance()->setBoolForKey(key, true);
}

void PlayerPrefs::loadCoin() const
{
	const char* key = "coinsCount";
	GameUser::getInstance().setCoin(UserDefault::getInstance()->getIntegerForKey(key));
}

void PlayerPrefs::saveCoin() const
{
	const char* key = "coinsCount";
	UserDefault::getInstance()->setIntegerForKey(key, GameUser::getInstance().getCoin());
}
