#include "GameplayScene.h"
#include "GameChoice.h"
#include "MenuScene.h"
#include "ShopScene.h"
#include "PlayerPrefs.h"
#include "GameUser.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace cocos2d::ui;

GameplayScene::GameplayScene()
{
}

GameplayScene::~GameplayScene()
{
}

bool GameplayScene::init(cocos2d::ValueMap& initData)
{
	if (!Layer::init())
		return false;

	_sceneNumber = initData["sceneNumber"].asInt();

	_rightFoodTime = GameChoice::getInstance().getRightFoodTime();
	_wrongFoodTime = GameChoice::getInstance().getWrongFoodTime();
	_initClockTime = GameChoice::getInstance().getInitClockTime();

	_visibleSize = Director::getInstance()->getVisibleSize();
	_visibleOrigin = Director::getInstance()->getVisibleOrigin();

	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(GameplayScene::onKeyReleasedCallback, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	int kitchenNumber = (int)GameUser::getInstance().getCurrentKitchen() + 1;

	auto shelf = Sprite::create(StringUtils::format("kitchens/kitchen_%d/shelf.png", kitchenNumber));
	addChild(shelf);
	shelf->setPosition(Vect(_visibleSize.width / 2, _visibleSize.height - shelf->getContentSize().height / 2));

	std::string animationName = "idle";
	_animationsMap[animationName] = createCookAnimation(animationName, 40, .05f);

	animationName = "loss";
	_animationsMap[animationName] = createCookAnimation(animationName, 40, .05f);

	animationName = "ok";
	_animationsMap[animationName] = createCookAnimation(animationName, 40, .05f);

	auto table = Sprite::create(StringUtils::format("kitchens/kitchen_%d/table.png", kitchenNumber));
	addChild(table);
	table->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height - 850));

	_burger = Node::create();
	_burger->setPosition(Vec2(_visibleSize.width / 2 + 145, _visibleSize.height - 490));
	addChild(_burger);

	createHud();

	//create pause layout
	_pauseLayout = Layout::create();
	addChild(_pauseLayout);
	_pauseLayout->setContentSize(_visibleSize);
	_pauseLayout->setSwallowTouches(true);
	_pauseLayout->setTouchEnabled(true);
	_pauseLayout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	_pauseLayout->setBackGroundColor(Color3B::BLACK);
	_pauseLayout->setBackGroundColorOpacity(100);

	auto pauseBackg = ImageView::create("gui/pauseScreen/pauseBackg.png");
	_pauseLayout->addChild(pauseBackg);
	pauseBackg->setPosition(Size(_pauseLayout->getContentSize().width / 2, _pauseLayout->getContentSize().height - 500));

	auto pauseHeader = ImageView::create("gui/pauseScreen/pauseGameOver.png");
	pauseHeader->setName("pauseHeader");
	pauseBackg->addChild(pauseHeader);
	pauseHeader->setPosition(Vect(pauseBackg->getContentSize().width / 2, pauseBackg->getContentSize().height - pauseHeader->getContentSize().height / 2));

	auto gameOverHeader = ImageView::create("gui/pauseScreen/pauseHeader.png");
	gameOverHeader->setName("gameOverHeader");
	pauseBackg->addChild(gameOverHeader);
	gameOverHeader->setPosition(pauseHeader->getPosition());

	const float buttonsPosY = -220;
	const float buttonsScale = 1.3f;

	auto pauseText = Text::create(GameChoice::getInstance().getString("TEXT_GAME_PAUSE"), GameChoice::getInstance().getFontName(), 100);
	pauseText->setName("pauseText");
	pauseBackg->addChild(pauseText);
	pauseText->setPosition(pauseBackg->getContentSize() / 2 + Size(0, 50));
	pauseText->enableOutline(Color4B::GRAY, 4);

	auto resumeButton = Button::create("gui/pauseScreen/resumeButton.png");
	resumeButton->setName("resume");
	pauseBackg->addChild(resumeButton);
	resumeButton->setPosition(pauseBackg->getContentSize() / 2 + Size(-150, buttonsPosY));
	resumeButton->addTouchEventListener(CC_CALLBACK_2(GameplayScene::pauseButtonCallback, this));
	resumeButton->setScale(buttonsScale);

	auto retryButton = Button::create("gui/pauseScreen/retryButton.png");
	retryButton->setName("retry");
	pauseBackg->addChild(retryButton);
	retryButton->setPosition(resumeButton->getPosition());
	retryButton->addTouchEventListener(CC_CALLBACK_2(GameplayScene::pauseButtonCallback, this));
	retryButton->setScale(buttonsScale);

	auto menuButton = Button::create("gui/pauseScreen/menuButton.png");
	menuButton->setName("menu");
	pauseBackg->addChild(menuButton);
	menuButton->setPosition(pauseBackg->getContentSize() / 2 + Size(0, buttonsPosY));
	menuButton->addTouchEventListener(CC_CALLBACK_2(GameplayScene::pauseButtonCallback, this));
	menuButton->setScale(buttonsScale);

	auto shopButton = Button::create("gui/pauseScreen/shopButton.png");
	shopButton->setName("shop");
	pauseBackg->addChild(shopButton);
	shopButton->setPosition(pauseBackg->getContentSize() / 2 + Size(150, buttonsPosY));
	shopButton->addTouchEventListener(CC_CALLBACK_2(GameplayScene::pauseButtonCallback, this));
	shopButton->setScale(buttonsScale);

	auto statsLayout = Layout::create();
	statsLayout->setName("stats");
	statsLayout->setAnchorPoint(Point(.5f, .5f));
	statsLayout->setContentSize(Size(pauseBackg->getContentSize() - Size(50, 300)));
	pauseBackg->addChild(statsLayout);
	statsLayout->setPosition(pauseBackg->getContentSize() / 2 + Size(0, 50));
	//statsLayout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	//statsLayout->setBackGroundColor(Color3B::RED);

	auto clockFrame = ImageView::create("gui/gameover/clock.png");
	clockFrame->setName("clock");
	statsLayout->addChild(clockFrame);
	clockFrame->setPosition(statsLayout->getContentSize() / 2 + Size(0, 100));
	auto clockText = Text::create("", "fonts/Far_Morvarid.ttf", 44);
	clockFrame->addChild(clockText);
	clockText->setPosition(clockFrame->getContentSize() / 2 + Size(60, 0));
	clockText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	clockText->enableOutline(Color4B::GRAY, 2);

	auto moneyFrame = ImageView::create("gui/gameover/money.png");
	moneyFrame->setName("money");
	statsLayout->addChild(moneyFrame);
	moneyFrame->setPosition(statsLayout->getContentSize() / 2 + Size(0, 0));
	auto moneyText = Text::create("", "fonts/Far_Morvarid.ttf", 44);
	moneyFrame->addChild(moneyText);
	moneyText->setPosition(moneyFrame->getContentSize() / 2 + Size(-20, 0));
	moneyText->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	moneyText->enableOutline(Color4B::GRAY, 2);

	auto bonusFrame = ImageView::create("gui/gameover/bonus.png");
	bonusFrame->setName("bonus");
	statsLayout->addChild(bonusFrame);
	bonusFrame->setPosition(statsLayout->getContentSize() / 2 + Size(0, -100));
	auto bonusText = Text::create("", "fonts/Far_Morvarid.ttf", 44);
	bonusFrame->addChild(bonusText);
	bonusText->setPosition(bonusFrame->getContentSize() / 2 + Size(-20, 0));
	bonusText->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	bonusText->enableOutline(Color4B::GRAY, 2);

	_pauseLayout->setVisible(false);
	//

	return true;
}

