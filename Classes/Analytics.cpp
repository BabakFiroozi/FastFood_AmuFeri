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

void Analytics::logEvent(const char *name, const char *keys, const char *values)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    JniMethodInfo mInfo;
    if (JniHelper::getStaticMethodInfo(mInfo, "org/cocos2dx/cpp/FirebaseWrapper", "logEvent", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring name_ = mInfo.env->NewStringUTF(name);
        jstring keys_ = mInfo.env->NewStringUTF(keys);
        jstring values_ = mInfo.env->NewStringUTF(values);

        mInfo.env->CallStaticVoidMethod(mInfo.classID, mInfo.methodID, name_, keys_, values_);
        mInfo.env->DeleteLocalRef(name_);
        mInfo.env->DeleteLocalRef(keys_);
        mInfo.env->DeleteLocalRef(values_);
        mInfo.env->DeleteLocalRef(mInfo.classID);
    }

#endif
}

void Analytics::logEvent(const std::string& name) {

    logEvent(name.c_str(), "", "");
}


