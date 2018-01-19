#pragma once

#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include "jni.h"

#ifdef __cplusplus
extern "C" {
#endif

	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_TaplighWrapper_onAdReadyNative(JNIEnv*, jobject, jstring);
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_TaplighWrapper_onLoadErrorNative(JNIEnv*, jobject, jint);
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_TaplighWrapper_onAdResultNative(JNIEnv*, jobject, jint, jstring);
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_TaplighWrapper_onRewardReadyNative(JNIEnv*, jobject, jstring);

#ifdef __cplusplus
}
#endif


class Tapligh
{
public:
	Tapligh();
	~Tapligh();

	static Tapligh& getInstance();

	void loadAd(const char* unitCode);
	void showAd(const char* unitCode);

	typedef std::function<void(const std::string&)> OnAdReadyFuncCallback;
	typedef std::function<void(int, const std::string&)> OnAdResultCallback;

	void setOnAdReadyFuncCallback(const OnAdReadyFuncCallback& callback);
	void callOnAdReadyFuncCallback(const std::string& s);

	void setOnAdResultFuncCallback(const OnAdResultCallback& callback);
	void callOnAdResultFuncCallback(int result, const std::string& token);

private:

	OnAdReadyFuncCallback _onAdReadyFuncCallback;
	OnAdResultCallback _onAdResultFuncCallback;
};

