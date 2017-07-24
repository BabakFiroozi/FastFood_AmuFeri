#ifndef __GENERAL_H__
#define __GENERAL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"


#define CREATE_FUNC_DATA(__TYPE__) \
static __TYPE__* create(cocos2d::ValueMap& initData) \
{ \
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet && pRet->init(initData)) \
{ \
	pRet->autorelease(); \
	return pRet; \
} \
else \
{ \
	delete pRet; \
	pRet = NULL; \
	return NULL; \
} \
}

#define CREATESCENE_FUNC_DATA() \
static cocos2d::Scene* createScene(cocos2d::ValueMap& initData) \
{ \
	cocos2d::Scene* scene = nullptr; \
	auto layer = create(initData); \
if (layer != nullptr) \
{ \
	scene = cocos2d::Scene::create(); \
	scene->addChild(layer); \
} \
	return scene; \
} \


class GameUtils
{
public:
	static std::string timeToString(int seconds)
	{
		int hour = seconds / 3600;
		int min = (seconds % 3600) / 60;
		int sec = seconds % 60;
		return cocos2d::StringUtils::format("%02d : %02d : %02d", hour, min, sec);
	}
};

#endif //__GENERAL_H__