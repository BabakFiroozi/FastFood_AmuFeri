
#include "StandbyLayout.h"
#include "Inventories.h"
#include "GameChoice.h"
#include "FoodFactory.h"
#include "ShopScene.h"
#include "PlayerPrefs.h"
#include "Tapligh.h"
#include "GameplayScene.h"

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

	if (PlayerPrefs::getInstance().isTutorialFinished(2))
	{
		auto backg = ImageView::create("gui/menu/standby.png");
		addChild(backg);
		backg->setPosition(getContentSize() / 2 + Size(0, 100));

		_pagesVec.pushBack(backg);

		Vect backgCenter = backg->getContentSize() / 2;

		Vect posOffset = backgCenter + Vect(-150, 100);

		auto titleText = Text::create(
				GameChoice::getInstance().getString("TEXT_POWERUPS_ADDED"),
				GameChoice::getInstance().getFontName(), 60);
		backg->addChild(titleText);
		titleText->setPosition(backgCenter + Vect(0, 330));
		titleText->setTextColor(Color4B::YELLOW);
		titleText->enableOutline(Color4B::GRAY, 2);

		std::vector<Text*> _countTextsVec;

		for (int i = (int) PowerupTypes::None + 1; i < (int) PowerupTypes::Count; ++i) {
			PowerupTypes type = (PowerupTypes) i;
			auto powerup = Inventories::getInstance().getPowerupByType(type);
			auto powerupImage = ImageView::create(
					StringUtils::format("gui/shop/powerups/powerup_%d.png", i + 1));
			backg->addChild(powerupImage);
			if (i > 0) {
				posOffset.x += (i % 2 == 1) ? 300 : 0;
				posOffset.x -= (i % 2 == 0) ? 300 : 0;
				posOffset.y -= (i % 2 == 0) ? 300 : 0;
			}
			powerupImage->setPosition(posOffset);
			powerupImage->setScale(.7f);

			int powerupsCount = Inventories::getInstance().getPowerupsCount(type);
			auto countText = Text::create(StringUtils::toString(powerupsCount),
										  GameChoice::getInstance().getFontName(), 60);
			backg->addChild(countText);
			countText->setPosition(powerupImage->getPosition() + Vect(0, -105));
			countText->enableOutline(Color4B::GRAY);
			countText->setTextColor(Color4B::YELLOW);
			_countTextsVec.push_back(countText);

			auto nameText = Text::create(
					Inventories::getInstance().getPowerupByType(type)->getName(),
					GameChoice::getInstance().getFontName(), 40);
			backg->addChild(nameText);
			nameText->setPosition(powerupImage->getPosition() + Vect(0, 100));
			nameText->enableOutline(Color4B::GRAY);
		}

		if(GameplayScene::s_AdLoadedInGame)
		{
            GameplayScene::s_AdLoadedInGame = false;
			auto adButton = Button::create("gui/videoAdButton.png");
			adButton->setScale(.6f);
			backg->addChild(adButton);
			adButton->setPosition(backg->getContentSize() / 2 + Size(0, 450));
			adButton->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(.3f, .7f), ScaleTo::create(.3f, .6f))));

			auto adMessage = Text::create(GameChoice::getInstance().getString("TEXT_SEE_VIDEO_GET_POWER"),
										  GameChoice::getInstance().getFontName(), 40);
			backg->addChild(adMessage);
			adMessage->setPosition(adButton->getPosition() + Vect(0, 80));
			adMessage->enableOutline(Color4B::GRAY);

			adButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType eventType) {
				Tapligh::getInstance().showAd(Tapligh::UNIT_CODE_2);
				adButton->setVisible(false);
				adMessage->setVisible(false);
				//gove powerup
				for(int p = 0; p < (int)PowerupTypes::Count; ++p)
				{
					Inventories::getInstance().incPowerup((PowerupTypes)p);
					_countTextsVec[p]->setString(StringUtils::toString(Inventories::getInstance().getPowerupsCount((PowerupTypes)p)));
				}
			});

