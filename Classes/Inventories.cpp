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

std::vector<PowerupPtr> Inventories::getAllPowerups()
{
	return _allPowerups;
}

PowerupPtr Inventories::getPowerupByType(PowerupTypes type)
{
	for (auto p : _allPowerups)
		if (p->getType() == type)
			return p;
	return nullptr;
}

void Inventories::incPowerup(PowerupTypes powerupType)
{
	_addedPowerups[powerupType] += 1;
}

void Inventories::decPowerup(PowerupTypes powerupType)
{
	if (!hasPowerup(powerupType))
		return;
	_addedPowerups[powerupType] -= 1;
}

bool Inventories::hasPowerup(PowerupTypes powerupType)
{
	bool has = _addedPowerups[powerupType] > 0;
	return has;
}

std::vector<KitchenPtr> Inventories::getAllKitchens()
{
	return _allKitchens;
}

KitchenPtr Inventories::getKitchenByType(KitchenTypes type)
{
	for (auto k : _allKitchens)
		if (k->getType() == type)
			return k;
	return nullptr;
}

void Inventories::unlockKitchen(KitchenTypes kitchenType)
{
	if (kitchenUnlocked(kitchenType))
		return;
	_unlockedKitchens.push_back(kitchenType);
}

bool Inventories::kitchenUnlocked(KitchenTypes kitchenType)
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

	rapidjson::Value powerupsArr(rapidjson::kArrayType);
	for (auto k : _addedPowerups)
	{
		rapidjson::Value obj(rapidjson::kObjectType);
		obj.AddMember("type", (int)k.first, allocator);
		obj.AddMember("count", (int)k.second, allocator);
		powerupsArr.PushBack(obj, allocator);
	}
	doc.AddMember("addedPowerups", powerupsArr, allocator);

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
		PowerupTypes type = (PowerupTypes)p["type"].GetInt();
		int count = p["count"].GetInt();
		_addedPowerups[type] = count;
	}
}

int Inventories::getPowerupsCount(PowerupTypes type)
{
	int count = 0;
	if (hasPowerup(type))
		count = _addedPowerups[type];
	return count;
}

