#include "GameChoice.h"

USING_NS_CC;

GameChoice::GameChoice()
{
}

GameChoice::~GameChoice()
{
}

GameChoice& GameChoice::getInstance()
{
	static GameChoice instance;
	return instance;
}

void GameChoice::initialize()
{
	std::string str = FileUtils::getInstance()->getStringFromFile("choice.json");
	_document.Parse<0>(str.c_str());
}

std::string GameChoice::getString(const std::string& key)
{
	std::string str = _document[key.c_str()].GetString();
	return str;
}

std::string GameChoice::getFontName()
{
	std::string name = _document["fontName"].GetString();
	return name;
}

int GameChoice::getTapsellVideoReward()
{
	int n = _document["tapsellVideoReward"].GetInt();
	return n;
}

int GameChoice::getTapsellCheckVideoInterval()
{
	int n = _document["tapsellCheckVideoInterval"].GetInt();
	return n;
}

int GameChoice::getTapsellVideoAvailableTime()
{
	int n = _document["tapsellVideoAvailableTime"].GetInt();
	return n;
}

float GameChoice::getGameplayTime()
{
	int n = _document["gameplayTime"].GetDouble();
	return n;
}

std::string GameChoice::getRateUrl()
{
	std::string url = _document["rateUrl"].GetString();
	return url;
}

std::string GameChoice::getTutorialText(int num)
{
	auto& arr = _document["tutorial_paths"];
	if (num > arr.Size() - 1)
		return "";
	std::string text = arr[num].GetString();
	return text;
}
