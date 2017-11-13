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

	Count
};

class Kitchen
{
public:
	Kitchen() {};
	~Kitchen() {};
	Kitchen(KitchenTypes type, const std::string& name, int value)
	{
		_type = type;
		_name = name;
		_value = value;
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
	Powerup(PowerupTypes type, const std::string& name, int value)
	{
		_type = type;
		_name = name;
		_value = value;
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

private:
	PowerupTypes _type;
	std::string _name;
	int _value;
};


class Inventories
{
public:
	Inventories();
	~Inventories();

	static Inventories& getInstance();

	std::vector<std::shared_ptr<Kitchen>> getAllKitchens();
	std::vector<std::shared_ptr<Powerup>> getAllPowerups();
	std::shared_ptr<Kitchen> getKitchenByType(KitchenTypes type);
	std::shared_ptr<Powerup> getPowerupByType(PowerupTypes type);

	void usePowerup(PowerupTypes powerupType);
	void addPowerup(PowerupTypes powerupType);

	void unlockKitchen(KitchenTypes kitchenType);
	bool kitchenUnlocked(KitchenTypes kitchenType);

	void initialize(const std::string& data);

	std::string serializeUnlockedKitchens();
	void initializeUnlockedKitchens(const std::string& data);

private:
	static bool _initialized;

	std::vector<std::shared_ptr<Kitchen>> _kitchens;
	std::vector<std::shared_ptr<Powerup>> _powerups;

	std::vector<KitchenTypes> _unlockedKitchens;
};