void GameplayScene::onEnter()
{
	Layer::onEnter();

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/music_game.ogg", true);

	scheduleUpdate();
}

void GameplayScene::onKeyReleasedCallback(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		if (!_isGameOver)
			showPausePage(!_pauseLayout->isVisible(), false);
	}
}

void GameplayScene::createHud()
{
	_hudLayout = Layout::create();
	addChild(_hudLayout);
	_hudLayout->setContentSize(_visibleSize);
	_hudLayout->setAnchorPoint(Point::ANCHOR_MIDDLE);
	_hudLayout->setPosition(_visibleOrigin + _visibleSize / 2);

	auto recipeBack = ImageView::create("kitchens/recipeBack.png");
	_hudLayout->addChild(recipeBack);
	recipeBack->setName("recipeBack");
	recipeBack->setPosition(Vect(_visibleSize.width / 2 - 270, _visibleSize.height - recipeBack->getContentSize().height / 2 + 30));
	auto recipeBack2 = ImageView::create("kitchens/recipeBack_2.png");
	recipeBack->addChild(recipeBack2);
	recipeBack2->setPosition(recipeBack->getContentSize() / 2);
	recipeBack2->setOpacity(200);

	recipeBack->setScale(1.0f);

	auto recipeList = Layout::create();
	recipeBack->addChild(recipeList);
	recipeList->setName("recipe");
	recipeList->setContentSize(recipeBack2->getContentSize() - Size(20, 180));
	recipeList->setPosition(Vect(5, 5));
	//recipesList->setInnerContainerSize()
	//recipeList->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	//recipeList->setBackGroundColor(Color3B::RED);
	//recipeList->setBackGroundColorOpacity(100);
	

	auto coinImage = ImageView::create("gui/coin.png");
	_hudLayout->addChild(coinImage);
	coinImage->setPosition(_visibleSize - coinImage->getContentSize() / 2);

	_coinText = Text::create(StringUtils::format("%d*", GameUser::getInstance().getCoin()), GameChoice::getInstance().getFontName(), 65);
	coinImage->addChild(_coinText);
	_coinText->setPosition(coinImage->getContentSize() / 2 + Size(-50, 0));
	_coinText->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	_coinText->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
	_coinText->enableOutline(Color4B::GRAY, 2);

	auto pauseButton = Button::create("gui/pauseButton.png");
	_hudLayout->addChild(pauseButton);
	pauseButton->setName("pause");
	pauseButton->setPosition(Vect(pauseButton->getContentSize().width / 2, _hudLayout->getContentSize().height - pauseButton->getContentSize().height / 2));
	pauseButton->addTouchEventListener(CC_CALLBACK_2(GameplayScene::pauseButtonCallback, this));


	auto clockBack = ImageView::create("dishes/clockBack.png");
	_hudLayout->addChild(clockBack);
	clockBack->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height - 730));

	auto clockBar = LoadingBar::create("dishes/clockBar.png");
	clockBack->addChild(clockBar);
	clockBar->setPosition(clockBack->getContentSize() / 2 + Size(52, 2));
	clockBar->setPercent(100);
	_clockBar = clockBar;

	auto clockImage = ImageView::create("dishes/clock.png");
	clockBack->addChild(clockImage);
	clockImage->setPosition(Vec2(clockImage->getContentSize().width / 2, clockBack->getContentSize().height / 2));

	clockBack->setScale(.8f);

	auto comboBarFrame = ImageView::create("dishes/comboBar_frame.png");
	comboBarFrame->setAnchorPoint(Point(0, .5f));
	_hudLayout->addChild(comboBarFrame);
	comboBarFrame->setPosition(Vec2(_visibleSize.width / 2 - comboBarFrame->getContentSize().width / 2 + 40, _visibleSize.height - 680));

	_comboBar = LoadingBar::create("dishes/comboBar_bar.png", 50);
	comboBarFrame->addChild(_comboBar);
	_comboBar->setPosition(comboBarFrame->getContentSize() / 2);

	auto headLightIcon = ImageView::create("gui/shop/powerups/headLight.png");
	comboBarFrame->addChild(headLightIcon);
	headLightIcon->setPosition(_comboBar->getPosition() + Vect(0, 50));
	headLightIcon->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(.2f, 1.2f), ScaleTo::create(.2f, 1.0f))));
	comboBarFrame->setVisible(false);

	_cookAssistButton = Button::create("gui/shop/powerups/cookAssist.png");
	_hudLayout->addChild(_cookAssistButton);
	_cookAssistButton->setPosition(_hudLayout->getContentSize() / 2 + Size(200, 450));
	_cookAssistButton->addTouchEventListener(CC_CALLBACK_2(GameplayScene::cookAssistButtonCallback, this));
	_cookAssistButton->setVisible(false);
	_cookAssistButton->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(.2f, 1.2f), ScaleTo::create(.2f, 1.0f))));

	createRecipeAndDishes();

	_standbyLayout = StandbyLayout::createData(1);
	_hudLayout->addChild(_standbyLayout);
	_standbyLayout->setReadyCallback(CC_CALLBACK_0(GameplayScene::startGame, this));
}

void GameplayScene::cookAssistButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == Widget::TouchEventType::ENDED)
	{
		auto button = static_cast<Button*>(sender);
		int iter = 0;

		for (auto dish : _dishesVec)
			dish->setEnabled(false);
		scheduleOnce([=](float dt) {
			for (auto dish : _dishesVec)
				dish->setEnabled(true);
		}, _recipeFoodsVec.size() * .2f + .1f, "_isCookAssistUsing_to_false");

		for (auto needFood : _recipeFoodsVec)
		{
			scheduleOnce([=](float dt) {
				Button* rightDish = nullptr;
				for (auto dish : _dishesVec)
				{
					FoodTypes dishFood = (FoodTypes)dish->getTag();
					if (dishFood == needFood)
					{
						rightDish = dish;
						break;
					}
				}
				dishButtonCallback(rightDish, Widget::TouchEventType::BEGAN);
			}, ++iter * .2f, "CookAssistFood" + StringUtils::toString(iter));
		}	

		Inventories::getInstance().decPowerup(PowerupTypes::CookAssist);
		button->setVisible(false);
	}
}

