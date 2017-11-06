#pragma once

enum class PowerupTypes
{
	None = -1,

	Count
};

enum class CookTypes
{
	None = -1,

	Count
};

class Powerup
{
public:
	Powerup() {};
	~Powerup() {};
	Powerup(PowerupTypes type, int value)
	{
		_type = type;
		_value = value;
	}

	PowerupTypes getType() const
	{
		return _type;
	}

	int getValue() const
	{
		return _value;
	}

private:
	PowerupTypes _type;
	int _value;

};

class Cook
{
public:
	Cook() {};
	~Cook() {};
	Cook(CookTypes type, int value)
	{
		_type = type;
		_value = value;
	}

	CookTypes getType() const
	{
		return _type;
	}

	int getValue() const
	{
		return _value;
	}

private:
	CookTypes _type;
	int _value;

};
