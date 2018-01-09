#pragma once

#include "General.h"

class StandbyLayout : public cocos2d::ui::Layout
{
public:
	StandbyLayout();
	virtual ~StandbyLayout();

	CREATE_FUNC_DATA(StandbyLayout);

	static StandbyLayout* createData(int snakeSize)
	{
		cocos2d::ValueMap initData;
		initData["snakeSize"] = snakeSize;
		return create(initData);
	}

	typedef std::function<void()> ReadyCallback;

	void setReadyCallback(const ReadyCallback& callback);

protected:
	virtual bool init(cocos2d::ValueMap& initData);

private:
	void buttonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType);

	cocos2d::Vect _visibleOrigin;
	cocos2d::Size _visibleSize;

	ReadyCallback _readyCallback;
};
