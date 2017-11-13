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

std::vector<std::shared_ptr<Kitchen>> Inventories::getAllKitchens()
{
	return _kitchens;
}

std::vector<std::shared_ptr<Powerup>> Inventories::getAllPowerups()
{
	return _powerups;
}

std::shared_ptr<Kitchen> Inventories::getKitchenByType(KitchenTypes type)
{
	for (auto c : _kitchens)
		if (c->getType() == type)
			return c;
	return nullptr;
}

std::shared_ptr<Powerup> Inventories::getPowerupByType(PowerupTypes type)
{
	for (auto p : _powerups)
		if (p->getType() == type)
			return p;
	return nullptr;
}

void Inventories::addPowerup(PowerupTypes powerupType)
{
}

void Inventories::usePowerup(PowerupTypes powerupType)
{
}

void Inventories::unlockKitchen(KitchenTypes kitchenType)
{
	if (kitchenUnlocked(kitchenType))
		return;
	_unlockedKitchens.push_back(kitchenType);
}

bool Inventories::kitchenUnlocked(KitchenTypes kitchenType)
{
	return std::find(_unlockedKitchens.begin(), _unlockedKitchens.end(), kitchenType) != _unlockedKitchens.end();
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
		_kitchens.push_back(std::make_shared<Kitchen>(kitchen));
	}	

	auto powerupsArr = doc["powerups"].GetArray();
	for (auto& c : powerupsArr)
	{
		PowerupTypes type = (PowerupTypes)c["type"].GetInt();
		std::string name = c["name"].GetString();
		int value = c["value"].GetInt();
		Powerup powerup(type, name, value);
		_powerups.push_back(std::make_shared<Powerup>(powerup));
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

