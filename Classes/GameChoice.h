#ifndef __GAME_OPTIONS_H__
#define __GAME_OPTIONS_H__

#include "cocos2d.h"
#include "json/document.h"


class GameChoice
{
public:
	GameChoice();
	~GameChoice();

	static GameChoice& getInstance();

	void initialize();

	std::string getFontName(bool eng = false) const;
	std::string getString(const std::string& key) const;

	int getTapsellVideoReward();

	int getTapsellCheckVideoInterval();
	int getTapsellVideoAvailableTime();

	float getInitClockTime();
	float getRightFoodTime();
	float getWrongFoodTime();
	float getPackBurgerTime();

	float getMusicVolume() const;
	float getEffectVolume() const;

	std::string getRateUrl() const;

	std::string getTutorialText(int num) const;

	std::vector<std::string> getCredits() const;

private:
	rapidjson::Document _document;

};

#endif //__GAME_OPTIONS_H__