void GameplayScene::dishButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == Widget::TouchEventType::BEGAN)
	{
		if (!_gameStarted || _isGameOver)
			return;

		if (_recipeFoodIndex == _recipeFoodsVec.size())
			return;

		auto button = static_cast<Button*>(sender);
		FoodTypes selFood = (FoodTypes)button->getTag();
		FoodTypes needFood = _recipeFoodsVec.at(_recipeFoodIndex);

		//if (selFood == FoodTypes::Bread_Top && needFood == FoodTypes::Bread_Neath)
		//	selFood = needFood;

		bool rightFood = selFood == needFood;

		auto food = FoodFactory::getInstance().getFood(needFood);
		if (food->getCount() <= 0)
		{
			onFoodFinished(needFood);
			return;
		}

		if (_forceTutDishButton != nullptr && _forceTutDishButton != button)
			return;

		if (rightFood)
		{
			auto foodSprite = Sprite::create(makeIconPath(food->getType(), food->getIconPath(), _burger->getChildrenCount() == 0));
			Vec2 pos = Vec2::ZERO;
			if (_burger->getChildrenCount() > 0)
			{
				auto lastFoodSprite = _burger->getChildren().at(_burger->getChildrenCount() - 1);
				pos = lastFoodSprite->getPosition();
				pos.y += food->getHeight();
			}
			_burger->addChild(foodSprite);
			foodSprite->setPosition(pos);
			foodSprite->setScale(.35f);

			auto scale1 = ScaleBy::create(.1f, 1.15f);
			auto scale2 = ScaleBy::create(.1f, 1.0f / 1.15f);
			foodSprite->runAction(Sequence::create(scale1, EaseBounceOut::create(scale2), nullptr));

			if (_recipeFoodIndex == _recipeFoodsVec.size() - 1)
			{
				playCookAnimation("ok", false);
				scheduleOnce(CC_SCHEDULE_SELECTOR(GameplayScene::packBurger), .3f);
			}

			if (!_isHeadLightActive)
			{
				_clockTimer += _rightFoodTime;
				if (_clockTimer > _initClockTime)
					_clockTimer = _initClockTime;

				if (_burgersCount > 0 && Inventories::getInstance().hasPowerup(PowerupTypes::HeadLight))
				{
					auto powerup = Inventories::getInstance().getPowerupByType(PowerupTypes::HeadLight);
					if (_clockTimer >= _initClockTime)
					{
						_isHeadLightActive = true;
						_clockTimer = _initClockTime + powerup->getValue();
						_comboBar->getParent()->setVisible(true);
						_comboBar->setPercent(100);
						Inventories::getInstance().decPowerup(PowerupTypes::HeadLight);
					}
				}
			}

			auto recipeList = static_cast<Layout*>(_hudLayout->getChildByName("recipeBack")->getChildByName("recipe"));
			auto foodLayout = recipeList->getChildren().at(_recipeFoodIndex);

			auto trueSign = foodLayout->getChildByName("trueSign");
			auto falseSign = foodLayout->getChildByName("falseSign");
			trueSign->setVisible(true);
			falseSign->setVisible(false);

			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/right.ogg");

			FoodFactory::getInstance().consumeFood(food->getType());

			_recipeFoodIndex++;

			auto foodCountLabel = static_cast<Label*>(button->getChildByName("FoodsCount"));
			foodCountLabel->setString(StringUtils::toString(food->getCount()));
			if (food->getCount() == 0)
				foodCountLabel->setTextColor(Color4B::RED);

			goTutorialStep();
		}
		else//wrong food
		{
			playCookAnimation("loss", false);

			if (!_isHeadLightActive)
			{
				_clockTimer -= _wrongFoodTime * _clockDecerementRate;
			}
			else
			{
				_isHeadLightActive = false;
				_clockTimer = _initClockTime * .9f;
				_comboBar->getParent()->setVisible(false);
			}

			auto recipeList = static_cast<Layout*>(_hudLayout->getChildByName("recipeBack")->getChildByName("recipe"));
			auto foodLayout = recipeList->getChildren().at(_recipeFoodIndex);

			auto trueSign = foodLayout->getChildByName("trueSign");
			auto falseSign = foodLayout->getChildByName("falseSign");
			trueSign->setVisible(false);
			falseSign->setVisible(true);

			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/wrong.ogg");

			goTutorialStep();
		}
	}
}

