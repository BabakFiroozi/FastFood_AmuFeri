#ifndef __FOOF_FACTROY_H__
#define __FOOF_FACTROY_H__

#include <string>
#include <vector>
#include "Food.h"

class FoodFactory
{
public:
	FoodFactory();
	~FoodFactory();

	static FoodFactory& getInstance();

	void addFood(const Food& food);
	Food getFood(FoodTypes type) const;
	void unlockFood(FoodTypes type);
	void initialize();

private:
	std::vector<Food> _foodsVec;
};

#endif//__FOOF_FACTROY_H__
