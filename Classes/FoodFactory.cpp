#include "FoodFactory.h"
#include "platform/CCFileUtils.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

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

void FoodFactory::addFood(std::shared_ptr<Food> food)
{
	_foodsVec.push_back(food);
}


std::shared_ptr<Food> FoodFactory::getFood(FoodTypes type)
{
	for (auto f : _foodsVec)
		if (f->getType() == type)
			return f;
	return nullptr;
}

void FoodFactory::unlockFood(FoodTypes type)
{
	auto food = getFood(type);
	food->unlocked();
}

void FoodFactory::initialize(const std::string& foodsStr)
{
	if (_initialized)
		return;

	_initialized = true;

	rapidjson::Document doc;
	doc.Parse<0>(foodsStr.c_str());

	auto arr = doc["foods"].GetArray();
	for (auto& f : arr)
	{
		FoodTypes type = (FoodTypes)f["type"].GetInt();
		int place = f["place"].GetInt();
		std::string name = f["name"].GetString();
		int worth = f["worth"].GetInt();
		bool unlocked = f["unlocked"].GetBool();
		std::string iconPath = f["iconPath"].GetString();
		int count = f["count"].GetInt();
		int price = f["price"].GetInt();
		Food food(type, place, worth, name, unlocked, iconPath, count, price);
		addFood(std::make_shared<Food>(food));
	}
}

std::string FoodFactory::serialize()
{
	rapidjson::Document doc;
	doc.SetObject();
	auto& allocator = doc.GetAllocator();

	rapidjson::Value foodsArray(rapidjson::kArrayType);
	for (auto f : _foodsVec)
	{
		rapidjson::Value food(rapidjson::kObjectType);
		food.AddMember("type", (int)f->getType(), allocator);
		food.AddMember("place", f->getPlace(), allocator);
		food.AddMember("name", rapidjson::StringRef(f->getName().c_str()), allocator);
		food.AddMember("worth", f->getWorth(), allocator);
		food.AddMember("unlocked", f->isUnlocked(), allocator);
		food.AddMember("iconPath", rapidjson::StringRef(f->getIconPath().c_str()), allocator);
		food.AddMember("count", f->getCount(), allocator);
		food.AddMember("price", f->getPrice(), allocator);
		foodsArray.PushBack(food, allocator);
	}
	doc.AddMember("foods", foodsArray, allocator);

	rapidjson::StringBuffer strBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strBuffer);
	doc.Accept(writer);

	return strBuffer.GetString();
}

void FoodFactory::consumeFood(FoodTypes type, int count)
{
	auto food = getFood(type);
	food->consume(count);
}

void FoodFactory::chargeFood(FoodTypes type, int count)
{
	auto food = getFood(type);
	food->charge(count);
}

std::vector<std::shared_ptr<Food>> FoodFactory::getAllFoods()
{
	return _foodsVec;
}