void GameplayScene::createRecipeAndDishes()
{
	//Create dishes
	const int Dishes_Count = 5;

	for (int i = 0; i < Dishes_Count; ++i)
		_hudLayout->removeChildByName(StringUtils::format("dish__%d", i));

	_availableFoodsVec.clear();

	std::vector<FoodTypes> unlockedFoodsVec;
	for (int c = (int)(FoodTypes::Bread) + 1; c < (int)FoodTypes::Count; ++c)
	{
		FoodTypes foodType = (FoodTypes)c;
		if (FoodFactory::getInstance().getFood(foodType)->isUnlocked())
			unlockedFoodsVec.push_back(foodType);
	}

	//foods on dishes
	int breadDishNumber = random(0, Dishes_Count - 1);
	FoodTypes foodType = FoodTypes::None;
	bool exist = false;
	do
	{
		if (_availableFoodsVec.size() == breadDishNumber)
		{
			_availableFoodsVec.push_back(FoodTypes::Bread);
			continue;
		}
		int randomFood = random(0, (int)unlockedFoodsVec.size() - 1);
		foodType = (FoodTypes)unlockedFoodsVec.at(randomFood);
		exist = std::find(_availableFoodsVec.begin(), _availableFoodsVec.end(), foodType) != _availableFoodsVec.end();
		if (!exist)
			_availableFoodsVec.push_back(foodType);
	} while (_availableFoodsVec.size() < Dishes_Count);

	for (auto oldDishButton : _dishesVec)
		oldDishButton->removeFromParent();
	_dishesVec.clear();

	float initX = _visibleSize.width / 2 - 200;
	Vect dishOffset(initX, 200);
	for (int i = 0; i < Dishes_Count; ++i)
	{
		auto dishButton = Button::create("dishes/dish_1.png");
		FoodTypes foodType = _availableFoodsVec.at(i);
		auto food = FoodFactory::getInstance().getFood(foodType);
		auto foodImage = ImageView::create(food->getIconPath());
		dishButton->addChild(foodImage);
		foodImage->setScale(.3f);
		foodImage->setPosition(dishButton->getContentSize() / 2);

		dishButton->setTag((int)foodType);

		dishButton->setName(StringUtils::format("dish__%d", i));

		_hudLayout->addChild(dishButton);
		dishButton->setPosition(dishOffset);

		auto foodCountLabel = Label::createWithTTF(StringUtils::toString(food->getCount()), "fonts/Far_Morvarid.ttf", 40);
		foodCountLabel->setName("FoodsCount");
		dishButton->addChild(foodCountLabel);
		foodCountLabel->setPosition(dishButton->getContentSize() / 2 + Size(0, 80));
		foodCountLabel->setTextColor(Color4B::ORANGE);
		if (food->getCount() == 0)
			foodCountLabel->setTextColor(Color4B::RED);
		foodCountLabel->enableOutline(Color4B::GRAY, 1);

		Size size = dishButton->getContentSize();

		dishOffset.x += size.width;
		if (i == 2)
		{
			dishOffset.x = initX + size.width / 2;
			dishOffset.y += size.height;
		}

		dishButton->addTouchEventListener(CC_CALLBACK_2(GameplayScene::dishButtonCallback, this));
		_dishesVec.pushBack(dishButton);
	}
	//dishes

	_recipeFoodsVec.clear();

	_recipeFoodsVec.push_back(FoodTypes::Bread);

	int minFoodsCount = 2;
	int maxFoodsCount = minFoodsCount  + _burgersCount / 3;
	if (maxFoodsCount > 8)
		maxFoodsCount = 8;

	int foodsCount = cocos2d::random(minFoodsCount, maxFoodsCount);

	if (!PlayerPrefs::getInstance().isTutorialFinished(1))
		foodsCount = 3;

	for (int f = 0; f < foodsCount; ++f)
	{
		int randomNumber = random(0, (int)_availableFoodsVec.size() - 1);
		if (randomNumber == breadDishNumber)
		{
			--f;
			continue;
		}
		FoodTypes foodType = _availableFoodsVec.at(randomNumber);

		int alreadyExist = 0;
		for (int v = 0; v < (int)_recipeFoodsVec.size(); ++v)
			alreadyExist += _recipeFoodsVec.at(v) == foodType ? 1 : 0;

		int maxSameFood = (PlayerPrefs::getInstance().isTutorialFinished(1) ? 3 : 1) - 1;

		if (alreadyExist > maxSameFood)
		{
			--f;
			continue;
		}

		_recipeFoodsVec.push_back(foodType);
	}

	_recipeFoodsVec.push_back(FoodTypes::Bread);

	auto recipeList = _hudLayout->getChildByName("recipeBack")->getChildByName("recipe");

	recipeList->removeAllChildren();

	for (int i = 0; i < (int)_recipeFoodsVec.size(); ++i)
	{
		auto food = FoodFactory::getInstance().getFood(_recipeFoodsVec.at(i));
		auto layout = Layout::create();
		layout->setContentSize(Size(170, 25));
		auto foodImage = ImageView::create(makeIconPath(food->getType(), food->getIconPath(), i == 0));
		foodImage->setScale(.25f);
		foodImage->setPosition(layout->getContentSize() / 2);
		layout->addChild(foodImage);
		recipeList->addChild(layout);
		layout->setPosition(Vec2(0, (i + 1) * layout->getContentSize().height * 1.25f));

		auto trueSign = ImageView::create("gui/trueSign.png");
		trueSign->setName("trueSign");
		layout->addChild(trueSign);
		trueSign->setPosition(layout->getContentSize() / 2 + Size(100, 0));
		trueSign->setScale(.4f);
		trueSign->setVisible(false);

		auto falseSign = ImageView::create("gui/falseSign.png");
		falseSign->setName("falseSign");
		layout->addChild(falseSign);
		falseSign->setPosition(layout->getContentSize() / 2 + Size(100, 0));
		falseSign->setScale(.4f);
		falseSign->setVisible(false);
	}

	_recipeFoodIndex = 0;

	if (_burgersCount > 0)
	{
		bool allowActiveCookAssist = true;
		for (auto needFood : _recipeFoodsVec)
			if (FoodFactory::getInstance().getFood(needFood)->getCount() < 1)
				allowActiveCookAssist = false;
		_cookAssistButton->setVisible(allowActiveCookAssist && Inventories::getInstance().hasPowerup(PowerupTypes::CookAssist));
	}

	playHumanSound();
}

void GameplayScene::startGame()
{
	_clockTimer = _initClockTime;
	if (!PlayerPrefs::getInstance().isTutorialFinished(1))
		_clockTimer -= _initClockTime / 2;
	_gameStarted = true;
	scheduleOnce([=](float dt) {
		playCookAnimation("idle", true);
	}, 4, "Idle_First");

	goTutorialStep();
}

void GameplayScene::update(float delta)
{
	Layer::update(delta);

	if (_gameStarted && !_isGameOver && _foodFinishedLayout == nullptr)
	{
		if (!_burgerIsPacking || _isHeadLightActive)
			if (PlayerPrefs::getInstance().isTutorialFinished(1))
				_clockTimer -= delta * _clockDecerementRate;

		if (_isHeadLightActive)
		{
			float barPercent = (_clockTimer - _initClockTime) / Inventories::getInstance().getPowerupByType(PowerupTypes::HeadLight)->getValue() * 100;
			_comboBar->setPercent(barPercent);

			if (_clockTimer <= _initClockTime)
			{
				_clockTimer = _initClockTime * .9f;
				_isHeadLightActive = false;
				_comboBar->getParent()->setVisible(false);
			}
		}

		float barPercent = _clockTimer / _initClockTime * 100;
		_clockBar->setPercent(barPercent);

		_coinText->setString(StringUtils::format("%d*", GameUser::getInstance().getCoin()));

		if (_clockTimer <= 0)
		{
			gameOver();
		}

		_playDuration += delta;
	}
}

void GameplayScene::gameOver()
{
	_isGameOver = true;
	showPausePage(true, true);
}

