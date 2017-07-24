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

	std::string getFontName();
	std::string getString(const std::string& key);

	int getTapsellVideoReward();

	int getTapsellCheckVideoInterval();
	int getTapsellVideoAvailableTime();

	float getGameplayTime();

	std::string getRateUrl();

	std::string getTutorialText(int num);

private:
	rapidjson::Document _document;

};

#endif //__GAME_OPTIONS_H__