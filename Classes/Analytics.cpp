//
// Created by Babak on 1/21/2018.
//

#include "Analytics.h"
#include "GameChoice.h"

USING_NS_CC;

Analytics::Analytics()
{
}

Analytics::~Analytics()
{
}

Analytics& Analytics::getInstance()
{
    static Analytics instance;
    return instance;
}

void Analytics::logEvent(const char *name, const char *value)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    JniMethodInfo mInfo;
    if (JniHelper::getStaticMethodInfo(mInfo, "org/cocos2dx/cpp/AnalyticsWrapper", "logEvent", "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring name_ = mInfo.env->NewStringUTF(name);
        jstring value_ = mInfo.env->NewStringUTF(value);

        mInfo.env->CallStaticVoidMethod(mInfo.classID, mInfo.methodID, name_, value_);
        mInfo.env->DeleteLocalRef(name_);
        mInfo.env->DeleteLocalRef(value_);
        mInfo.env->DeleteLocalRef(mInfo.classID);
    }

#endif
}

void Analytics::logEvent(const std::string& name, const std::string& val) {

    std::string eventName = "Game:Info:" + name;
    logEvent(eventName.c_str(), val.c_str());
}


