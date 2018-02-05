//
// Created by Babak on 1/21/2018.
//

#ifndef PROJ_ANDROID_STUDIO_FIREBASE_H
#define PROJ_ANDROID_STUDIO_FIREBASE_H


#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include "jni.h"

class Firebase
{
public:
    Firebase();
    ~Firebase();

    static Firebase& getInstance();

    void logEvent(const char* keys, const char* values);

};


#endif //PROJ_ANDROID_STUDIO_FIREBASE_H
