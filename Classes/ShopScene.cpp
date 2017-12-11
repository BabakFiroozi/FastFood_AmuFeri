#include "ShopScene.h"
#include "GameChoice.h"
#include "MenuScene.h"
#include "FoodFactory.h"
#include "json/document.h"
#include "Inventories.h"
#include "GameUser.h"
#include "PlayerPrefs.h"


USING_NS_CC;
using namespace cocos2d::ui;

ShopScene::ShopScene()
{
}

ShopScene::~ShopScene()
{
}

bool ShopScene::init(ValueMap& initData)
{
	if (!Layer::init())
		return false;

	Device::setKeepScreenOn(true);

	_defaultShopType = (ShopTypes)initData["shopType"].asInt();

	_visibleOrigin = Director::getInstance()->getVisibleOrigin();
	_visibleSize = Director::getInstance()->getVisibleSize();

	auto background = Layout::create();
	addChild(background);
	background->setContentSize(_visibleSize);
	background->setAnchorPoint(Point::ANCHOR_MIDDLE);
	background->setPosition(_visibleOrigin + _visibleSize / 2);

	auto backgImage = ImageView::create("gui/shop/backg.png");
	background->addChild(backgImage);
	backgImage->setPosition(background->getContentSize() / 2);

	_backButton = Button::create("gui/shop/backButton.png");
	background->addChild(_backButton);
	_backButton->addTouchEventListener(CC_CALLBACK_2(ShopScene::buttonCallback, this));
	_backButton->setPosition(Vect(_backButton->getContentSize().width / 2, _visibleSize.height - _backButton->getContentSize().height / 2));

	auto shopBackg = ImageView::create("gui/shop/shopBackg.png");
	background->addChild(shopBackg);
	shopBackg->setPosition(Vect(_visibleSize.width / 2, _visibleSize.height - 690));

	auto shopHeader = ImageView::create("gui/shop/shop_header.png");
	shopBackg->addChild(shopHeader);
	shopHeader->setPosition(Vect(_visibleSize.width / 2 - 100, _visibleSize.height - 220));

	_headerText = Text::create(GameChoice::getInstance().getString("TEXT_FOODS"), GameChoice::getInstance().getFontName(), 68);
	shopHeader->addChild(_headerText);
	_headerText->setPosition(shopHeader->getContentSize() / 2);
	_headerText->enableGlow(Color4B::GREEN);
	shopHeader->setScale(.8f);

	auto tabButtonGroup = RadioButtonGroup::create();
	shopBackg->addChild(tabButtonGroup);
	for (int i = 0; i < 4; ++i)
	{
		auto tabButton = RadioButton::create("gui/shop/tabButton.png", "gui/shop/tabButtonSel.png");
		tabButtonGroup->addRadioButton(tabButton);
		tabButton->setPosition(Vect(125 + i * 164, shopBackg->getContentSize().height + tabButton->getContentSize().height / 2));
		shopBackg->addChild(tabButton);

		auto ShopIcon = ImageView::create(StringUtils::format("gui/shop/shop__%d.png", i + 1));
		tabButton->addChild(ShopIcon);
		ShopIcon->setPosition(tabButton->getContentSize() / 2);
		ShopIcon->setName("ShopIcon");

		tabButton->addEventListener(CC_CALLBACK_2(ShopScene::tabButtonCallback, this));

		if(i == 0) tabButton->setName("food");
		if (i == 1) tabButton->setName("power");
		if (i == 2) tabButton->setName("cook");
		if (i == 3) tabButton->setName("coin");
	}
	tabButtonGroup->setSelectedButton(0);

	_shopListView = ListView::create();
	_shopListView->setAnchorPoint(Point(.5f, .5f));
	_shopListView->setContentSize(Size(700, 998));
	shopBackg->addChild(_shopListView);
	//_shopListView->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	//_shopListView->setBackGroundColor(Color3B::RED);
	_shopListView->setPosition(shopBackg->getContentSize() / 2);
	_shopListView->setDirection(ListView::Direction::HORIZONTAL);

	_shopListView->setItemsMargin(10);

	/*auto shopFrameOver = ImageView::create("gui/shop/shopFrame2.png");
	shopBackg->addChild(shopFrameOver);
	shopFrameOver->setPosition(shopBackg->getContentSize() / 2);*/

	auto coinImage = ImageView::create("gui/coin.png");
	background->addChild(coinImage);
	coinImage->setPosition(_visibleSize - coinImage->getContentSize() / 2);

	int coinsCount = GameUser::getInstance().getCoin();
	auto coinsText = Text::create(StringUtils::format("%d x", coinsCount), GameChoice::getInstance().getFontName(true), 50);
	coinImage->addChild(coinsText);
	coinsText->setPosition(coinImage->getContentSize() / 2 + Size(-50, 0));
	coinsText->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	coinsText->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
	coinsText->enableOutline(Color4B::GRAY, 2);
	_coinsText = coinsText;

	_messagePopup = ImageView::create("gui/shop/messagePopup.png");
	background->addChild(_messagePopup);
	_messagePopup->setPosition(Vect(_visibleSize.width / 2, _visibleSize.height - _messagePopup->getContentSize().height / 2));
	auto messageText = Text::create("", GameChoice::getInstance().getFontName(), 60);
	_messagePopup->addChild(messageText);
	messageText->setPosition(_messagePopup->getContentSize() / 2);
	_messagePopup->setPositionY(_messagePopup->getPositionY() + 300);
	messageText->setTextColor(Color4B::WHITE);
	messageText->enableOutline(Color4B::GRAY, 3);

	return true;
}

