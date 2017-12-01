#include "GameplayScene.h"
#include "GameChoice.h"
#include "MenuScene.h"
#include "ShopScene.h"
#include "PlayerPrefs.h"
#include "GameUser.h"


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
	_makeBurgerTime = GameChoice::getInstance().getMakeBurgerTime();
	_comboIncTime = 12;
	_comboDecTime = 4;

	_visibleSize = Director::getInstance()->getVisibleSize();
	_origin = Director::getInstance()->getVisibleOrigin();

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

	auto pauseBackg = ImageView::create("gui/pauseBackg.png");
	_pauseLayout->addChild(pauseBackg);
	pauseBackg->setPosition(Size(_pauseLayout->getContentSize().width / 2, _pauseLayout->getContentSize().height - 500));

	auto pauseHeader = ImageView::create("gui/pauseHeader.png");
	pauseHeader->setName("pauseHeader");
	pauseBackg->addChild(pauseHeader);
	pauseHeader->setPosition(Vect(pauseBackg->getContentSize().width / 2, pauseBackg->getContentSize().height - pauseHeader->getContentSize().height / 2));

	auto gameOverHeader = ImageView::create("gui/pauseGameOver.png");
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

	auto resumeButton = Button::create("gui/resumeButton.png");
	resumeButton->setName("resume");
	pauseBackg->addChild(resumeButton);
	resumeButton->setPosition(pauseBackg->getContentSize() / 2 + Size(-150, buttonsPosY));
	resumeButton->addTouchEventListener(CC_CALLBACK_2(GameplayScene::pauseButtonCallback, this));
	resumeButton->setScale(buttonsScale);

	auto retryButton = Button::create("gui/retryButton.png");
	retryButton->setName("retry");
	pauseBackg->addChild(retryButton);
	retryButton->setPosition(resumeButton->getPosition());
	retryButton->addTouchEventListener(CC_CALLBACK_2(GameplayScene::pauseButtonCallback, this));
	retryButton->setScale(buttonsScale);

	auto menuButton = Button::create("gui/menuButton.png");
	menuButton->setName("menu");
	pauseBackg->addChild(menuButton);
	menuButton->setPosition(pauseBackg->getContentSize() / 2 + Size(0, buttonsPosY));
	menuButton->addTouchEventListener(CC_CALLBACK_2(GameplayScene::pauseButtonCallback, this));
	menuButton->setScale(buttonsScale);

	auto shopButton = Button::create("gui/shopButton.png");
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

	auto clockFrame = ImageView::create("gui/clock.png");
	clockFrame->setName("clock");
	statsLayout->addChild(clockFrame);
	clockFrame->setPosition(statsLayout->getContentSize() / 2 + Size(0, 100));
	auto clockText = Text::create("12:12", GameChoice::getInstance().getFontName(), 60);
	clockFrame->addChild(clockText);
	clockText->setPosition(clockFrame->getContentSize() / 2 + Size(60, 0));
	clockText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	//clockText->setAnchorPoint(Point(0, .5f));
	clockText->enableOutline(Color4B::GRAY, 2);

	auto moneyFrame = ImageView::create("gui/money.png");
	moneyFrame->setName("money");
	statsLayout->addChild(moneyFrame);
	moneyFrame->setPosition(statsLayout->getContentSize() / 2 + Size(0, 0));
	auto moneyText = Text::create("150000", GameChoice::getInstance().getFontName(), 60);
	moneyFrame->addChild(moneyText);
	moneyText->setPosition(moneyFrame->getContentSize() / 2 + Size(-20, 0));
	moneyText->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	//moneyText->setAnchorPoint(Point(0, .5f));
	moneyText->enableOutline(Color4B::GRAY, 2);

	auto bonusFrame = ImageView::create("gui/bonus.png");
	bonusFrame->setName("money");
	statsLayout->addChild(bonusFrame);
	bonusFrame->setPosition(statsLayout->getContentSize() / 2 + Size(0, -100));
	auto bonusText = Text::create("150000", GameChoice::getInstance().getFontName(), 60);
	bonusFrame->addChild(bonusText);
	bonusText->setPosition(bonusFrame->getContentSize() / 2 + Size(-20, 0));
	bonusText->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	//bonusText->setAnchorPoint(Point(0, .5f));
	bonusText->enableOutline(Color4B::GRAY, 2);

	_pauseLayout->setVisible(false);
	//

	return true;
}

