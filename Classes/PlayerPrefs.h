#pragma once

#include "FoodFactory.h"
#include "Inventories.h"

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

	void loadCurrentKitchen() const;
	void saveCurrentKitchen() const;

	void loadUnlockedKitchens() const;
	void saveUnlockedKitchens() const;

	void setVolume(bool on) const;
	bool getVolume() const;

private:
};