void ShopScene::onEnter()
{
	Layer::onEnter();

	showTab(_defaultShopType);
}

void ShopScene::update(float dt)
{
	Layer::update(dt);
}

void ShopScene::buttonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == Widget::TouchEventType::ENDED)
	{
		if (sender == _backButton)
		{
			auto scene = TransitionSlideInL::create(.5f, MenuScene::createSceneData());
			Director::getInstance()->replaceScene(scene);
		}
	}
}

void ShopScene::tabButtonCallback(cocos2d::ui::RadioButton* sender, cocos2d::ui::RadioButton::EventType eventType)
{
	if (eventType == RadioButton::EventType::SELECTED)
	{
		ShopTypes shopType = ShopTypes::None;
		if (sender->getName() == "food") shopType = ShopTypes::Food;
		if (sender->getName() == "power") shopType = ShopTypes::Powerup;
		if (sender->getName() == "cook") shopType = ShopTypes::Kitchen;
		if (sender->getName() == "coin") shopType = ShopTypes::Coin;
		showTab(shopType);
	}
}

void ShopScene::showTab(ShopTypes shopType)
{
	std::string fontName = GameChoice::getInstance().getFontName();
	const float fontSize = 74;

	std::string filePath = "shop.json";
	if (!FileUtils::getInstance()->isFileExist(filePath))
	{
		CCLOG("Json file not found.");
		return;
	}

	std::string str = FileUtils::getInstance()->getStringFromFile(filePath);
	rapidjson::Document doc;
	doc.Parse<0>(str.c_str());

	_foodAddAmount = doc["foodAddAmount"].GetInt();

	auto shopsArr = doc["food"].GetArray();
	if (shopType == ShopTypes::Coin)
		shopsArr = doc["coin"].GetArray();
	if (shopType == ShopTypes::Kitchen)
		shopsArr = doc["kitchen"].GetArray();
	if (shopType == ShopTypes::Powerup)
		shopsArr = doc["powerup"].GetArray();

	if (shopType == ShopTypes::Food)
		_headerText->setString(GameChoice::getInstance().getString("TEXT_FOODS"));
	if (shopType == ShopTypes::Powerup)
		_headerText->setString(GameChoice::getInstance().getString("TEXT_POWERUPS"));
	if (shopType == ShopTypes::Kitchen)
		_headerText->setString(GameChoice::getInstance().getString("TEXT_KITCHENS"));
	if (shopType == ShopTypes::Coin)
		_headerText->setString(GameChoice::getInstance().getString("TEXT_COINS"));

	_shopListView->removeAllItems();

	_shopDataMap.clear();

	int itemNumber = 0;
	for (auto& shop : shopsArr)
	{
		itemNumber++;

		auto shopLayout = Layout::create();
		shopLayout->setContentSize(Size(560, 960));
		_shopListView->pushBackCustomItem(shopLayout);

		auto itemFrame = ImageView::create("gui/shop/shopFrame.png");
		shopLayout->addChild(itemFrame);
		itemFrame->setPosition(shopLayout->getContentSize() / 2 + Size(0, 0));
		//itemFrame->setOpacity(100);

		bool unlockedFood = false;

		if (shopType == ShopTypes::Food)
		{
			FoodTypes foodType = (FoodTypes)shop["type"].GetInt();
			auto food = FoodFactory::getInstance().getFood(foodType);
			unlockedFood = food->isUnlocked();
		}

		if (unlockedFood)
		{
			const int maxCount = Food::Max_Count;

			FoodTypes foodType = (FoodTypes)shop["type"].GetInt();
			auto food = FoodFactory::getInstance().getFood(foodType);			

			auto icon = ImageView::create(food->getIconPath());
			itemFrame->addChild(icon);
			icon->setPosition(itemFrame->getContentSize() / 2 + Size(0, -310));
			icon->setScale(.45f);

			auto name = Text::create(food->getName(), fontName, fontSize - 12);
			itemFrame->addChild(name);
			name->setPosition(icon->getPosition() + Vect(0, -90));
			name->setTextColor(Color4B(160, 100, 10, 255));

			auto frame = ImageView::create("gui/shop/entrepot_frame.png");
			itemFrame->addChild(frame);
			frame->setPosition(itemFrame->getContentSize() / 2);
			frame->setRotation(-90);			

			auto bar = LoadingBar::create("gui/shop/entrepot_bar.png");
			frame->addChild(bar);
			bar->setPosition(frame->getContentSize() / 2);
			int foodCount = food->getCount();
			int percent = foodCount * 100 / maxCount;
			bar->setPercent(percent);

			if (percent >= 50)
				bar->setColor(Color3B::GREEN);
			else if (percent >= 20)
				bar->setColor(Color3B::YELLOW);
			else
				bar->setColor(Color3B::RED);

			auto count = Text::create(StringUtils::toString(foodCount), fontName, fontSize - 10);
			bar->addChild(count);
			count->setPosition(bar->getContentSize() / 2);
			count->setRotation(90);

			auto button = Button::create("gui/shop/entrepot_add.png");
			itemFrame->addChild(button);
			button->setPosition(frame->getPosition() + Size(0, 300));
			button->addTouchEventListener(CC_CALLBACK_2(ShopScene::addButtonCallback, this));
			button->setName("charge");
			button->setTag((int)food->getType());

			auto add = Text::create(StringUtils::format("+%d", _foodAddAmount), fontName, fontSize);
			button->addChild(add);
			add->setPosition(button->getContentSize() / 2);
			add->enableOutline(Color4B::GRAY, 3);

			auto coin = ImageView::create("gui/coin.png");
			itemFrame->addChild(coin);
			coin->setPosition(button->getPosition() + Size(60, 90));

			int addPrice = food->getPrice() / food->getInitCount() * _foodAddAmount;
			auto price = Text::create(StringUtils::toString(addPrice), fontName, fontSize);
			coin->addChild(price);
			price->setPosition(coin->getContentSize() / 2 + Size(-50, 15));
			price->setTextHorizontalAlignment(TextHAlignment::RIGHT);
			price->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
			price->enableOutline(Color4B::GRAY, 3);
			price->setTextAreaSize(Size(200, 100));
		}
		else// not unlocked food
		{
			std::string shopName = "";
			std::string shopIconPath = "";
			std::string frameIconPath = "";
			float shopIconScale = 1.0f;			

			bool unlockedFood = false;

			int foodPrice = 0;
			int foodsCount = 0;
			if (shopType == ShopTypes::Food)
			{
				FoodTypes foodType = (FoodTypes)shop["type"].GetInt();
				auto food = FoodFactory::getInstance().getFood(foodType);
				shopName = food->getName();
				shopIconPath = food->getIconPath();
				frameIconPath = "gui/shop/dishFrame.png";
				shopIconScale = .5f;
				foodPrice = food->getPrice();
				foodsCount = food->getCount();
			}

			if (shopType == ShopTypes::Coin)
			{
				shopName = shop["name"].GetString();
				shopIconPath = StringUtils::format("gui/shop/coins/pack_%d.png", itemNumber);
				frameIconPath = "gui/shop/coins/frame.png";
				shopIconScale = .9f;
			}

			if (shopType == ShopTypes::Kitchen)
			{
				shopName = shop["name"].GetString();
				shopIconPath = StringUtils::format("gui/shop/kitchens/kitchen_%d.png", itemNumber);
				frameIconPath = "gui/shop/kitchens/frame.png";
				shopIconScale = 1.1f;
			}

			if (shopType == ShopTypes::Powerup)
			{
				shopName = shop["name"].GetString();
				shopIconPath = StringUtils::format("gui/shop/powers/powerup_%d.png", itemNumber);
				frameIconPath = "gui/shop/powers/frame.png";
				shopIconScale = 1;
			}

			auto nameText = Text::create(shopName, fontName, fontSize - 10);
			itemFrame->addChild(nameText);
			nameText->setPosition(itemFrame->getContentSize() / 2 + Size(0, 430));
			nameText->setTextColor(Color4B(160, 100, 10, 255));

			auto frameIcon = ImageView::create(frameIconPath);
			itemFrame->addChild(frameIcon);
			frameIcon->setPosition(itemFrame->getContentSize() / 2 + Size(0, 250));

			auto shopImage = ImageView::create(shopIconPath);
			frameIcon->addChild(shopImage);
			shopImage->setPosition(frameIcon->getContentSize() / 2 + Size(0, 10));
			shopImage->setScale(shopIconScale);

			auto priceFrame = ImageView::create("gui/shop/priceFrame.png");
			itemFrame->addChild(priceFrame);
			priceFrame->setPosition(itemFrame->getContentSize() / 2 + Size(0, -270));
			priceFrame->setOpacity(230);

			auto buyButton = Button::create("gui/shop/shopButton.png");
			itemFrame->addChild(buyButton);
			buyButton->setScale(.9f);
			buyButton->setPosition(itemFrame->getContentSize() / 2 + Size(0, -300));
			auto buyText = Text::create(GameChoice::getInstance().getString("TEXT_BUY"), fontName, fontSize);
			buyButton->addChild(buyText);
			buyText->setPosition(buyButton->getContentSize() / 2 + Size(0, -10));
			buyText->enableOutline(Color4B::GRAY, 3);
			if (shopType == ShopTypes::Kitchen)
			{
				KitchenTypes kitchenType = (KitchenTypes)(itemNumber - 1);
				if (Inventories::getInstance().isKitchenUnlocked(kitchenType))
				{
					if (GameUser::getInstance().getCurrentKitchen() == kitchenType)
						buyText->setString(GameChoice::getInstance().getString("TEXT_ACTIVE"));
					else
						buyText->setString(GameChoice::getInstance().getString("TEXT_SELECT"));
				}
			}

			int shopAmount = shop["amount"].GetInt();
			if (shopAmount > 1)
			{
				auto amountText = Text::create(StringUtils::toString(shopAmount), fontName, fontSize + 20);
				itemFrame->addChild(amountText);
				amountText->enableOutline(Color4B::GRAY, 2);
				if (shopType == ShopTypes::Food)
				{
					amountText->setString(StringUtils::toString(foodsCount));
					amountText->setPosition(itemFrame->getContentSize() / 2 + Size(0, 360));
				}
				if (shopType == ShopTypes::Coin)
					amountText->setPosition(itemFrame->getContentSize() / 2 + Size(0, 420));
			}

			int shopPrice = shop["price"].GetInt();
			if (shopPrice == -1)
				shopPrice = foodPrice;
			auto priceText = Text::create(StringUtils::toString(shopPrice), fontName, fontSize);
			itemFrame->addChild(priceText);
			priceText->setPosition(itemFrame->getContentSize() / 2 + Size(30, -220));
			priceText->setTextHorizontalAlignment(TextHAlignment::RIGHT);
			priceText->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
			priceText->setTextAreaSize(Size(340, 140));
			priceText->enableOutline(Color4B::GRAY, 3);

			if (shopType == ShopTypes::Coin)
			{
				std::string tomanStr = GameChoice::getInstance().getString("TEXT_TOUMAN") + " " + priceText->getString();
				priceText->setString(tomanStr);
				priceText->setPositionX(priceText->getPositionX() + 120);
			}
			else
			{
				auto coinIcon = ImageView::create("gui/coin.png");
				priceText->addChild(coinIcon);
				coinIcon->setPosition(priceText->getTextAreaSize() / 2 + Size(220, 10));
			}

			_shopDataMap[buyButton] = ShopData(shopType, itemNumber - 1, shopPrice);
			buyButton->addTouchEventListener(CC_CALLBACK_2(ShopScene::buyButtonCallback, this));
		}
	}
}

