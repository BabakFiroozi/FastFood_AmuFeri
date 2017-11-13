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

Food::Food(FoodTypes type, int place, int worth, const std::string& name, bool unlocked, const std::string& iconPath, int count, int price)
{
	_type = type;
	_place = place;
	_worth = worth;
	_name = name;
	_unlocked = unlocked;
	_iconPath = iconPath;
	_count = count;
	_price = price;
}

FoodTypes Food::getType() const
{
	return _type;
}

int Food::getWorth() const
{
	return _worth;
}

int Food::getPlace() const
{
	return _place;
}

const std::string& Food::getName() const
{
	return _name;
}

bool Food::isUnlocked() const
{
	return _unlocked;
}

void Food::unlocked()
{
	_unlocked = true;
}

const std::string& Food::getIconPath() const
{
	return _iconPath;
}

int Food::getCount() const
{
	return _count;
}

int Food::getPrice() const
{
	return _price;
}

void Food::charge(int c)
{
	_count += c;
	if (_count > Max_Count)
		_count = Max_Count;
}

void Food::consume(int c)
{
	_count -= c;
	if (_count < 0)
		_count = 0;
}

