//
// Created by Babak on 1/21/2018.
//

#include "Firebase.h"

USING_NS_CC;

Firebase::Firebase()
{
}

Firebase::~Firebase()
{
}

Firebase& Firebase::getInstance()
{
    static Firebase instance;
    return instance;
}

void Firebase::logEvent(const char *keys, const char *values)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    JniMethodInfo mInfo;
    if (JniHelper::getStaticMethodInfo(mInfo, "org/cocos2dx/cpp/FirebaseWrapper", "logEvent", "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring keys_ = mInfo.env->NewStringUTF(keys);
        jstring values_ = mInfo.env->NewStringUTF(values);

        mInfo.env->CallStaticVoidMethod(mInfo.classID, mInfo.methodID, keys_, values_);
        mInfo.env->DeleteLocalRef(keys_);
        mInfo.env->DeleteLocalRef(values_);
        mInfo.env->DeleteLocalRef(mInfo.classID);
    }

#endif
}


