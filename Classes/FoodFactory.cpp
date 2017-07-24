#include "FoodFactory.h"
#include "platform/CCFileUtils.h"
#include "json/document.h"


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
	std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile("foods/foods.json");
	rapidjson::Document doc;
	doc.Parse<0>(str.c_str());

	auto arr = doc["foods"].GetArray();
	for (auto& f : arr)
	{
		FoodTypes type = (FoodTypes)f["type"].GetInt();
		std::string name = f["name"].GetString();
		int worth = f["worth"].GetInt();
		bool unlocked = f["unlocked"].GetBool();
		std::string iconPath = f["iconPath"].GetString();
		addFood(Food(type, worth, name, unlocked, iconPath));
	}

}