void ShopScene::buyButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == Widget::TouchEventType::ENDED)
	{
		auto button = static_cast<Button*>(sender);
		auto shopData = _shopDataMap[button];

		if (shopData.shopType == ShopTypes::Food)
		{
			FoodTypes foodType = (FoodTypes)shopData.itemNumber;
			int price = shopData.price;
			if (price <= GameUser::getInstance().getCoin())
			{
				FoodFactory::getInstance().unlockFood(foodType);
				updateListScrollPos();
				showTab(ShopTypes::Food);
				GameUser::getInstance().addCoin(-price);
			}
			else
			{
				//show some message
				showMessage(GameChoice::getInstance().getString("TEXT_NOT_ENOUGH_MONEY"));
			}
		}

		if (shopData.shopType == ShopTypes::Kitchen)
		{
			KitchenTypes kitchenType = (KitchenTypes)shopData.itemNumber;
			int price = shopData.price;
			if (price <= GameUser::getInstance().getCoin())
			{
				Inventories::getInstance().unlockKitchen(kitchenType);
				GameUser::getInstance().setCurrentKitchen(kitchenType);
				updateListScrollPos();
				showTab(ShopTypes::Kitchen);
				GameUser::getInstance().addCoin(-price);
			}
			else
			{
				//show some message
				showMessage(GameChoice::getInstance().getString("TEXT_NOT_ENOUGH_MONEY"));
			}
		}

		if (shopData.shopType == ShopTypes::Powerup)
		{
			PowerupTypes powerupType = (PowerupTypes)shopData.itemNumber;
			int price = shopData.price;
			if (price <= GameUser::getInstance().getCoin())
			{
				Inventories::getInstance().addPowerup(powerupType);
				//GameUser::getInstance().setCurrentKitchen(powerupType);
				updateListScrollPos();
				showTab(ShopTypes::Powerup);
				GameUser::getInstance().addCoin(-price);
			}
			else
			{
				//show some message
				showMessage(GameChoice::getInstance().getString("TEXT_NOT_ENOUGH_MONEY"));
			}
		}

		updateCoinsText();
	}
}