void GameplayScene::packBurger(float dt)
{
	_burgersCount++;

	if (_burgersCount == 2)
		goTutorialStep();

	if (_burgersCount == 1)
	{
		scheduleOnce([=](float dt) {
			goTutorialStep();
		}, 1, "ShowTurnTwoTutorial");
	}

	if (Inventories::getInstance().hasPowerup(PowerupTypes::RichCustomer))
	{
		Inventories::getInstance().decPowerup(PowerupTypes::RichCustomer);
		createAdjunct();
	}

	//pack and give
	Vect burgerPos = _burger->getPosition();
	auto func1 = CallFunc::create([=]() {
		//pack animation code
		int coin = 0;
		for (auto foodType : _recipeFoodsVec)
		{
			auto food = FoodFactory::getInstance().getFood(foodType);
			coin += food->getPrice() / food->getInitCount() + food->getWorth();
		}

		if (!_isHeadLightActive)
			coin *= 2;

		if (Inventories::getInstance().hasPowerup(PowerupTypes::CaptainCook))
		{
			Inventories::getInstance().decPowerup(PowerupTypes::CaptainCook);
			auto powerup = Inventories::getInstance().getPowerupByType(PowerupTypes::CaptainCook);
			coin += random(powerup->getValue() / 2, powerup->getValue());
			//effect
			auto captainCookEffect = ImageView::create("gui/shop/powerups/captainCook.png");
			_hudLayout->addChild(captainCookEffect);
			captainCookEffect->setPosition(_clockBar->getParent()->getPosition() + Vect(0, -80));
			captainCookEffect->setScale(.2f);
			captainCookEffect->runAction(Sequence::create(EaseBounceOut::create(ScaleTo::create(.3f, .9f)), DelayTime::create(1), RemoveSelf::create(), nullptr));
		}

		giveCoinWithEffect(coin, _coinText->getParent()->getPosition() - Vect(170, 330), .8f);

		KitchenTypes currentKitchen = GameUser::getInstance().getCurrentKitchen();
		int kitchenCoinCoef = Inventories::getInstance().getKitchenByType(currentKitchen)->getValue();
		coin *= kitchenCoinCoef;		

	});

	auto move1 = MoveTo::create(.3f, burgerPos + Vect(400, 0));
	auto move2 = MoveTo::create(.0f, burgerPos);
	auto seq = Sequence::create(func1, DelayTime::create(.1f),
		CallFunc::create([=]() { 
		auto packBag = Sprite::create("dishes/packBag.png");
		_burger->addChild(packBag);
		packBag->setPosition(Vect(0, 110)); }),
		DelayTime::create(.1f), move1, CallFunc::create([=]() {_burger->removeAllChildren(); }), move2, nullptr);
	_burger->runAction(seq);


	auto recipeBack = _hudLayout->getChildByName("recipeBack");
	Vec2 recipePos = recipeBack->getPosition();

	move1 = MoveTo::create(.3f, recipePos + Vect(-500, 0));
	move2 = MoveTo::create(.0f, recipePos + Vect(0, 500));
	auto move3 = MoveTo::create(.3f, recipePos);
	seq = Sequence::create(DelayTime::create(.2f), move1, move2, CallFunc::create([=]() {createRecipeAndDishes(); _recipeFoodIndex = 0; }), EaseSineInOut::create(move3), nullptr);
	recipeBack->runAction(seq);

	float packBurgerTime = GameChoice::getInstance().getPackBurgerTime();

	_burgerIsPacking = true;
	scheduleOnce([=](float dt) {
		_burgerIsPacking = false;
		auto adjunctButton = _hudLayout->getChildByName("Adjunct");
		if (adjunctButton != nullptr)
		{
			if (adjunctButton->getPositionX() > _hudLayout->getContentSize().width / 2)
				adjunctButton->runAction(Sequence::create(EaseSineIn::create(MoveBy::create(.2f, Vect(400, 0))), RemoveSelf::create(), nullptr));
			else
				adjunctButton->runAction(Sequence::create(EaseSineIn::create(MoveBy::create(.2f, Vect(-400, 0))), RemoveSelf::create(), nullptr));
		}
	}, packBurgerTime, "PackingBurgerFinished");

	if (PlayerPrefs::getInstance().isTutorialFinished(1))
	{
		if (_clockDecerementRate < 1)
		{
			_clockDecerementRate += .1f;
			if (_clockDecerementRate > 1)
				_clockDecerementRate = 1;
		}
		else
		{
			_clockDecerementRate += .02f;
			if (_clockDecerementRate > 1.5f)
				_clockDecerementRate = 1.5f;
		}
	}

	return;
}

void GameplayScene::onExit()
{
	Layer::onExit();
	PlayerPrefs::getInstance().saveFoods();
}

void GameplayScene::pauseButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (!PlayerPrefs::getInstance().isTutorialFinished(1))
		return;

	if (eventType == Widget::TouchEventType::ENDED)
	{
		auto button = static_cast<Button*>(sender);

		std::string buttonName = button->getName();

		if (buttonName == "pause")
		{
			showPausePage(true, false);
		}

		if (buttonName == "shop")
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			Director::getInstance()->resume();
			auto scene = TransitionFlipX::create(.5f, ShopScene::createSceneData(ShopTypes::Food));
			Director::getInstance()->replaceScene(scene);
		}

		if (buttonName == "retry")
		{
			Director::getInstance()->resume();
			auto scene = TransitionFade::create(.5f, GameplayScene::createSceneData(_sceneNumber));
			Director::getInstance()->replaceScene(scene);
		}

		if (buttonName == "resume")
		{
			showPausePage(false, false);
		}

		if (buttonName == "menu")
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			Director::getInstance()->resume();
			auto scene = TransitionTurnOffTiles::create(.7f, MenuScene::createSceneData());
			Director::getInstance()->replaceScene(scene);
		}
	}
}

void GameplayScene::showPausePage(bool show, bool gameOver)
{
	if(gameOver)
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/music_end.ogg", false);

	auto pauseBackg = _pauseLayout->getChildren().at(0);
	pauseBackg->getChildByName("resume")->setVisible(!gameOver);
	pauseBackg->getChildByName("retry")->setVisible(gameOver);
	pauseBackg->getChildByName("stats")->setVisible(gameOver);
	pauseBackg->getChildByName("pauseHeader")->setVisible(!gameOver);
	pauseBackg->getChildByName("gameOverHeader")->setVisible(gameOver);
	pauseBackg->getChildByName("pauseText")->setVisible(!gameOver);
	_pauseLayout->setVisible(show);

	pauseBackg->getChildByName("shop")->setVisible(PlayerPrefs::getInstance().isTutorialFinished(2));

	auto stats = pauseBackg->getChildByName("stats");

	int duration = _playDuration;
	int hou = duration / 3600;
	int min = duration / 60 % 60;
	int sec = duration % 60;
	auto clockText = static_cast<Text*>(stats->getChildByName("clock")->getChildren().at(0));
	clockText->setString(StringUtils::toString(StringUtils::format("%02d : %02d : %02d", hou, min, sec)));

	auto moneyText = static_cast<Text*>(stats->getChildByName("money")->getChildren().at(0));
	moneyText->setString(StringUtils::toString(_coinsCount));

	auto bonusText = static_cast<Text*>(stats->getChildByName("bonus")->getChildren().at(0));
	bonusText->setString(StringUtils::toString(_burgersCount));

	int previousRecord = PlayerPrefs::getInstance().getSandwitch();

	PlayerPrefs::getInstance().setSandwitch(_burgersCount);

	if (previousRecord > 0 && previousRecord < _burgersCount && show && gameOver)
	{
		auto star = ImageView::create("gui/score.png");
		pauseBackg->addChild(star);
		star->setPosition(pauseBackg->getContentSize() / 2 + Size(250, -50));
		star->runAction(Repeat::create(RotateBy::create(360, 1), 100));
	}

	PlayerPrefs::getInstance().addSandwitchTotal(_burgersCount);

	if (show)
		Director::getInstance()->pause();
	else
		Director::getInstance()->resume();
}

