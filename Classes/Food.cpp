#include "Food.h"

Food::~Food()
{
}

Food::Food()
{
	_type = FoodTypes::None;
	_name = "";
	_worth = -1;
	_unlocked = false;
}

Food::Food(FoodTypes type, int worth, const std::string& name, bool unlocked, const std::string& iconPath)
{
	_type = type;
	_worth = worth;
	_name = name;
	_unlocked = unlocked;
	_iconPath = iconPath;
}

FoodTypes Food::getType() const
{
	return _type;
}

int Food::getWorth() const
{
	return _worth;
}

std::string Food::getName() const
{
	return _name;
}

bool Food::isUnlocked() const
{
	return _unlocked;
}

void Food::setUnlocked()
{
	_unlocked = true;
}

std::string Food::getIconPath() const
{
	return _iconPath;
}

