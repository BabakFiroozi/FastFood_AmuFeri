#pragma once

#include "cocos2d.h"
#include "json/document.h"


enum class KitchenTypes
{
	None = -1,

	Kitchen_1,
	Kitchen_2,
	Kitchen_3,

	Count
};

enum class PowerupTypes
{
	None = -1,

	RichCustomer,
	HeadLight,
	CaptainCook,
	BellClock,

	Count
};

class Kitchen
{
public:
	Kitchen() {};
	~Kitchen() {};
	Kitchen(KitchenTypes type, const std::string& name, int value): _type(type), _name(name), _value(value)
	{
	}

	KitchenTypes getType() const
	{
		return _type;
	}

	int getValue() const
	{
		return _value;
	}

	const std::string& getName() const
	{
		return _name;
	}	
private:
	KitchenTypes _type;
	std::string _name;
	int _value;

};

class Powerup
{
public:
	Powerup() {};
	~Powerup() {};
	Powerup(PowerupTypes type, const std::string& name, int value, int amount): _type(type), _name(name), _value(value), _amount(amount)
	{
	}

	PowerupTypes getType() const
	{
		return _type;
	}

	int getValue() const
	{
		return _value;
	}

	const std::string& getName() const
	{
		return _name;
	}

	int getAmount() const
	{
		return _amount;
	}

	void addAmount()
	{
		_amount++;
	}

private:
	PowerupTypes _type;
	std::string _name;
	int _value;
	int _amount;
};

typedef std::shared_ptr<Kitchen> KitchenPtr;
typedef std::shared_ptr<Powerup> PowerupPtr;


class Inventories
{
public:
	Inventories();
	~Inventories();

	static Inventories& getInstance();

	std::vector<KitchenPtr> getAllKitchens();
	std::vector<PowerupPtr> getAllPowerups();
	KitchenPtr getKitchenByType(KitchenTypes type);
	PowerupPtr getPowerupByType(PowerupTypes type);

	void incPowerup(PowerupTypes powerupType);
	void decPowerup(PowerupTypes powerupType);
	bool hasPowerup(PowerupTypes powerupType);

	void unlockKitchen(KitchenTypes kitchenType);
	bool isKitchenUnlocked(KitchenTypes kitchenType);

	void initialize(const std::string& data);

	std::string serializeUnlockedKitchens();
	void initializeUnlockedKitchens(const std::string& data);

	std::string serializeAddedPowerups();
	void initializeAddedPowerups(const std::string& data);

	int getPowerupsCount(PowerupTypes type);

private:
	static bool _initialized;

	std::vector<KitchenPtr> _allKitchens;
	std::vector<PowerupPtr> _allPowerups;

	std::vector<KitchenTypes> _unlockedKitchens;
	std::map<PowerupTypes, int> _addedPowerups;
};