std::string GameplayScene::makeIconPath(const FoodTypes foodType, const std::string& iconPath, bool cond)
{
	std::string finalPath = "";
	if (foodType == FoodTypes::Bread)
	{
		std::string path = iconPath;
		path.insert(path.find('.'), (cond ? "_neath" : "_top"));
		finalPath = path;
	}
	else
		finalPath = iconPath;
	return finalPath;
}

cocos2d::Sprite* GameplayScene::createCookAnimation(const std::string& animationName, const int framesCount, float delay /*= 0.0f*/, unsigned int loops /*= 1U*/)
{
	int kitchenNumber = (int)GameUser::getInstance().getCurrentKitchen() + 1;

	auto cookSpriteBatch = SpriteBatchNode::create(StringUtils::format("kitchens/kitchen_%d/cook.png", kitchenNumber));
	addChild(cookSpriteBatch);
	cookSpriteBatch->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height - 300));
	auto spriteFrameChache = SpriteFrameCache::getInstance();
	spriteFrameChache->addSpriteFramesWithFile(StringUtils::format("kitchens/kitchen_%d/cook.plist", kitchenNumber));

	if (kitchenNumber == 1)
		cookSpriteBatch->setScale(1.5f);
	if (kitchenNumber == 2)
		cookSpriteBatch->setScale(1.8f);
	if (kitchenNumber == 3)
		cookSpriteBatch->setScale(1.7f);

	auto firstSprite = Sprite::createWithSpriteFrameName(StringUtils::format("Cook%d_%s__001.png", kitchenNumber, animationName.c_str()));
	cookSpriteBatch->addChild(firstSprite);

	Vector<SpriteFrame*> spriteFramesVec;
	for (int i = 1; i <= framesCount; ++i)
	{
		std::string frameName = StringUtils::format("Cook%d_%s__%03d.png", kitchenNumber, animationName.c_str(), i);
		auto sprFrame = spriteFrameChache->getSpriteFrameByName(frameName);
		if (sprFrame != nullptr)
			spriteFramesVec.pushBack(sprFrame);
	}
	auto animation = Animation::createWithSpriteFrames(spriteFramesVec, delay, loops);
	firstSprite->setUserData(animation);
	animation->retain();

	return firstSprite;
}

void GameplayScene::playCookAnimation(const std::string& animationName, bool loop)
{
	for (auto& m : _animationsMap)
	{
		auto sprite = m.second;
		sprite->setVisible(m.first == animationName);
	}

	auto spr = _animationsMap[animationName];
	auto animation = static_cast<Animation*>(spr->getUserData());

	const int actionTag = 10001;
	auto action = spr->getActionByTag(actionTag);
	if (action != nullptr)
		spr->stopAction(action);

	auto animate = Animate::create(animation);
	animate->setTag(actionTag);

	spr->runAction(animate);

	if (animationName == "loss" || animationName == "ok")
	{
		scheduleOnce([=](float dt) {
			playCookAnimation("idle", true);
		}, 4, "Idle_After_Loss_Ok");
	}
}

void GameplayScene::onFoodFinished(FoodTypes foodType)
{
	auto food = FoodFactory::getInstance().getFood(foodType);

	_foodFinishedLayout = Layout::create();
	_foodFinishedLayout->setContentSize(_visibleSize);
	_foodFinishedLayout->setTouchEnabled(true);
	_foodFinishedLayout->setSwallowTouches(true);
	addChild(_foodFinishedLayout);

	_foodFinishedLayout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	_foodFinishedLayout->setBackGroundColor(Color3B::BLACK);
	_foodFinishedLayout->setBackGroundColorOpacity(150);


	auto popupBackg = ImageView::create("gui/gameover/finishedBackg.png");
	_foodFinishedLayout->addChild(popupBackg);
	popupBackg->setPosition(_foodFinishedLayout->getContentSize() / 2 + Size(0, 200));

	auto warning = ImageView::create("gui/gameover/warning.png");
	popupBackg->addChild(warning);
	Size backgSize = popupBackg->getContentSize();
	warning->setPosition(Size(backgSize.width / 2, backgSize.height + 80));
	warning->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(.2f, 1.1f), ScaleTo::create(.2f, 1.0f))));

	auto foodImage = ImageView::create(food->getIconPath());
	popupBackg->addChild(foodImage);
	foodImage->setPosition(backgSize / 2 + Size(0, 190));
	foodImage->setScale(.5f);

	std::string fontName = GameChoice::getInstance().getFontName();

	std::string textStr = GameChoice::getInstance().getString("TEXT_FOOD_FINISHED") + " " + food->getName();
	auto text = Text::create(textStr, fontName, 60);
	popupBackg->addChild(text);
	text->setPosition(backgSize / 2 + Size(0, 70));
	text->setTextColor(Color4B::RED);
	text->enableOutline(Color4B::YELLOW, 2);

	auto message = Text::create(GameChoice::getInstance().getString("TEXT_CUSTOMER_WATING"), fontName, 45);
	popupBackg->addChild(message);
	message->setPosition(backgSize / 2 + Size(0, -20));
	message->enableOutline(Color4B::GRAY, 2);


	auto button = Button::create("gui/shop/entrepot_add.png");
	button->setScale(1.2f);
	popupBackg->addChild(button);
	button->setPosition(backgSize / 2 + Size(0, -205));
	button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType eventType) {
		if (eventType == Widget::TouchEventType::ENDED)
		{
			FoodFactory::getInstance().chargeFood(food->getType(), 5);
			for (auto dish : _dishesVec)
			{
				if ((FoodTypes)dish->getTag() == foodType)
				{
					auto foodCountLabel = static_cast<Label*>(dish->getChildByName("FoodsCount"));
					foodCountLabel->setString(StringUtils::toString(food->getCount()));
					foodCountLabel->setTextColor(Color4B::ORANGE);
				}
			}
		}
	});

	auto pointer = ImageView::create("gui/hand_pointer.png");
	popupBackg->addChild(pointer);
	pointer->setPosition(button->getPosition() + Size(-200, 0));
	pointer->runAction(RepeatForever::create(Sequence::createWithTwoActions(MoveBy::create(.3f, Vect(30, 0)), MoveBy::create(.3f, Vect(-30, 0)))));

	int fontSize = 78;

	auto add = Text::create(StringUtils::format("+%d", 5), fontName, fontSize);
	button->addChild(add);
	add->setPosition(button->getContentSize() / 2);
	add->enableOutline(Color4B::GRAY, 3);

	auto coin = ImageView::create("gui/coin.png");
	popupBackg->addChild(coin);
	coin->setPosition(button->getPosition() + Size(60, 95));

	auto price = Text::create(StringUtils::toString(food->getPrice()), fontName, fontSize);
	coin->addChild(price);
	price->setPosition(coin->getContentSize() / 2 + Size(-50, 15));
	price->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	price->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
	price->enableOutline(Color4B::GRAY, 3);
	price->setTextAreaSize(Size(200, 100));

	auto close = Button::create("gui/menu/closeButton.png");
	popupBackg->addChild(close);
	close->setPosition(backgSize);
	close->addTouchEventListener([=](Ref* sender, Widget::TouchEventType eventType) {
		if (eventType == Widget::TouchEventType::ENDED)
		{
			_foodFinishedLayout->removeFromParent();
			_foodFinishedLayout = nullptr;
		}
	});
}

