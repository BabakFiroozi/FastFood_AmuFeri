#pragma once

#include "General.h"

class SplashScene : public cocos2d::LayerColor
{
public:
	SplashScene();
	virtual ~SplashScene();

	CREATE_FUNC(SplashScene);

	static cocos2d::Scene* createScene()
	{ 
		cocos2d::Scene* scene = nullptr;
		auto layer = SplashScene::create();
		if (layer != nullptr)
		{
			scene = cocos2d::Scene::create();
			scene->addChild(layer);
		}
			return scene;
	}


protected:
	bool init() override;
	void onEnter() override;

private:
	void goMenu(float dt);
	void showLogo(float dt);

	float _duration = 0;
};