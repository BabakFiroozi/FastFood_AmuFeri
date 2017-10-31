#pragma once

#include "FoodFactory.h"

class PlayerPrefs
{
public:
	PlayerPrefs();
	~PlayerPrefs();

	static PlayerPrefs& getInstance();

	void loadFoods() const;
	void saveFoods() const;

	bool isFirstRun() const;
	void doneFirstRun() const;

	void loadCoin() const;
	void saveCoin() const;

private:


};

