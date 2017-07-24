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

	static cocos2d::Scene* createSceneData(/*int stageLevel, int stageNumber*/)
	{
		cocos2d::ValueMap initData;
		/*initData["stageNumber"] = stageNumber;
		initData["stageLevel"] = stageLevel;*/
		return createScene(initData);
	}

protected:
	virtual bool init(cocos2d::ValueMap& initData);
	virtual void onEnter() override;
	virtual void update(float delta) override;

private:
	void onKeyReleasedCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void createHud();
	void dishButtonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType);
	void generateRecipe();
	void startGame();

	cocos2d::Size _visibleSize;
	cocos2d::Vect _origin;

	cocos2d::ui::Layout* _hudLayout = nullptr;
	cocos2d::Vector<cocos2d::ui::Button*> _dishesVec;
	cocos2d::ui::LoadingBar* _clockBar;

	std::vector<FoodTypes> _recipeFoodsVec;
	std::vector<FoodTypes> _availableFoodsVec;
	int _recipeFoodIndex;

	cocos2d::Node* _makedMeal;

	float _clockTimer;
	float _gameTime;
	bool _gameStarted = false;

};

#endif //__GAMEPLAY_SCENE_H__