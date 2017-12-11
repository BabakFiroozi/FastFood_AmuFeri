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
	UserDefault::getInstance()->setBoolForKey(key, false);
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

void PlayerPrefs::loadCurrentKitchen() const
{
	const char* key = "currentKitchen";
	KitchenTypes kitchen = (KitchenTypes)UserDefault::getInstance()->getIntegerForKey(key, -1);
	GameUser::getInstance().setCurrentKitchen(kitchen);
}

void PlayerPrefs::saveCurrentKitchen() const
{
	const char* key = "currentKitchen";
	int kitchen = (int)GameUser::getInstance().getCurrentKitchen();
	UserDefault::getInstance()->setIntegerForKey(key, kitchen);
}

void PlayerPrefs::loadUnlockedKitchens() const
{
	const char* key = "unclokedKitchens";
	std::string kitchensStr = UserDefault::getInstance()->getStringForKey(key);
	Inventories::getInstance().initializeUnlockedKitchens(kitchensStr);
}

void PlayerPrefs::saveUnlockedKitchens() const
{
	const char* key = "unclokedKitchens";
	std::string kitchensStr = Inventories::getInstance().serializeUnlockedKitchens();
	UserDefault::getInstance()->setStringForKey(key, kitchensStr);
}

void PlayerPrefs::loadAddedPowerups() const
{
	const char* key = "addedPowerups";
	std::string powerupsStr = UserDefault::getInstance()->getStringForKey(key);
	Inventories::getInstance().initializeAddedPowerups(powerupsStr);
}

void PlayerPrefs::saveAddedPowerups() const
{
	const char* key = "addedPowerups";
	std::string powerupsStr = Inventories::getInstance().serializeAddedPowerups();
	UserDefault::getInstance()->setStringForKey(key, powerupsStr);
}

void PlayerPrefs::setVolume(bool on) const
{
	const char* key = "gameVolume";
	UserDefault::getInstance()->setBoolForKey(key, on);
}

bool PlayerPrefs::getVolume() const
{
	const char* key = "gameVolume";
	bool on = UserDefault::getInstance()->getBoolForKey(key, true);
	return on;
}

void PlayerPrefs::setSandwitch(int count) const
{
	const char* key = "sandwitch";
	int c = UserDefault::getInstance()->getIntegerForKey(key, 0);
	if (count > c)
		UserDefault::getInstance()->setIntegerForKey(key, count);
}

int PlayerPrefs::getSandwitch() const
{
	const char* key = "sandwitch";
	int c = UserDefault::getInstance()->getIntegerForKey(key, 0);
	return c;
}
