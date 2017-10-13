#ifndef __FOOD_H__
#define __FOOD_H__

#include <string>

enum class FoodTypes
{
	None = -1,

	Bread_Neath,
	Bread_Top,
	Sauce,
	Meat,
	Cucumber,
	Onion,
	Steak,
	Lettuce,
	CheeseGoda,

	Count
};


class Food
{
public:
	Food();
	~Food();

	Food(FoodTypes type, int place, int worth, const std::string& name, bool unlocked, const std::string& iconPath);

	FoodTypes getType() const;
	int getWorth() const;
	int getPlace() const;
	std::string getName() const;
	bool isUnlocked() const;
	void setUnlocked();
	std::string getIconPath() const;

private:
	FoodTypes _type;
	int _place;
	int _worth;
	std::string _name;
	bool _unlocked;
	std::string _iconPath;
};



#endif // __FOOD_H__

