#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "General.h"

class MenuScene : public cocos2d::Layer
{
public:
	MenuScene();
	virtual ~MenuScene();

	CREATE_FUNC_DATA(MenuScene);
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
	void onEnter() override;
	void update(float dt) override;

private:

	void buttonCallback(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType eventType);

	cocos2d::Vect _visibleOrigin;
	cocos2d::Size _visibleSize;

	cocos2d::ui::Button* _playButton;
	cocos2d::ui::Button* _soundButton;
	cocos2d::ui::Button* _settingButton;
	cocos2d::ui::Button* _exitButton;

	cocos2d::ui::Button* _foodButton;


};

#endif //__MENU_SCENE_H__