//			Tapligh::getInstance().setOnAdResultFuncCallback([=](int result, const std::string& token) {
//				if (result == (int)Tapligh::ADResult::adViewCompletely)
//				{
//					for(int p = 0; p < (int)PowerupTypes::Count; ++p)
//					{
//						Inventories::getInstance().incPowerup((PowerupTypes)p);
//						_countTextsVec[p]->setString(StringUtils::toString(Inventories::getInstance().getPowerupsCount((PowerupTypes)p)));
//					}
//				}
//			});

		}
	}

	std::vector<FoodPtr> lowFoodsVec;
	for (auto food : FoodFactory::getInstance().getAllFoods())
	{
		if (food->isUnlocked() && food->getCount() < 10)
			lowFoodsVec.push_back(food);
		if (lowFoodsVec.size() == 6)
			break;
	}

	if (lowFoodsVec.size() > 0)
	{
		auto backg = ImageView::create("gui/menu/standby.png");
		addChild(backg);
		backg->setPosition(getContentSize() / 2 + Size(0, 100));

		_pagesVec.pushBack(backg);

		Vect backgCenter = backg->getContentSize() / 2;

		Vect posOffset = backgCenter + Vect(-150, 180);

		auto titleText = Text::create(GameChoice::getInstance().getString("TEXT_LOW_FOODS"), GameChoice::getInstance().getFontName(), 60);
		backg->addChild(titleText);
		titleText->setPosition(backgCenter + Vect(0, 330));
		titleText->setTextColor(Color4B::YELLOW);
		titleText->enableOutline(Color4B::GRAY, 2);

		auto shopButton = Button::create("gui/menu/shopBadge.png");
		backg->addChild(shopButton);
		shopButton->setPosition(backgCenter + Vect(0, 450));
		shopButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType eventType) {
			if (eventType == Widget::TouchEventType::ENDED)
			{
				auto scene = TransitionFade::create(.5f, ShopScene::createSceneData(ShopTypes::Food));
				Director::getInstance()->replaceScene(scene);
			}
		});

		for (int i = 0; i < (int)lowFoodsVec.size(); ++i)
		{
			auto food = lowFoodsVec.at(i);

			auto foodImage = ImageView::create(food->getIconPath());
			backg->addChild(foodImage);
			if (i > 0)
			{
				posOffset.x += (i % 2 == 1) ? 300 : 0;
				posOffset.x -= (i % 2 == 0) ? 300 : 0;
				posOffset.y -= (i % 2 == 0) ? 220 : 0;
			}
			foodImage->setPosition(posOffset);
			foodImage->setScale(.3f);

			auto countText = Text::create(StringUtils::toString(food->getCount()), GameChoice::getInstance().getFontName(), 60);
			backg->addChild(countText);
			countText->setPosition(foodImage->getPosition() + Vect(0, -75));
			countText->enableOutline(Color4B::GRAY);
			countText->setTextColor(Color4B::YELLOW);

			auto nameText = Text::create(food->getName(), GameChoice::getInstance().getFontName(), 40);
			backg->addChild(nameText);
			nameText->setPosition(foodImage->getPosition() + Vect(0, 65));
			nameText->enableOutline(Color4B::GRAY);
		}
	}
	
	auto readyButton = Button::create("gui/ready.png");
	readyButton->setZoomScale(0);
	addChild(readyButton);
	readyButton->setPosition(getContentSize() / 2 + Size(0, -380));
	readyButton->addTouchEventListener(CC_CALLBACK_2(StandbyLayout::buttonCallback, this));
	readyButton->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(.25f, 1.1f), ScaleTo::create(.25f, 1.0f))));

	for (auto page : _pagesVec)
		page->setVisible(_pagesVec.getIndex(page) == _pageIndex);

	if (_pagesVec.size() > 1)
	{
		readyButton->setVisible(false);
		auto nextButton = Button::create("gui/nextButton.png");
		addChild(nextButton);
		nextButton->setPosition(getContentSize() / 2 + Size(0, -380));
		nextButton->addTouchEventListener([=](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
		{
			if (eventType == Widget::TouchEventType::ENDED)
			{
				if (_pageIndex < _pagesVec.size() - 1)
				{
					auto page = _pagesVec.at(_pageIndex);
					page->runAction(MoveBy::create(.3f, Vect(1000, 0)));
					_pageIndex++;
					page = _pagesVec.at(_pageIndex);
					page->setVisible(true);
					page->setPositionX(page->getPositionX() - 1000);
					page->runAction(MoveBy::create(.3f, Vect(1000, 0)));
				}

				if (_pageIndex == _pagesVec.size() - 1)
				{
					nextButton->setVisible(false);
					scheduleOnce([=](float dt) {
						readyButton->setVisible(true);
					}, .3f, "ShowReadyButton");
				}
			}
		});
	}
	else
	{
		readyButton->setVisible(true);
	}
	

	return true;
}

void StandbyLayout::buttonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == Widget::TouchEventType::ENDED)
	{
		if (_readyCallback != nullptr)
			_readyCallback();
		removeFromParent();
	}
}

void StandbyLayout::setReadyCallback(const ReadyCallback& callback)
{
	_readyCallback = callback;
}
