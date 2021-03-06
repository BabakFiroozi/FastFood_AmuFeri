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

	void loadAddedPowerups() const;
	void saveAddedPowerups() const;

	void setVolume(bool on) const;
	bool getVolume() const;

	void setSandwitch(int count) const;
	int  getSandwitch() const;

	void addSandwitchTotal(int count) const;
	int  getSandwitchTotal() const;

	bool isTutorialFinished(int part) const;
	void finishTutrial(int part) const;

private:
};

