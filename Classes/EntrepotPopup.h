#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "json/document.h"
#include "FoodFactory.h"

class EntrepotPopup : public cocos2d::ui::Layout
{
public:
	static EntrepotPopup* create();

	EntrepotPopup();
	virtual ~EntrepotPopup();

protected:

	bool init() override;
	void onEnter() override;

private:
	void updateEntrepot();
	void buttonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType);

	cocos2d::ui::ListView* _listView;

};

