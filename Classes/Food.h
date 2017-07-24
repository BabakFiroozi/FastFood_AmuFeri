#ifndef __FOOD_H__
#define __FOOD_H__

#include <string>

enum class FoodTypes
{
	None = -1,

	Food_1,
	Food_2,
	Food_3,
	Food_4,
	Food_5,

	Count
};


class Food
{
public:
	Food();
	~Food();

	Food(FoodTypes type, int worth, const std::string& name, bool unlocked, const std::string& iconPath);

	FoodTypes getType() const;
	int getWorth() const;
	std::string getName() const;
	bool isUnlocked() const;
	void setUnlocked();
	std::string getIconPath() const;

private:
	FoodTypes _type;
	int _worth;
	std::string _name;
	bool _unlocked;
	std::string _iconPath;

};



#endif // !__FOOD_H__

