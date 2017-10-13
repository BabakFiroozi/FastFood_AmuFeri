#include "GameplayScene.h"
#include "GameChoice.h"
#include "MenuScene.h"
#include "ShopScene.h"

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

	auto shelf = Sprite::create("kitchens/shelf_1.png");
	addChild(shelf);
	shelf->setPosition(Vect(_visibleSize.width / 2, _visibleSize.height - shelf->getContentSize().height / 2));

	auto cook = Sprite::create("cooks/cook_1.png");
	addChild(cook);
	cook->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height - 300));

	auto table = Sprite::create("kitchens/table_1.png");
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
	auto clockText = Text::create("12:12", GameChoice::getInstance().getFontName(), 40);
	clockFrame->addChild(clockText);
	clockText->setPosition(clockFrame->getContentSize() / 2 + Size(60, 0));
	clockText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	//clockText->setAnchorPoint(Point(0, .5f));

	auto moneyFrame = ImageView::create("gui/money.png");
	moneyFrame->setName("money");
	statsLayout->addChild(moneyFrame);
	moneyFrame->setPosition(statsLayout->getContentSize() / 2 + Size(0, 0));
	auto moneyText = Text::create("150000", GameChoice::getInstance().getFontName(), 40);
	moneyFrame->addChild(moneyText);
	moneyText->setPosition(moneyFrame->getContentSize() / 2 + Size(0, 0));
	moneyText->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	//moneyText->setAnchorPoint(Point(0, .5f));

	auto bonusFrame = ImageView::create("gui/bonus.png");
	bonusFrame->setName("money");
	statsLayout->addChild(bonusFrame);
	bonusFrame->setPosition(statsLayout->getContentSize() / 2 + Size(0, -100));
	auto bonusText = Text::create("150000", GameChoice::getInstance().getFontName(), 40);
	bonusFrame->addChild(bonusText);
	bonusText->setPosition(bonusFrame->getContentSize() / 2 + Size(0, 0));
	bonusText->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	//bonusText->setAnchorPoint(Point(0, .5f));

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

	auto recipeBack = ImageView::create("cooks/recipeBack.png");
	_hudLayout->addChild(recipeBack);
	recipeBack->setName("recipeBack");
	recipeBack->setPosition(Vect(_visibleSize.width / 2 - 270, _visibleSize.height - recipeBack->getContentSize().height / 2 + 30));
	auto recipeBack2 = ImageView::create("cooks/recipeBack_2.png");
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
		FoodTypes selFood = *(FoodTypes*)button->getUserData();
		FoodTypes needFood = _recipeFoodsVec.at(_recipeFoodIndex);

		if (selFood == FoodTypes::Bread_Top && needFood == FoodTypes::Bread_Neath)
			selFood = needFood;

		bool rightFood = selFood == needFood;


		if (rightFood)
		{
			auto food = FoodFactory::getInstance().getFood(needFood);
			auto foodSprite = Sprite::create(food.getIconPath());
			Vec2 pos = Vec2::ZERO;
			if (_burger->getChildrenCount() > 0)
			{
				auto lastFoodSprite = _burger->getChildren().at(_burger->getChildrenCount() - 1);
				pos = lastFoodSprite->getPosition();
				pos.y += 30;
			}
			_burger->addChild(foodSprite);
			foodSprite->setPosition(pos);
			foodSprite->setScale(.35f);

			auto scale1 = ScaleBy::create(.1f, 1.15f);
			auto scale2 = ScaleBy::create(.1f, 1.0f / 1.15f);
			foodSprite->runAction(Sequence::create(scale1, EaseBounceOut::create(scale2), nullptr));

			if (_recipeFoodIndex == _recipeFoodsVec.size() - 1)
			{
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
			auto lineImage = foodLayout->getChildByName("line");
			lineImage->setVisible(true);
			lineImage->setColor(Color3B::GREEN);
			//play right sound

			_recipeFoodIndex++;
		}
		else
		{
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
			auto lineImage = foodLayout->getChildByName("line");
			lineImage->setVisible(true);
			lineImage->setColor(Color3B::RED);
			//play wrong sound
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

	//foods on dishes
	_availableFoodsVec.push_back(FoodTypes::Bread_Top);
	FoodTypes foodType = FoodTypes::None;
	bool exist = false;
	do 
	{
		foodType = (FoodTypes)random((int)FoodTypes::Sauce, (int)FoodTypes::Count - 1);
		exist = std::find(_availableFoodsVec.begin(), _availableFoodsVec.end(), foodType) != _availableFoodsVec.end();
		if(!exist)
			_availableFoodsVec.push_back(foodType);
	} while (_availableFoodsVec.size() < Dishes_Count);


	float initX = _visibleSize.width / 2 - 200;
	Vect dishOffset(initX, 200);
	for (int i = 0; i < Dishes_Count; ++i)
	{
		auto dishButton = Button::create("dishes/dish_1.png");
		FoodTypes foodType = _availableFoodsVec.at(i);
		auto food = FoodFactory::getInstance().getFood(foodType);
		auto foodImage = ImageView::create(food.getIconPath());
		dishButton->addChild(foodImage);
		foodImage->setScale(.3);
		foodImage->setPosition(dishButton->getContentSize() / 2);

		auto typePtr = new FoodTypes(foodType);
		dishButton->setUserData(typePtr);
		_userDataVec.push_back(typePtr);

		dishButton->setName(StringUtils::format("dish__%d", i));

		_hudLayout->addChild(dishButton);
		dishButton->setPosition(dishOffset);

		Size size = dishButton->getContentSize();

		dishOffset.x += size.width;
		if (i == 2)
		{
			dishOffset.x = initX + size.width / 2;
			dishOffset.y += size.height;
		}

		dishButton->addTouchEventListener(CC_CALLBACK_2(GameplayScene::dishButtonCallback, this));
	}
	//dishes

	_recipeFoodsVec.clear();

	_recipeFoodsVec.push_back(FoodTypes::Bread_Neath);

	int foodsCount = cocos2d::random(3, 6);
	for (int f = 0; f < foodsCount; ++f)
	{
		FoodTypes foodType = _availableFoodsVec.at(random(1, (int)_availableFoodsVec.size() - 1));
		_recipeFoodsVec.push_back(foodType);
	}

	_recipeFoodsVec.push_back(FoodTypes::Bread_Top);

	auto recipeList = _hudLayout->getChildByName("recipeBack")->getChildByName("recipe");

	recipeList->removeAllChildren();

	for (int i = 0; i < _recipeFoodsVec.size(); ++i)
	{
		auto f = _recipeFoodsVec.at(i);
		auto food = FoodFactory::getInstance().getFood(f);
		auto layout = Layout::create();
		layout->setContentSize(Size(170, 25));
		auto item = ImageView::create(food.getIconPath());
		item->setScale(.25f);
		item->setPosition(layout->getContentSize() / 2);
		layout->addChild(item);
		recipeList->addChild(layout);
		layout->setPosition(Vec2(0, (i + 1) * layout->getContentSize().height * .85f));

		auto line = ImageView::create("gui/line.png");
		line->setName("line");
		layout->addChild(line);
		line->setPosition(layout->getContentSize() / 2 + Size(0, -8));
		line->setScaleX(40);
		line->setVisible(false);
	}

	_recipeFoodIndex = 0;
}

void GameplayScene::startGame()
{
	_burgerTime = GameChoice::getInstance().getMakeBurgerTime();
	_clockTimer = _burgerTime;
	_gameStarted = true;
}

void GameplayScene::update(float delta)
{
	Layer::update(delta);

	if (_gameStarted && !_isGameOver)
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
			coin += food.getWorth();
		}
		if (!_comboIsActive)
			coin *= 2;
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

	for (auto p : _userDataVec)
		delete p;
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
	_pauseLayout->setVisible(show);

	if (show)
		Director::getInstance()->pause();
	else
		Director::getInstance()->resume();
}

