//
// Created by Babak on 1/21/2018.
//

#ifndef PROJ_ANDROID_STUDIO_FIREBASE_H
#define PROJ_ANDROID_STUDIO_FIREBASE_H


#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include "jni.h"

class Analytics
{
public:
    Analytics();
    ~Analytics();

    static Analytics& getInstance();

    void logEvent(const char* name, const char* value);
    void logEvent(const std::string& name, const std::string& val = "");

};


#endif //PROJ_ANDROID_STUDIO_FIREBASE_H
