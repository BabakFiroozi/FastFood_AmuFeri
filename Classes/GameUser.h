#pragma once

#include <string>
//#include <vector>

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

	std::string getName()
	{
		return _name;
	}
	void setUserName(const std::string& name)
	{
		_name = name;
	}

protected:

private:
	int _coin;
	std::string _name;

};