void GameplayScene::createAdjunct()
{
	if (random(0, 100) < 50)
		return;

	rapidjson::Document doc;
	std::string str = FileUtils::getInstance()->getStringFromFile("adjunct.json");
	doc.Parse<0>(str.c_str());

	auto arr = doc["adjunct"].GetArray();
	int n = random(0, (int)arr.Size() - 1);
	auto& adjunct = arr[n];

	std::string iconPath = adjunct["iconPath"].GetString();
	int worth = adjunct["worth"].GetInt();
	std::string name = adjunct["name"].GetString();

	if (_hudLayout->getChildByName("Adjunct") != nullptr)
		_hudLayout->removeChildByName("Adjunct");

	auto button = Button::create(iconPath);
	button->setName("Adjunct");
	_hudLayout->addChild(button);
	if (random(0, 100) < 50)
	{
		button->setPosition(_dishesVec.at(3)->getPosition() + Vect(-200, 20));
		button->setPositionX(button->getPositionX() - 400);
		button->runAction(EaseSineOut::create(MoveBy::create(.2f, Vect(400, 0))));
	}
	else
	{
		button->setPosition(_dishesVec.at(4)->getPosition() + Vect(200, 20));
		button->setPositionX(button->getPositionX() + 400);
		button->runAction(EaseSineOut::create(MoveBy::create(.2f, Vect(-400, 0))));
	}	

	button->setScale(.35f);

	auto text = Text::create(name, GameChoice::getInstance().getFontName(), 120);
	button->addChild(text);
	text->setPosition(button->getContentSize() / 2 + Size(0, 250));
	text->setTextColor(Color4B::YELLOW);

	button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType eventType) {
		if (eventType == Widget::TouchEventType::BEGAN)
		{
			giveCoinWithEffect(worth * 5, button->getPosition(), .8f, true);
			button->removeFromParent();
		}
	});
}

void GameplayScene::giveCoinWithEffect(int coin, const Vect& pos, float scale, bool forAdjunct)
{
	auto coinFunc = [=]() {
		_coinsCount += coin;
		GameUser::getInstance().addCoin(coin);
		PlayerPrefs::getInstance().saveCoin();
	};

	auto animFunc = [=]() {
		auto coinImage = _coinText->getParent();
		auto image = ImageView::create("gui/coin.png");
		image->setScale(scale);
		image->setPosition(pos);
		coinImage->getParent()->addChild(image);
		image->runAction(Sequence::createWithTwoActions(MoveTo::create(.3f, coinImage->getPosition()), RemoveSelf::create()));
		coinImage->runAction(Sequence::create(DelayTime::create(.3f), ScaleTo::create(.05f, 1.2f), ScaleTo::create(.05f, 1.0f), nullptr));
	};

	Sequence* seq = nullptr;
	if (!forAdjunct)
		seq = Sequence::create(DelayTime::create(.2f), CallFunc::create(animFunc), DelayTime::create(.1f),
			CallFunc::create(animFunc), DelayTime::create(.1f), CallFunc::create(animFunc),
			DelayTime::create(.2f), CallFunc::create(coinFunc), nullptr);
	else
		seq = Sequence::create(DelayTime::create(.02f), CallFunc::create(animFunc), nullptr);

	_coinText->getParent()->getParent()->runAction(seq);

	//play sound
	if (!forAdjunct)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/earn_money.ogg");
	else
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/coin.ogg");
}

