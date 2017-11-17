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
	FishFillet,
	Tomato,

	Count
};


class Food
{
public:
	static const int Max_Count;

	Food();
	~Food();

	Food(FoodTypes type, int place, int worth, const std::string& name, bool unlocked, const std::string& iconPath, int count, int price, int height);

	FoodTypes getType() const;
	int getWorth() const;
	int getPlace() const;
	const std::string& getName() const;
	bool isUnlocked() const;
	void unlocked();
	const std::string& getIconPath() const;
	int getCount() const;
	void consume(int c);
	int getPrice() const;
	int getHeight() const;
	void charge(int c);

private:
	FoodTypes _type;
	int _place;
	int _worth;
	std::string _name;
	bool _unlocked;
	std::string _iconPath;
	int _count;
	int _price;
	int _height;
};



#endif // __FOOD_H__

