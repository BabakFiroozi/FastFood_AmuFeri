#ifndef __FOOD_FACTROY_H__
#define __FOOD_FACTROY_H__

#include <string>
#include <vector>
#include "Food.h"
#include <memory>

typedef std::shared_ptr<Food> FoodPtr;

class FoodFactory
{
public:
	FoodFactory();
	~FoodFactory();

	static FoodFactory& getInstance();

	void addFood(FoodPtr food);
	FoodPtr getFood(FoodTypes type);
	void unlockFood(FoodTypes type);
	void initialize(const std::string& foodsStr);
	void consumeFood(FoodTypes type, int count = 1);
	void chargeFood(FoodTypes type, int count);
	std::string serialize();

	std::vector<FoodPtr> getAllFoods();

private:
	std::vector<FoodPtr> _foodsVec;

	static bool _initialized;
};

#endif//__FOOD_FACTROY_H__