void ShopScene::addButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == Widget::TouchEventType::ENDED)
	{
		auto button = static_cast<Button*>(sender);
		std::string buttonName = button->getName();
		if (buttonName == "charge")
		{
			FoodTypes foodType = (FoodTypes)button->getTag();
			auto food = FoodFactory::getInstance().getFood(foodType);
			int addPrice = food->getPrice() / food->getCount() * _foodAddAmount;

			if (addPrice <= GameUser::getInstance().getCoin())
			{
				FoodFactory::getInstance().chargeFood(foodType, 5);
				updateListScrollPos();
				showTab(ShopTypes::Food);

				GameUser::getInstance().addCoin(-addPrice);
				updateCoinsText();
			}
			else
			{
				//show some message
				showMessage(GameChoice::getInstance().getString("TEXT_NOT_ENOUGH_MONEY"));
			}			
		}
	}
}

void ShopScene::updateListScrollPos()
{
	Vect pos = _shopListView->getInnerContainerPosition();
	runAction(Sequence::create(DelayTime::create(.03f), CallFunc::create([=]() {_shopListView->setInnerContainerPosition(pos); }), nullptr));
}

void ShopScene::onExit()
{
	Layer::onExit();
	PlayerPrefs::getInstance().saveUnlockedKitchens();
	PlayerPrefs::getInstance().saveCurrentKitchen();
	PlayerPrefs::getInstance().saveFoods();
}

void ShopScene::updateCoinsText()
{
	int coins = GameUser::getInstance().getCoin();
	_coinsText->setString(StringUtils::toString(coins));
	PlayerPrefs::getInstance().saveCoin();
}

void ShopScene::showMessage(const std::string& msg)
{
	if (_messagePopup == nullptr)
		return;

	const int actTag = 1020;
	auto act = _messagePopup->getActionByTag(actTag);
	if (act != nullptr && !act->isDone())
		return;

	auto text = static_cast<Text*>(_messagePopup->getChildren().at(0));
	text->setString(msg);

	auto seq = Sequence::create(
		MoveBy::create(.5f, Vect(0, -300)),
		DelayTime::create(1),
		MoveBy::create(.5f, Vect(0, 300)), nullptr);
	seq->setTag(actTag);
	_messagePopup->runAction(seq);
}
