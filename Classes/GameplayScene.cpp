#include "GameplayScene.h"
#include "GameChoice.h"

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

	FoodFactory::getInstance().initialize();

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
	table->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height - 550));

	createHud();

	return true;
}

void GameplayScene::onEnter()
{
	Layer::onEnter();

	_makedMeal = Node::create();
	_makedMeal->setPosition(Vec2(_visibleSize.width / 2 + 145, _visibleSize.height - 490));
	addChild(_makedMeal);

	scheduleUpdate();

	scheduleOnce([=](float dt)
	{
		startGame();
	}, 1, "startGame");
}

void GameplayScene::onKeyReleasedCallback(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
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
	recipeBack->setPosition(Vect(_visibleSize.width / 2 - 280, _visibleSize.height - recipeBack->getContentSize().height / 2 + 30));
	auto recipeBack2 = ImageView::create("cooks/recipeBack_2.png");
	recipeBack->addChild(recipeBack2);
	recipeBack2->setPosition(recipeBack->getContentSize() / 2);
	recipeBack2->setOpacity(200);

	recipeBack->setScale(.9f);

	auto recipeList = ListView::create();
	recipeBack->addChild(recipeList);
	recipeList->setName("recipe");
	recipeList->setContentSize(recipeBack2->getContentSize() - Size(20, 180));
	recipeList->setPosition(Vect(5, 5));
	//recipesList->setInnerContainerSize()
	//recipeList->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	//recipeList->setBackGroundColor(Color3B::RED);
	//recipeList->setBackGroundColorOpacity(100);
	recipeList->setScrollBarEnabled(false);	

	//foods on dishes
	_availableFoodsVec.push_back(FoodTypes::Food_1);
	_availableFoodsVec.push_back(FoodTypes::Food_2);
	_availableFoodsVec.push_back(FoodTypes::Food_3);
	_availableFoodsVec.push_back(FoodTypes::Food_4);
	_availableFoodsVec.push_back(FoodTypes::Food_5);

	float initX = _visibleSize.width / 2 - 100;
	Vect dishOffset(initX, _visibleSize.height - 800);
	for (int i = 0; i < 5; ++i)
	{
		auto dishButton = Button::create("dishes/dish_1.png");

		FoodTypes foodType = _availableFoodsVec.at(i);
		auto food = FoodFactory::getInstance().getFood(foodType);
		auto foodImage = ImageView::create(food.getIconPath());
		dishButton->addChild(foodImage);
		foodImage->setScale(.25f);
		foodImage->setPosition(dishButton->getContentSize() / 2);

		dishButton->setUserData(new FoodTypes(foodType));

		//dish->setName("bread");
		_hudLayout->addChild(dishButton);
		dishButton->setPosition(dishOffset);

		Size size = dishButton->getContentSize();

		dishOffset.x += size.width;
		if (i == 1)
		{
			dishOffset.x = initX - size.width / 2;
			dishOffset.y -= size.height;
		}

		dishButton->addTouchEventListener(CC_CALLBACK_2(GameplayScene::dishButtonCallback, this));

		auto clockBack = ImageView::create("dishes/clockBack.png");
		_hudLayout->addChild(clockBack);
		clockBack->setPosition(Vec2(_visibleSize.width / 2, _visibleSize.height - 670));

		auto clockBar = LoadingBar::create("dishes/clockBar.png");
		clockBack->addChild(clockBar);
		clockBar->setPosition(clockBack->getContentSize() / 2 + Size(52, 2));
		clockBar->setPercent(100);
		_clockBar = clockBar;

		auto clockImage = ImageView::create("dishes/clock.png");
		clockBack->addChild(clockImage);
		clockImage->setPosition(Vec2(clockImage->getContentSize().width / 2, clockBack->getContentSize().height / 2));

		clockBack->setScale(.8f);

		generateRecipe();
	}
}

void GameplayScene::dishButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (eventType == Widget::TouchEventType::ENDED)
	{
		if (!_gameStarted)
			return;

		if (_recipeFoodIndex == _recipeFoodsVec.size())
			return;

		auto button = static_cast<Button*>(sender);
		FoodTypes selFood = *(FoodTypes*)button->getUserData();
		FoodTypes needFood = _recipeFoodsVec.at(_recipeFoodIndex);
		bool rightFood = selFood == needFood;

		if (rightFood)
		{
			_recipeFoodIndex++;

			auto food = FoodFactory::getInstance().getFood(needFood);
			auto foodSprite = Sprite::create(food.getIconPath());
			Vec2 pos = Vec2::ZERO;
			if (_makedMeal->getChildrenCount() > 0)
			{
				auto lastFoodSprite = _makedMeal->getChildren().at(_makedMeal->getChildrenCount() - 1);
				pos = lastFoodSprite->getPosition();
				pos.y += 40;
			}
			_makedMeal->addChild(foodSprite);
			foodSprite->setPosition(pos);
			foodSprite->setScale(.35f);
		}
	}
}

void GameplayScene::generateRecipe()
{
	_recipeFoodsVec.clear();

	_recipeFoodsVec.push_back(FoodTypes::Food_1);
	_recipeFoodsVec.push_back(FoodTypes::Food_4);
	_recipeFoodsVec.push_back(FoodTypes::Food_5);
	_recipeFoodsVec.push_back(FoodTypes::Food_2);
	_recipeFoodsVec.push_back(FoodTypes::Food_3);

	auto recipeList = static_cast<ListView*>(_hudLayout->getChildByName("recipeBack")->getChildByName("recipe"));

	recipeList->removeAllChildren();

	for (int i = _recipeFoodsVec.size() - 1; i >= 0; --i)
	{
		auto f = _recipeFoodsVec.at(i);
		auto food = FoodFactory::getInstance().getFood(f);
		auto layout = Layout::create();
		layout->setContentSize(Size(170, 120));
		auto item = ImageView::create(food.getIconPath());
		item->setScale(.25f);
		item->setPosition(layout->getContentSize() / 2);
		layout->addChild(item);
		recipeList->pushBackCustomItem(layout);
	}

	recipeList->scrollToBottom(0, false);

	_recipeFoodIndex = 0;

}

void GameplayScene::startGame()
{
	_gameTime = GameChoice::getInstance().getGameplayTime();
	_clockTimer = _gameTime;
	_gameStarted = true;
}

void GameplayScene::update(float delta)
{
	Layer::update(delta);

	if (_gameStarted)
	{
		_clockTimer -= delta;

		float barPercent = _clockTimer / _gameTime * 100;
		_clockBar->setPercent(barPercent);

		if (_clockTimer <= 0)
		{

		}
	}
}

