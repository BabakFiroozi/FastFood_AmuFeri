#include "EntrepotPopup.h"
#include "GameChoice.h"

using namespace cocos2d;
using namespace cocos2d::ui;


EntrepotPopup::EntrepotPopup()
{
}

EntrepotPopup::~EntrepotPopup()
{
}

EntrepotPopup* EntrepotPopup::create()
{
	EntrepotPopup* widget = new (std::nothrow) EntrepotPopup();
	if (widget && widget->init())
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

bool EntrepotPopup::init()
{
	if (!Layout::init())
		return false;

	setSwallowTouches(true);
	setTouchEnabled(true);

	setContentSize(Director::getInstance()->getVisibleSize());

	setAnchorPoint(Point::ANCHOR_MIDDLE);

	setBackGroundColor(Color3B::BLACK);
	setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	setBackGroundColorOpacity(100);

	auto backg = ImageView::create("gui/menu/entrepot_bg.png");
	addChild(backg);
	backg->setPosition(getContentSize() / 2);

	std::string fontName = GameChoice::getInstance().getFontName();
	const float fontSize = 68;
	auto title = Text::create("entropet", fontName, fontSize);
	addChild(title);
	title->setPosition(backg->getPosition() + Vect(0,  backg->getContentSize().height / 2 + 20));
	title->setTextColor(Color4B::GRAY);

	_listView = ListView::create();
	addChild(_listView);

	_listView->setAnchorPoint(Point::ANCHOR_MIDDLE);
	_listView->setPosition(getContentSize() / 2);
	_listView->setDirection(ScrollView::Direction::HORIZONTAL);
	_listView->setContentSize(Size(700, 1000));

	_listView->setItemsMargin(10);

	auto closeButton = Button::create("gui/menu/closeButton.png");
	closeButton->setName("close");
	addChild(closeButton);
	closeButton->setPosition(backg->getPosition() + backg->getContentSize() / 2 + Size(-closeButton->getContentSize().width / 2, closeButton->getContentSize().height / 2));
	closeButton->addTouchEventListener(CC_CALLBACK_2(EntrepotPopup::buttonCallback, this));

	return true;
}

void EntrepotPopup::onEnter()
{
	Layout::onEnter();

	updateEntrepot();
}

void EntrepotPopup::updateEntrepot()
{
	_listView->removeAllItems();

	std::string fontName = GameChoice::getInstance().getFontName();
	const float fontSize = 68;

	const int maxCount = 100;

	for (auto& food : FoodFactory::getInstance().getAllFoods())
	{
		if (!food->isUnlocked())
			continue;

		auto layout = Layout::create();
		layout->setContentSize(Size(460, 960));
		_listView->pushBackCustomItem(layout);

		layout->setBackGroundImage("gui/menu/entrepot_element.png");

		auto name = Text::create(food->getName(), fontName, fontSize);
		layout->addChild(name);
		name->setPosition(layout->getContentSize() / 2 + Size(0, -440));

		auto icon = ImageView::create(food->getIconPath());
		layout->addChild(icon);
		icon->setPosition(name->getPosition() + Vect(0, 120));
		icon->setScale(.55f);

		auto bar = LoadingBar::create("gui/menu/entrepot_bar.png");
		layout->addChild(bar);
		bar->setPosition(layout->getContentSize() / 2 + Size(0, 0));

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
		layout->addChild(button);
		button->setPosition(bar->getPosition() + Size(0, 300));
		button->addTouchEventListener(CC_CALLBACK_2(EntrepotPopup::buttonCallback, this));
		button->setName("charge");
		button->setTag((int)food->getType());

		auto add = Text::create("+5", fontName, fontSize);
		button->addChild(add);
		add->setPosition(button->getContentSize() / 2);

		auto coin = ImageView::create("gui/coin.png");
		layout->addChild(coin);
		coin->setPosition(button->getPosition() + Size(50, 100));

		auto price = Text::create(StringUtils::toString(food->getPrice()), fontName, fontSize);
		coin->addChild(price);
		price->setPosition(coin->getContentSize() / 2 - Size(50, 0));
		price->setTextHorizontalAlignment(TextHAlignment::RIGHT);
		price->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
	}
}

void EntrepotPopup::buttonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == Widget::TouchEventType::ENDED)
	{
		auto button = static_cast<Button*>(sender);
		std::string buttonName = button->getName();
		if (buttonName == "charge")
		{
			FoodTypes foodType = (FoodTypes)button->getTag();
			FoodFactory::getInstance().chargeFood(foodType, 5);
			updateEntrepot();
		}
		if (buttonName == "close")
			removeFromParent();
	}
}
