#ifndef __SHOP_SCENE_H__
#define __SHOP_SCENE_H__

#include "cocos2d.h"
#include "General.h"

enum class ShopTypes
{
	None = -1,
	Food,
	Power,
	Cook,
	Coin,
	Count
};


class ShopScene : public cocos2d::Layer
{
public:
	ShopScene();
	virtual ~ShopScene();

	CREATE_FUNC_DATA(ShopScene);
	CREATESCENE_FUNC_DATA();

	static cocos2d::Scene* createSceneData(ShopTypes shopType)
	{
		cocos2d::ValueMap initData;
		initData["shopType"] = (int)shopType;
		return createScene(initData);
	}

protected:
	virtual bool init(cocos2d::ValueMap& initData);
	void onEnter() override;
	void update(float dt) override;

private:

	struct ShopData
	{
		ShopTypes shopType;
		int itemNumber;

		ShopData()
		{
		}

		ShopData(ShopTypes shopType, int itemNumber)
		{
			this->shopType = shopType;
			this->itemNumber = itemNumber;
		}
	};

	void buttonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType);
	void buyButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType);
	void tabButtonCallback(cocos2d::ui::RadioButton* sender, cocos2d::ui::RadioButton::EventType eventType);

	void showTab(ShopTypes shopType);

	ShopTypes _defaultShopType;

	cocos2d::Vect _visibleOrigin;
	cocos2d::Size _visibleSize;
	cocos2d::ui::Button* _backButton;

	cocos2d::ui::ListView* _shopListView;

	std::map<cocos2d::ui::Button*, ShopData> _shopDataMap;
};

#endif //__SHOP_SCENE_H__
