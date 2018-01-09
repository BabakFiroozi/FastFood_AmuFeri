
#include "StandbyLayout.h"

USING_NS_CC;
using namespace cocos2d::ui;

StandbyLayout::StandbyLayout()
{
}

StandbyLayout::~StandbyLayout()
{
}

bool StandbyLayout::init(ValueMap& initData)
{
	if (!Layout::init())
		return false;

	_visibleOrigin = Director::getInstance()->getVisibleOrigin();
	_visibleSize = Director::getInstance()->getVisibleSize();

	Vect layoutCenter = _visibleSize / 2;

	setContentSize(_visibleSize);

	setSwallowTouches(true);
	setTouchEnabled(true);

	setBackGroundColor(Color3B::BLACK);
	setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	setBackGroundColorOpacity(100);

	auto readyButton = Button::create("gui/ready.png");
	addChild(readyButton);
	readyButton->setPosition(getContentSize() / 2 + Size(0, 100));

	readyButton->addTouchEventListener([=](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
	{
		if (_readyCallback != nullptr)
			_readyCallback();
		removeFromParent();
	});

	return true;
}

void StandbyLayout::buttonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == Widget::TouchEventType::ENDED)
	{
	}
}

void StandbyLayout::setReadyCallback(const ReadyCallback& callback)
{
	_readyCallback = callback;
}
