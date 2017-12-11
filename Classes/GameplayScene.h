#ifndef __GAMEPLAY_SCENE_H__
#define __GAMEPLAY_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "json/document.h"
#include "General.h"
#include "FoodFactory.h"

class GameplayScene : public cocos2d::Layer
{
public:
	GameplayScene();
	virtual ~GameplayScene();

	CREATE_FUNC_DATA(GameplayScene);
	CREATESCENE_FUNC_DATA();

	static cocos2d::Scene* createSceneData(int sceneNumber)
	{
		cocos2d::ValueMap initData;
		initData["sceneNumber"] = sceneNumber;
		return createScene(initData);
	}

protected:
	virtual bool init(cocos2d::ValueMap& initData);
	void onEnter() override;
	void onExit() override;
	void update(float delta) override;

private:
	void onKeyReleasedCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void createHud();
	void dishButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType);
	void pauseButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType);
	void createRecipeAndDishes();
	void startGame();
	void gameOver();
	void packBurger(float dt);
	void showPausePage(bool show, bool gameOver);
	std::string makeIconPath(const FoodTypes foodType, const std::string& iconPath, bool cond);


	cocos2d::Sprite* createCookAnimation(const std::string& animationName, const int framesCount, float delay = 0.0f, unsigned int loops = 1U);
	void playCookAnimation(const std::string& animationName, bool loop);

	void onFoodFinished(FoodTypes foodType);

	void createAdjunct();

	void coinEffect(int coin, const cocos2d::Vect& pos, float scale, bool forAdjunct = false);

	cocos2d::Size _visibleSize;
	cocos2d::Vect _visibleOrigin;

	cocos2d::ui::Layout* _hudLayout = nullptr;
	cocos2d::Vector<cocos2d::ui::Button*> _dishesVec;
	cocos2d::ui::LoadingBar* _clockBar;
	cocos2d::ui::LoadingBar* _comboBar;

	cocos2d::ui::Layout* _standbyLayout;
	cocos2d::ui::Layout* _pauseLayout;

	std::vector<FoodTypes> _recipeFoodsVec;
	std::vector<FoodTypes> _availableFoodsVec;

	cocos2d::ui::Text* _coinText;
	int _coinsCount = 0;;
	int _burgersCount = 0;

	float _playDuration = 0;

	int _recipeFoodIndex;

	cocos2d::Node* _burger;

	float _clockTimer;
	bool _gameStarted = false;
	bool _isGameOver = false;
	bool _comboIsActive = false;

	float _rightFoodTime;
	float _wrongFoodTime;
	float _initClockTime;
	float _comboIncTime;
	float _comboDecTime;

	std::map<std::string, cocos2d::Sprite*> _animationsMap;
	int _sceneNumber;

	cocos2d::ui::Layout* _foodFinishedLayout = nullptr;

	float _clockDecerementRate = .5f;
};

#endif //__GAMEPLAY_SCENE_H__