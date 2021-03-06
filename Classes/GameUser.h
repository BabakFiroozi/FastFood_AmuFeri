#pragma once

#include <string>


class GameUser
{
public:
	const int Max_Coin = 99999;

	GameUser()
	{
	}
	~GameUser()
	{
	}

	static GameUser& getInstance()
	{
		static GameUser instance;
		return instance;
	}

	int getCoin() const
	{
		return _coin;
	}
	void setCoin(int c)
	{
		_coin = c;
	}
	void addCoin(int c)
	{
		_coin += c;
		if (_coin > Max_Coin)
			_coin = Max_Coin;
	}

	const std::string& getName() const
	{
		return _name;
	}
	void setUserName(const std::string& name)
	{
		_name = name;
	}

	KitchenTypes getCurrentKitchen() const
	{
		return _currentKitchen;
	}

	void setCurrentKitchen(KitchenTypes kitchenType)
	{
		_currentKitchen = kitchenType;
	}

protected:

private:
	int _coin;
	std::string _name;
	KitchenTypes _currentKitchen;

};