void GameplayScene::goTutorialStep()
{
	if (PlayerPrefs::getInstance().isTutorialFinished(1))
		return;

	_tutorialStep++;

	if (_burgersCount == 2)
	{
		_forceTutDishButton = nullptr;
		PlayerPrefs::getInstance().finishTutrial(1);
	}

	_hudLayout->removeChildByName("TutHand");

	if (_tutorialStep == 5 || _burgersCount == 2)
	{
		auto finishFrame = ImageView::create("tut/frame3.png");
		_hudLayout->addChild(finishFrame);
		finishFrame->setPosition(_hudLayout->getContentSize() / 2 + Size(0, 200));
		finishFrame->setScale(.9f);

		std::string messageName = _tutorialStep == 5 ? "TEXT_FIRST_BURGER" : "TEXT_TUT_FINISH";
		auto finishText = Text::create(GameChoice::getInstance().getString(messageName), GameChoice::getInstance().getFontName(), 50);
		finishFrame->addChild(finishText);
		finishText->setPosition(finishFrame->getContentSize() / 2);
		finishText->setTextColor(Color4B::ORANGE);
		scheduleOnce([=](float dt) {
			finishFrame->removeFromParent();
		}, 2, "RemoveTutorialFinishedText" + StringUtils::toString(_tutorialStep));

		_clockTimer = _initClockTime;

		return;
	}

	if(_tutorialStep == 6)
		_tutorialNodesVec.clear();

	for (auto n : _tutorialNodesVec)
		n->removeFromParent();
	_tutorialNodesVec.clear();

	auto recipeList = _hudLayout->getChildByName("recipeBack")->getChildByName("recipe");

	if (_recipeFoodIndex == recipeList->getChildren().size())
		return;

	auto node = recipeList->getChildren().at(_recipeFoodIndex);
	auto arrowImage = ImageView::create("tut/arrow.png");
	node->addChild(arrowImage);
	arrowImage->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	arrowImage->setPosition(node->getContentSize() / 2 + Size(70, 0));
	arrowImage->runAction(RepeatForever::create(Sequence::createWithTwoActions(MoveBy::create(.4f, Vect(30, 0)), MoveBy::create(.4f, Vect(-30, 0)))));

	_tutorialNodesVec.push_back(arrowImage);

	auto tutFrame = ImageView::create("tut/frame.png");
	tutFrame->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	arrowImage->addChild(tutFrame);
	tutFrame->setPosition(arrowImage->getContentSize() + Size(-70, -20));

	auto tutText = Text::create(GameChoice::getInstance().getString("TEXT_TUT_SELECT_DISH"), GameChoice::getInstance().getFontName(), 40);
	tutFrame->addChild(tutText);
	tutText->setPosition(tutFrame->getContentSize() / 2);
	tutText->setTextColor(Color4B::BLACK);

	bool needShowWrongStep = _tutorialStep == 7 || _tutorialStep == 8;

	FoodTypes foodType = _recipeFoodsVec.at(_recipeFoodIndex);
	for (auto dish : _dishesVec)
	{
		if (needShowWrongStep)
		{
			if ((FoodTypes)dish->getTag() != foodType)
			{
				if((FoodTypes)dish->getTag() == FoodTypes::Bread)
					continue;
				if (dish == _forceTutDishButton)
					continue;
				auto glow = ImageView::create("tut/glow.png");
				glow->setColor(Color3B::RED);
				dish->addChild(glow);
				glow->setPosition(dish->getContentSize() / 2);
				glow->runAction(RepeatForever::create(Sequence::createWithTwoActions(FadeOut::create(.4f), FadeIn::create(.4f))));
				_tutorialNodesVec.push_back(glow);
				_forceTutDishButton = dish;

				auto hand = ImageView::create("tut/hand.png");
				_hudLayout->addChild(hand);
				hand->setPosition(dish->getPosition());
				hand->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
				hand->setName("TutHand");
				hand->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(.4f, 1.2f), ScaleTo::create(.4f, 1))));

				break;
			}
		}
		else
		{
			if ((FoodTypes)dish->getTag() == foodType)
			{
				auto glow = ImageView::create("tut/glow.png");
				glow->setColor(Color3B::GREEN);
				dish->addChild(glow);
				glow->setPosition(dish->getContentSize() / 2);
				glow->runAction(RepeatForever::create(Sequence::createWithTwoActions(FadeOut::create(.4f), FadeIn::create(.4f))));
				_tutorialNodesVec.push_back(glow);
				_forceTutDishButton = dish;

				auto hand = ImageView::create("tut/hand.png");
				_hudLayout->addChild(hand);
				hand->setPosition(dish->getPosition());
				hand->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
				hand->setName("TutHand");
				hand->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(.4f, 1.2f), ScaleTo::create(.4f, 1))));

				break;
			}
		}
	}

	if (_tutorialStep > 1 && _tutorialStep < 4)
	{
		auto tutFrame2 = ImageView::create("tut/frame2.png");
		_clockBar->getParent()->addChild(tutFrame2);
		tutFrame2->setPosition(_clockBar->getPosition() + Vect(-10, 150));
		tutFrame2->setScale(1.05f);
		_tutorialNodesVec.push_back(tutFrame2);

		auto tutText2 = Text::create(GameChoice::getInstance().getString("TEXT_TUT_TIME_REWARD"), GameChoice::getInstance().getFontName(), 40);
		tutFrame2->addChild(tutText2);
		tutText2->setPosition(tutFrame2->getContentSize() / 2);
		tutText2->setTextColor(Color4B::BLACK);
		tutFrame2->runAction(RepeatForever::create(Sequence::createWithTwoActions(MoveBy::create(.4f, Vect(0, -30)), MoveBy::create(.4f, Vect(0, 30)))));
		tutText2->setTextColor(Color4B::GREEN);

		auto arrowImage2 = ImageView::create("tut/arrow2.png");
		tutFrame2->addChild(arrowImage2);
		arrowImage2->setPosition(tutFrame2->getContentSize() / 2 + Size(0, -70));
	}

	if (needShowWrongStep)
	{
		auto tutFrame2 = ImageView::create("tut/frame2.png");
		_clockBar->getParent()->addChild(tutFrame2);
		tutFrame2->setPosition(_clockBar->getPosition() + Vect(-10, 150));
		tutFrame2->setScale(1.05f);
		_tutorialNodesVec.push_back(tutFrame2);

		auto tutText2 = Text::create(GameChoice::getInstance().getString("TEXT_TUT_TIME_PENALTY"), GameChoice::getInstance().getFontName(), 40);
		tutFrame2->addChild(tutText2);
		tutText2->setPosition(tutFrame2->getContentSize() / 2);
		tutText2->setTextColor(Color4B::BLACK);
		tutFrame2->runAction(RepeatForever::create(Sequence::createWithTwoActions(MoveBy::create(.4f, Vect(0, -30)), MoveBy::create(.4f, Vect(0, 30)))));
		tutText2->setTextColor(Color4B::RED);

		auto arrowImage2 = ImageView::create("tut/arrow2.png");
		tutFrame2->addChild(arrowImage2);
		arrowImage2->setPosition(tutFrame2->getContentSize() / 2 + Size(0, -70));
	}
}

void GameplayScene::playCustomerSound()
{
	int randomNumber;
	do
	{
		randomNumber = random(1, 15);
	} while (randomNumber == _lastCustomerSoundNum);
	_lastCustomerSoundNum = randomNumber;
	std::string soundFile = StringUtils::format("sounds/customer/customer_%d.ogg", randomNumber);
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundFile.c_str());
}

void GameplayScene::playCookSound()
{
	int kitchenNumber = (int)GameUser::getInstance().getCurrentKitchen() + 1;
	int randomNumber;
	do
	{
		randomNumber = random(1, kitchenNumber == 3 ? 6 : 3);
	} while (randomNumber == _lastCookSoundNum);
	_lastCookSoundNum = randomNumber;
	std::string soundFile = StringUtils::format("sounds/cook/cook_%d_%d.ogg", kitchenNumber, randomNumber);
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundFile.c_str());
}


void GameplayScene::playHumanSound()
{
	if (!PlayerPrefs::getInstance().isTutorialFinished(1))
		return;

	if (_playSoundCounter == -1)
	{
		_playSoundCounter = random(2, 4);
		_togglePlaySound = random(1, 100) > 50;
	}

	if (_playSoundCounter > 0)
	{
		_playSoundCounter--;
		if (_playSoundCounter == 0)
		{
			_playSoundCounter = random(2, 4);
			_togglePlaySound = !_togglePlaySound;
			if (_togglePlaySound)
				playCookSound();
			else
				playCustomerSound();

			auto quoteSprite = Sprite::create("sounds/quote.png");
			addChild(quoteSprite);
			if (_togglePlaySound)//cook
			{
				quoteSprite->setPosition(_visibleSize / 2 + Size(100, 500));
				quoteSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
			}
			else
			{
				quoteSprite->setPosition(_visibleSize / 2 + Size(400, -200));
				quoteSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_RIGHT);
				quoteSprite->setFlippedX(true);
			}

			for (int i = 0; i < 3; ++i)
			{
				auto quoteDot = Sprite::create("sounds/quoteDot.png");
				quoteSprite->addChild(quoteDot);
				quoteDot->setPosition(quoteSprite->getContentSize() / 2 + Size(-30 + i * 30, 25));
				quoteDot->setVisible(false);
			}

			quoteSprite->runAction(Sequence::create(
				DelayTime::create(.2f), CallFunc::create([=]() {quoteSprite->getChildren().at(0)->setVisible(true); }),
				DelayTime::create(.2f), CallFunc::create([=]() {quoteSprite->getChildren().at(1)->setVisible(true); }),
				DelayTime::create(.2f), CallFunc::create([=]() {quoteSprite->getChildren().at(2)->setVisible(true); }),
				nullptr));

			quoteSprite->setScale(.2f);
			quoteSprite->runAction(Sequence::create(ScaleTo::create(.1f, 1), DelayTime::create(1.2f), RemoveSelf::create(), nullptr));
		}
	}
}