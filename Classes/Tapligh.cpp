#include "Tapligh.h"

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_TaplighWrapper_onAdReadyNative(JNIEnv* env, jobject obj, jstring token)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string token_ = JniHelper::jstring2string(token);
	Tapligh::getInstance().callOnAdReadyFuncCallback(token_);
#endif
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_TaplighWrapper_onLoadErrorNative(JNIEnv* env, jobject obj, jint status)
{
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_TaplighWrapper_onAdResultNative(JNIEnv* env, jobject obj, jint result, jstring token)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	int result_ = result;
	std::string token_ = JniHelper::jstring2string(token);
	Tapligh::getInstance().callOnAdResultFuncCallback(result_, token_);
#endif
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_TaplighWrapper_onRewardReadyNative(JNIEnv* env, jobject obj, jstring reward)
{
}

Tapligh::Tapligh()
{
}

Tapligh::~Tapligh()
{
}

Tapligh& Tapligh::getInstance()
{
	static Tapligh instance;
	return instance;
}

void Tapligh::loadAd(const char* unitCode)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JniMethodInfo mInfo;
	if (JniHelper::getStaticMethodInfo(mInfo, "org/cocos2dx/cpp/TaplighWrapper", "loadAd", "(Ljava/lang/String;)V"))
	{
		jstring unitCode_ = mInfo.env->NewStringUTF(unitCode);

		mInfo.env->CallStaticVoidMethod(mInfo.classID, mInfo.methodID, unitCode_);
		mInfo.env->DeleteLocalRef(unitCode_);
		mInfo.env->DeleteLocalRef(mInfo.classID);
	}

#endif
}

void Tapligh::showAd(const char* unitCode)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JniMethodInfo mInfo;
	if (JniHelper::getStaticMethodInfo(mInfo, "org/cocos2dx/cpp/TaplighWrapper", "showAd", "(Ljava/lang/String;)V"))
	{
		jstring unitCode_ = mInfo.env->NewStringUTF(unitCode);

		mInfo.env->CallStaticVoidMethod(mInfo.classID, mInfo.methodID, unitCode_);
		mInfo.env->DeleteLocalRef(unitCode_);
		mInfo.env->DeleteLocalRef(mInfo.classID);
	}

#endif
}

void Tapligh::setOnAdReadyFuncCallback(const OnAdReadyFuncCallback& callback)
{
	_onAdReadyFuncCallback = callback;
}

void Tapligh::callOnAdReadyFuncCallback(const std::string& s)
{
	if (_onAdReadyFuncCallback != nullptr)
		_onAdReadyFuncCallback(s);
}

void Tapligh::setOnAdResultFuncCallback(const OnAdResultCallback& callback)
{
	_onAdResultFuncCallback = callback;
}

void Tapligh::callOnAdResultFuncCallback(int result, const std::string& token)
{
	if (_onAdResultFuncCallback != nullptr)
		_onAdResultFuncCallback(result, token);
}
