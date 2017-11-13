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

	_defaultShopType = (ShopTypes)initData["shopType"].asInt();

	_visibleOrigin = Director::getInstance()->getVisibleOrigin();
	_visibleSize = Director::getInstance()->getVisibleSize();

	auto background = ImageView::create("gui/shop/backg.png");
	addChild(background);
	background->setPosition(_visibleSize / 2);

	const int fontSize = 40;

	_backButton = Button::create("gui/shop/backButton.png");
	background->addChild(_backButton);
	_backButton->addTouchEventListener(CC_CALLBACK_2(ShopScene::buttonCallback, this));
	_backButton->setPosition(Vect(_backButton->getContentSize().width / 2, _visibleSize.height - _backButton->getContentSize().height / 2));

	auto shopBackg = ImageView::create("gui/shop/shopBackg.png");
	background->addChild(shopBackg);
	shopBackg->setPosition(Vect(_visibleSize.width / 2, _visibleSize.height - 690));

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
	const float fontSize = 68;

	std::string filePath = "shop.json";
	//if (!FileUtils::getInstance()->isFileExist(filePath))
	//	return;

	std::string str = FileUtils::getInstance()->getStringFromFile(filePath);
	rapidjson::Document doc;
	doc.Parse<0>(str.c_str());

	auto shopsArr = doc["food"].GetArray();
	if (shopType == ShopTypes::Coin)
		shopsArr = doc["coin"].GetArray();
	if (shopType == ShopTypes::Kitchen)
		shopsArr = doc["kitchen"].GetArray();
	if (shopType == ShopTypes::Powerup)
		shopsArr = doc["powerup"].GetArray();

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
		itemFrame->setOpacity(100);

		bool unlockedFood = false;

		if (shopType == ShopTypes::Food)
		{
			FoodTypes foodType = (FoodTypes)shop["type"].GetInt();
			auto food = FoodFactory::getInstance().getFood(foodType);
			unlockedFood = food->isUnlocked();
		}

		if (unlockedFood)
		{
			const int maxCount = 100;

			FoodTypes foodType = (FoodTypes)shop["type"].GetInt();
			auto food = FoodFactory::getInstance().getFood(foodType);

			auto name = Text::create(food->getName(), fontName, fontSize);
			itemFrame->addChild(name);
			name->setPosition(itemFrame->getContentSize() / 2 + Size(0, -440));

			auto icon = ImageView::create(food->getIconPath());
			itemFrame->addChild(icon);
			icon->setPosition(name->getPosition() + Vect(0, 120));
			icon->setScale(.55f);

			auto bar = LoadingBar::create("gui/menu/entrepot_bar.png");
			itemFrame->addChild(bar);
			bar->setPosition(itemFrame->getContentSize() / 2 + Size(0, 0));

			auto frame = ImageView::create("gui/menu/entrepot_frame.png");
			bar->addChild(frame);
			frame->setPosition(bar->getContentSize() / 2);
			bar->setRotation(-90);
			int foodCount = food->getCount();
			int percent = foodCount * 100 / maxCount;
			bar->setPercent(percent);

			auto count = Text::create(StringUtils::format("%d/%d", foodCount, maxCount), fontName, fontSize - 10);
			bar->addChild(count);
			count->setPosition(bar->getContentSize() / 2);
			count->setRotation(90);

			auto button = Button::create("gui/menu/entrepot_add.png", "gui/menu/entrepot_add__press.png");
			itemFrame->addChild(button);
			button->setPosition(bar->getPosition() + Size(0, 300));
			button->addTouchEventListener(CC_CALLBACK_2(ShopScene::addButtonCallback, this));
			button->setName("charge");
			button->setTag((int)food->getType());

			auto add = Text::create("+5", fontName, fontSize);
			button->addChild(add);
			add->setPosition(button->getContentSize() / 2);

			auto coin = ImageView::create("gui/coin.png");
			itemFrame->addChild(coin);
			coin->setPosition(button->getPosition() + Size(50, 100));

			auto price = Text::create(StringUtils::toString(food->getPrice()), fontName, fontSize);
			coin->addChild(price);
			price->setPosition(coin->getContentSize() / 2 - Size(50, 0));
			price->setTextHorizontalAlignment(TextHAlignment::RIGHT);
			price->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
		}
		else
		{
			std::string shopName = "";
			std::string shopIconPath = "";
			std::string frameIconPath = "";
			float shopIconScale = 1.0f;
			int shopAmount = shop["amount"].GetInt();

			bool unlockedFood = false;

			if (shopType == ShopTypes::Food)
			{
				FoodTypes foodType = (FoodTypes)shop["type"].GetInt();
				auto food = FoodFactory::getInstance().getFood(foodType);
				shopName = food->getName();
				shopIconPath = food->getIconPath();
				frameIconPath = "gui/shop/dishFrame.png";
				shopIconScale = .5f;
			}

			if (shopType == ShopTypes::Coin)
			{
				shopName = shop["name"].GetString();
				shopIconPath = StringUtils::format("gui/shop/coins/pack_%d.png", itemNumber);
				frameIconPath = "gui/shop/coins/frame.png";
				shopIconScale = 1;
			}

			if (shopType == ShopTypes::Kitchen)
			{
				shopName = shop["name"].GetString();
				shopIconPath = StringUtils::format("gui/shop/kitchens/kitchen_%d.png", itemNumber);
				frameIconPath = "gui/shop/kitchens/frame.png";
				shopIconScale = 1;
			}

			if (shopType == ShopTypes::Powerup)
			{
				shopName = shop["name"].GetString();
				shopIconPath = StringUtils::format("gui/shop/powers/power_%d.png", itemNumber);
				frameIconPath = "gui/shop/powers/frame.png";
				shopIconScale = 1;
			}

			auto nameText = Text::create(shopName, fontName, fontSize);
			itemFrame->addChild(nameText);
			nameText->setPosition(itemFrame->getContentSize() / 2 + Size(0, 420));			

			auto frameIcon = ImageView::create(frameIconPath);
			itemFrame->addChild(frameIcon);
			frameIcon->setPosition(itemFrame->getContentSize() / 2 + Size(0, 170));

			auto shopImage = ImageView::create(shopIconPath);
			frameIcon->addChild(shopImage);
			shopImage->setPosition(frameIcon->getContentSize() / 2 + Size(0, 10));
			shopImage->setScale(shopIconScale);

			int shopPrice = shop["price"].GetInt();
			auto priceText = Text::create(StringUtils::toString(shopPrice), fontName, fontSize);
			itemFrame->addChild(priceText);
			priceText->setPosition(itemFrame->getContentSize() / 2 + Size(0, -140));
			priceText->setTextHorizontalAlignment(TextHAlignment::RIGHT);
			priceText->setAnchorPoint(Point(1, .5f));
			priceText->setTextAreaSize(Size(140, 70));

			auto coinIcon = ImageView::create("gui/coin.png");
			priceText->addChild(coinIcon);
			coinIcon->setPosition(priceText->getTextAreaSize() / 2 + Size(120, 0));

			auto buyButton = Button::create("gui/shop/shopButton.png");
			itemFrame->addChild(buyButton);
			buyButton->setScale(.9f);
			buyButton->setPosition(itemFrame->getContentSize() / 2 + Size(0, -260));
			auto buyText = Text::create("Buy", fontName, fontSize);
			buyButton->addChild(buyText);
			buyText->setPosition(buyButton->getContentSize() / 2);
			if (shopType == ShopTypes::Kitchen)
			{
				KitchenTypes kitchenType = (KitchenTypes)(itemNumber - 1);
				if (Inventories::getInstance().kitchenUnlocked(kitchenType))
				{
					if (GameUser::getInstance().getCurrentKitchen() == kitchenType)
						buyText->setString("Selected");
					else
						buyText->setString("Select");
				}
			}

			_shopDataMap[buyButton] = ShopData(shopType, itemNumber - 1);

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
			FoodFactory::getInstance().unlockFood(foodType);
			showTab(ShopTypes::Food);
			PlayerPrefs::getInstance().saveFoods();
		}

		if (shopData.shopType == ShopTypes::Kitchen)
		{
			KitchenTypes kitchenType = (KitchenTypes)shopData.itemNumber;
			Inventories::getInstance().unlockKitchen(kitchenType);
			PlayerPrefs::getInstance().saveUnlockedKitchens();
			GameUser::getInstance().setCurrentKitchen(kitchenType);
			PlayerPrefs::getInstance().saveCurrentKitchen();
			showTab(ShopTypes::Kitchen);
		}
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
			FoodFactory::getInstance().chargeFood(foodType, 5);
			showTab(ShopTypes::Food);
		}
	}
}