void GameplayScene::onEnter()
{
	Layer::onEnter();

	scheduleUpdate();
}

void GameplayScene::onKeyReleasedCallback(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		showPausePage(true, false);
	}
}

void GameplayScene::createHud()
{
	_hudLayout = Layout::create();
	addChild(_hudLayout);
	_hudLayout->setContentSize(_visibleSize);
	_hudLayout->setAnchorPoint(Point(.5f, .5f));
	_hudLayout->setPosition(_visibleSize / 2);

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

	createRecipeAndDishes();


	auto coinImage = ImageView::create("gui/coin.png");
	_hudLayout->addChild(coinImage);
	coinImage->setPosition(_visibleSize - coinImage->getContentSize() / 2);

	_coinText = Text::create(StringUtils::format("%d %s", _coinsCount, "x"), "fonts/Marker Felt.ttf", 50);
	coinImage->addChild(_coinText);
	_coinText->setPosition(coinImage->getContentSize() / 2 + Size(-55, 0));
	_coinText->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	_coinText->setAnchorPoint(Point(1, .5f));

	auto pauseButton = Button::create("gui/pauseButton.png");
	_hudLayout->addChild(pauseButton);
	pauseButton->setName("pause");
	pauseButton->setPosition(Vect(pauseButton->getContentSize().width / 2, _visibleSize.height - pauseButton->getContentSize().height / 2));
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

	auto comboText = Text::create("Combo", "fonts/Marker Felt.ttf", 32);
	comboBarFrame->addChild(comboText);
	comboText->setTextColor(Color4B::YELLOW);
	comboText->setPosition(_comboBar->getPosition() + Vect(0, 30));
	comboBarFrame->setVisible(false);

	_standbyLayout = Layout::create();
	_hudLayout->addChild(_standbyLayout);
	_standbyLayout->setContentSize(_visibleSize);
	_standbyLayout->setSwallowTouches(true);
	_standbyLayout->setTouchEnabled(true);

	_standbyLayout->setBackGroundColor(Color3B::BLACK);
	_standbyLayout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	_standbyLayout->setBackGroundColorOpacity(100);

	auto readyButton = Button::create("gui/ready.png");
	_standbyLayout->addChild(readyButton);
	readyButton->setPosition(_standbyLayout->getContentSize() / 2 + Size(0, 100));

	readyButton->addTouchEventListener([=](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
	{
		_hudLayout->removeChild(_standbyLayout);
		startGame();
	});
}

void GameplayScene::dishButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == Widget::TouchEventType::ENDED)
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

			if (!_comboIsActive)
			{
				_clockTimer += _rightFoodTime;
				if (_clockTimer >= _makeBurgerTime)
				{
					_comboIsActive = true;
					_clockTimer = _makeBurgerTime + _comboIncTime;
					_comboBar->getParent()->setVisible(true);
					_comboBar->setPercent(100);
				}
			}

			auto recipeList = static_cast<Layout*>(_hudLayout->getChildByName("recipeBack")->getChildByName("recipe"));
			auto foodLayout = recipeList->getChildren().at(_recipeFoodIndex);

			auto trueSign = foodLayout->getChildByName("trueSign");
			auto falseSign = foodLayout->getChildByName("falseSign");
			trueSign->setVisible(true);
			falseSign->setVisible(false);

			//play right sound
			//TODO

			FoodFactory::getInstance().consumeFood(food->getType());

			_recipeFoodIndex++;

			auto foodCountLabel = static_cast<Label*>(button->getChildByName("FoodsCount"));
			foodCountLabel->setString(StringUtils::toString(food->getCount()));
			if (food->getCount() == 0)
				foodCountLabel->setTextColor(Color4B::RED);
		}
		else
		{
			playCookAnimation("loss", false);

			if (!_comboIsActive)
			{
				_clockTimer -= _wrongFoodTime;
			}
			else
			{
				_comboIsActive = false;
				_clockTimer = _makeBurgerTime - _comboDecTime;
				_comboBar->getParent()->setVisible(false);
			}

			auto recipeList = static_cast<Layout*>(_hudLayout->getChildByName("recipeBack")->getChildByName("recipe"));
			auto foodLayout = recipeList->getChildren().at(_recipeFoodIndex);

			auto trueSign = foodLayout->getChildByName("trueSign");
			auto falseSign = foodLayout->getChildByName("falseSign");
			trueSign->setVisible(false);
			falseSign->setVisible(true);

			//play wrong sound
			//TODO
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
	_availableFoodsVec.push_back(FoodTypes::Bread);
	FoodTypes foodType = FoodTypes::None;
	bool exist = false;
	do
	{
		int randomFood = random(0, (int)unlockedFoodsVec.size() - 1);
		foodType = (FoodTypes)unlockedFoodsVec.at(randomFood);
		exist = std::find(_availableFoodsVec.begin(), _availableFoodsVec.end(), foodType) != _availableFoodsVec.end();
		if (!exist)
			_availableFoodsVec.push_back(foodType);
	} while (_availableFoodsVec.size() < Dishes_Count);


	float initX = _visibleSize.width / 2 - 200;
	Vect dishOffset(initX, 200);
	for (int i = 0; i < Dishes_Count; ++i)
	{
		auto dishButton = Button::create("dishes/dish_1.png");
		FoodTypes foodType = _availableFoodsVec.at(i);
		auto food = FoodFactory::getInstance().getFood(foodType);
		auto foodImage = ImageView::create(food->getIconPath());
		dishButton->addChild(foodImage);
		foodImage->setScale(.3);
		foodImage->setPosition(dishButton->getContentSize() / 2);

		dishButton->setTag((int)foodType);

		dishButton->setName(StringUtils::format("dish__%d", i));

		_hudLayout->addChild(dishButton);
		dishButton->setPosition(dishOffset);

		auto foodCountLabel = Label::createWithTTF(StringUtils::toString(food->getCount()), GameChoice::getInstance().getFontName(), 50);
		foodCountLabel->setName("FoodsCount");
		dishButton->addChild(foodCountLabel);
		foodCountLabel->setPosition(dishButton->getContentSize() / 2 + Size(0, 80));
		foodCountLabel->setTextColor(Color4B(100, 100, 100, 255));
		if (food->getCount() == 0)
			foodCountLabel->setTextColor(Color4B::RED);

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

	int foodsCount = cocos2d::random(3, 8);
	for (int f = 0; f < foodsCount; ++f)
	{
		FoodTypes foodType = _availableFoodsVec.at(random(1, (int)_availableFoodsVec.size() - 1));
		_recipeFoodsVec.push_back(foodType);
	}

	_recipeFoodsVec.push_back(FoodTypes::Bread);

	auto recipeList = _hudLayout->getChildByName("recipeBack")->getChildByName("recipe");

	recipeList->removeAllChildren();

	for (int i = 0; i < _recipeFoodsVec.size(); ++i)
	{
		auto food = FoodFactory::getInstance().getFood(_recipeFoodsVec.at(i));
		auto layout = Layout::create();
		layout->setContentSize(Size(170, 25));
		auto item = ImageView::create(makeIconPath(food->getType(), food->getIconPath(), i == 0));
		item->setScale(.25f);
		item->setPosition(layout->getContentSize() / 2);
		layout->addChild(item);
		recipeList->addChild(layout);
		layout->setPosition(Vec2(0, (i + 1) * layout->getContentSize().height * 1.2f));

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
}

void GameplayScene::startGame()
{
	_burgerTime = GameChoice::getInstance().getMakeBurgerTime();
	_clockTimer = _burgerTime;
	_gameStarted = true;
	playCookAnimation("idle", true);
}

void GameplayScene::update(float delta)
{
	Layer::update(delta);

	if (_gameStarted && !_isGameOver && _foodFinishedLayout == nullptr)
	{
		_clockTimer -= delta;

		if (_comboIsActive)
		{
			float barPercent = (_clockTimer - _makeBurgerTime) / _comboIncTime * 100;
			_comboBar->setPercent(barPercent);

			if (_clockTimer <= _makeBurgerTime)
			{
				_clockTimer = _makeBurgerTime - _comboDecTime;
				_comboIsActive = false;
				_comboBar->getParent()->setVisible(false);
			}
		}

		float barPercent = _clockTimer / _burgerTime * 100;
		_clockBar->setPercent(barPercent);

		_coinText->setString(StringUtils::format("%d %s", _coinsCount, "x"));

		if (_clockTimer <= 0)
		{
			gameOver();
		}
	}
}

void GameplayScene::gameOver()
{
	_isGameOver = true;
	showPausePage(true, true);
}

void GameplayScene::packBurger(float dt)
{
	//pack and give
	Vect burgerPos = _burger->getPosition();
	auto func1 = CallFunc::create([=]() {
		//pack animation code
		int coin = 0;
		for (auto foodType : _availableFoodsVec)
		{
			auto food = FoodFactory::getInstance().getFood(foodType);
			coin += food->getWorth();
		}
		if (!_comboIsActive)
			coin *= 2;

		KitchenTypes currentKitchen = GameUser::getInstance().getCurrentKitchen();
		int cookCoinCoef = Inventories::getInstance().getKitchenByType(currentKitchen)->getValue();
		coin *= cookCoinCoef;

		_coinsCount += _recipeFoodsVec.size() * coin;
	});

	auto move1 = MoveTo::create(.3f, burgerPos + Vect(500, 0));
	auto move2 = MoveTo::create(.0f, burgerPos);
	auto seq = Sequence::create(func1, DelayTime::create(.2f), move1, CallFunc::create([=]() {_burger->removeAllChildren(); }), move2, nullptr);
	_burger->runAction(seq);


	auto recipeBack = _hudLayout->getChildByName("recipeBack");
	Vec2 recipePos = recipeBack->getPosition();

	move1 = MoveTo::create(.3f, recipePos + Vect(-500, 0));
	move2 = MoveTo::create(.0f, recipePos + Vect(0, 500));
	auto move3 = MoveTo::create(.3f, recipePos);
	seq = Sequence::create(DelayTime::create(.2f), move1, move2, CallFunc::create([=]() {createRecipeAndDishes(); _recipeFoodIndex = 0; }), EaseSineInOut::create(move3), nullptr);
	recipeBack->runAction(seq);

	if (!_comboIsActive)
		_clockTimer += GameChoice::getInstance().getPackBurgerTime();

	return;
}

void GameplayScene::onExit()
{
	Layer::onExit();
	PlayerPrefs::getInstance().saveFoods();
}

void GameplayScene::pauseButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
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
			showPausePage(false, false);
			auto scene = TransitionSlideInR::create(.5f, ShopScene::createSceneData(ShopTypes::Food));
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
			Director::getInstance()->resume();
			auto scene = TransitionSlideInL::create(.5f, MenuScene::createSceneData());
			Director::getInstance()->replaceScene(scene);
		}
	}
}

void GameplayScene::showPausePage(bool show, bool gameOver)
{
	auto pauseBackg = _pauseLayout->getChildren().at(0);
	pauseBackg->getChildByName("resume")->setVisible(!gameOver);
	pauseBackg->getChildByName("retry")->setVisible(gameOver);
	pauseBackg->getChildByName("stats")->setVisible(gameOver);
	pauseBackg->getChildByName("pauseHeader")->setVisible(!gameOver);
	pauseBackg->getChildByName("gameOverHeader")->setVisible(gameOver);
	pauseBackg->getChildByName("pauseText")->setVisible(!gameOver);
	_pauseLayout->setVisible(show);

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

	cookSpriteBatch->setScale(1.5f);

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

	auto animate = Animate::create(animation);
	spr->runAction(animate);

	if (animationName == "loss" || animationName == "ok")
	{
		float duration = animate->getDuration();
		scheduleOnce([=](float dt) {
			playCookAnimation("idle", true);
		}, duration, "Idle_After_Loss_Ok");
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
					foodCountLabel->setTextColor(Color4B(100, 100, 100, 255));
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

