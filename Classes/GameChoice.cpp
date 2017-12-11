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

std::string GameChoice::getString(const std::string& key) const
{
	std::string str = _document[key.c_str()].GetString();
	return str;
}

std::string GameChoice::getFontName(bool eng /*= false*/) const
{
	std::string name = !eng ? _document["fontName"].GetString() : _document["fontNameEng"].GetString();
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

float GameChoice::getInitClockTime()
{
	float n = _document["initClockTime"].GetDouble();
	return n;
}

float GameChoice::getRightFoodTime()
{
	float n = _document["rigtFoodTime"].GetDouble();
	return n;
}

float GameChoice::getWrongFoodTime()
{
	float n = _document["wrongFoodTime"].GetDouble();
	return n;
}

float GameChoice::getPackBurgerTime()
{
	float n = _document["musicVolume"].GetDouble();
	return n;
}

float GameChoice::getMusicVolume() const
{
	float n = _document["rigtFoodTime"].GetDouble();
	return n;
}

float GameChoice::getEffectVolume() const
{
	float n = _document["effectVolume"].GetDouble();
	return n;
}

std::string GameChoice::getRateUrl() const
{
	std::string url = _document["rateUrl"].GetString();
	return url;
}

std::string GameChoice::getTutorialText(int num) const
{
	auto arr = _document["tutorial_paths"].GetArray();
	if (num > arr.Size() - 1)
		return "";
	std::string text = arr[num].GetString();
	return text;
}

std::vector<std::string> GameChoice::getCredits() const
{
	std::vector<std::string> vec;
	auto arr = _document["credtisList"].GetArray();
	for (int i = 0; i < arr.Size(); ++i)
		vec.push_back(arr[i].GetString());
	return vec;

}
