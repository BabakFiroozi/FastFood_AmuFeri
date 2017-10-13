#include "FoodFactory.h"
#include "platform/CCFileUtils.h"
#include "json/document.h"

bool FoodFactory::_initialized = false;

FoodFactory::FoodFactory()
{
}

FoodFactory::~FoodFactory()
{
}

FoodFactory& FoodFactory::getInstance()
{
	static FoodFactory instance;
	return instance;
}

void FoodFactory::addFood(const Food& food)
{
	_foodsVec.push_back(food);
}

Food FoodFactory::getFood(FoodTypes type) const
{
	for (auto f : _foodsVec)
		if (f.getType() == type)
			return f;
	return Food();
}

void FoodFactory::unlockFood(FoodTypes type)
{
	auto food = getFood(type);
	food.setUnlocked();
}

void FoodFactory::initialize()
{
	if (_initialized)
		return;

	_initialized = true;

	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile("foods/_foods.json");
	rapidjson::Document doc;
	doc.Parse<0>(str.c_str());

	auto arr = doc["foods"].GetArray();
	for (auto& f : arr)
	{
		FoodTypes type = (FoodTypes)f["type"].GetInt();
		int place = f["place"].GetInt();
		std::string name = f["name"].GetString();
		int worth = f["worth"].GetInt();
		bool unlocked = f["unlocked"].GetBool();
		std::string iconPath = f["iconPath"].GetString();
		Food food(type, place, worth, name, unlocked, iconPath);
		addFood(food);
	}

}
