#include "Inventories.h"
#include "PlayerPrefs.h"
#include "json/stringbuffer.h"
#include "json/writer.h"


USING_NS_CC;

bool Inventories::_initialized = false;

Inventories::Inventories()
{
}

Inventories::~Inventories()
{
}

Inventories& Inventories::getInstance()
{
	static Inventories instance;
	return instance;
}

std::vector<KitchenPtr> Inventories::getAllKitchens()
{
	return _allKitchens;
}

std::vector<PowerupPtr> Inventories::getAllPowerups()
{
	return _allPowerups;
}

KitchenPtr Inventories::getKitchenByType(KitchenTypes type)
{
	for (auto c : _allKitchens)
		if (c->getType() == type)
			return c;
	return nullptr;
}

PowerupPtr Inventories::getPowerupByType(PowerupTypes type)
{
	for (auto p : _allPowerups)
		if (p->getType() == type)
			return p;
	return nullptr;
}

void Inventories::addPowerup(PowerupTypes powerupType)
{
	if (isPowerupAdded(powerupType))
		return;
	_addedPowerups.push_back(powerupType);
}

bool Inventories::isPowerupAdded(PowerupTypes powerupType)
{
	bool found = std::find(_addedPowerups.begin(), _addedPowerups.end(), powerupType) != _addedPowerups.end();
	return found;
}

void Inventories::unlockKitchen(KitchenTypes kitchenType)
{
	if (isKitchenUnlocked(kitchenType))
		return;
	_unlockedKitchens.push_back(kitchenType);
}

bool Inventories::isKitchenUnlocked(KitchenTypes kitchenType)
{
	bool found = std::find(_unlockedKitchens.begin(), _unlockedKitchens.end(), kitchenType) != _unlockedKitchens.end();
	return found;
}


void Inventories::initialize(const std::string& data)
{
	if (_initialized)
		return;

	_initialized = true;

	rapidjson::Document doc;

	doc.Parse<0>(data.c_str());

	auto kitchensAr = doc["kitchens"].GetArray();
	for (auto& c : kitchensAr)
	{
		KitchenTypes type = (KitchenTypes)c["type"].GetInt();
		std::string name = c["name"].GetString();
		int value = c["value"].GetInt();
		Kitchen kitchen(type, name, value);
		_allKitchens.push_back(std::make_shared<Kitchen>(kitchen));
	}	

	auto powerupsArr = doc["powerups"].GetArray();
	for (auto& c : powerupsArr)
	{
		PowerupTypes type = (PowerupTypes)c["type"].GetInt();
		std::string name = c["name"].GetString();
		int value = c["value"].GetInt();
		int amount = c["amount"].GetInt();
		Powerup powerup(type, name, value, amount);
		_allPowerups.push_back(std::make_shared<Powerup>(powerup));
	}
}

std::string Inventories::serializeUnlockedKitchens()
{
	rapidjson::Document doc;
	doc.SetObject();
	auto& allocator = doc.GetAllocator();

	rapidjson::Value kitchesArr(rapidjson::kArrayType);
	for (auto k : _unlockedKitchens)
		kitchesArr.PushBack((int)k, allocator);
	doc.AddMember("unlockedKitchens", kitchesArr, allocator);

	rapidjson::StringBuffer strBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strBuffer);
	doc.Accept(writer);

	return strBuffer.GetString();
}

void Inventories::initializeUnlockedKitchens(const std::string& data)
{
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());

	auto arr = doc["unlockedKitchens"].GetArray();
	for (auto& k : arr)
	{
		KitchenTypes kitchen = (KitchenTypes)k.GetInt();
		_unlockedKitchens.push_back(kitchen);
	}
}

std::string Inventories::serializeAddedPowerups()
{
	rapidjson::Document doc;
	doc.SetObject();
	auto& allocator = doc.GetAllocator();

	rapidjson::Value kitchesArr(rapidjson::kArrayType);
	for (auto k : _unlockedKitchens)
		kitchesArr.PushBack((int)k, allocator);
	doc.AddMember("addedPowerups", kitchesArr, allocator);

	rapidjson::StringBuffer strBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strBuffer);
	doc.Accept(writer);

	return strBuffer.GetString();
}

void Inventories::initializeAddedPowerups(const std::string& data)
{
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());

	auto arr = doc["addedPowerups"].GetArray();
	for (auto& p : arr)
	{
		PowerupTypes powerup = (PowerupTypes)p.GetInt();
		_addedPowerups.push_back(powerup);
	}
}

