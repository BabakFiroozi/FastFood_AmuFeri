#include "BazinamaAcLe.h"

USING_NS_CC;

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_BazinamaAcLeWrapper_achievement(JNIEnv* env, jobject obj, jstring data)
{
}

BazinamaAcLe::BazinamaAcLe()
{
}

BazinamaAcLe::~BazinamaAcLe()
{
}

BazinamaAcLe& BazinamaAcLe::getInstance()
{
	static BazinamaAcLe instance;
	return instance;
}


void BazinamaAcLe::getUserAchievements(const char* pkgName)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JniMethodInfo mInfo;
	if (JniHelper::getStaticMethodInfo(mInfo, "org/cocos2dx/cpp/InAppBillingWrapper", "getUserAchievements", "(Ljava/lang/String;)V"))
	{
		jstring pkgName_ = mInfo.env->NewStringUTF(pkgName);

		mInfo.env->CallStaticVoidMethod(mInfo.classID, mInfo.methodID, pkgName_);
		mInfo.env->DeleteLocalRef(pkgName_);
		mInfo.env->DeleteLocalRef(mInfo.classID);
	}

#endif
}

void BazinamaAcLe::unlockUserAchievements(const char* pkgName, const char* achievementId)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JniMethodInfo mInfo;
	if (JniHelper::getStaticMethodInfo(mInfo, "org/cocos2dx/cpp/InAppBillingWrapper", "unlockUserAchievement", "(Ljava/lang/String;Ljava/lang/String;)V"))
	{
		jstring pkgName_ = mInfo.env->NewStringUTF(pkgName);
		jstring achievementId_ = mInfo.env->NewStringUTF(achievementId);

		mInfo.env->CallStaticVoidMethod(mInfo.classID, mInfo.methodID, pkgName_, achievementId_);
		mInfo.env->DeleteLocalRef(pkgName_);
		mInfo.env->DeleteLocalRef(achievementId_);
		mInfo.env->DeleteLocalRef(mInfo.classID);
	}

#endif
}

void BazinamaAcLe::incrementAchievement(const char* pkgName, const char* achievementId, int step)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JniMethodInfo mInfo;
	if (JniHelper::getStaticMethodInfo(mInfo, "org/cocos2dx/cpp/InAppBillingWrapper", "incrementAchievement", "(Ljava/lang/String;Ljava/lang/String;I)V"))
	{
		jstring pkgName_ = mInfo.env->NewStringUTF(pkgName);
		jstring achievementId_ = mInfo.env->NewStringUTF(achievementId);
		jint step_ = step;

		mInfo.env->CallStaticVoidMethod(mInfo.classID, mInfo.methodID, pkgName_, achievementId_, step_);
		mInfo.env->DeleteLocalRef(pkgName_);
		mInfo.env->DeleteLocalRef(achievementId_);
		mInfo.env->DeleteLocalRef(mInfo.classID);
	}

#endif
}

void BazinamaAcLe::submitScore(const char* pkgName, const char* scoreId, int scoreValue)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JniMethodInfo mInfo;
	if (JniHelper::getStaticMethodInfo(mInfo, "org/cocos2dx/cpp/InAppBillingWrapper", "submitScore", "(Ljava/lang/String;Ljava/lang/String;I)V"))
	{
		jstring pkgName_ = mInfo.env->NewStringUTF(pkgName);
		jstring scoreId_ = mInfo.env->NewStringUTF(scoreId);
		jint scoreValue_ = (jint)scoreValue;

		mInfo.env->CallStaticVoidMethod(mInfo.classID, mInfo.methodID, pkgName_, scoreId_, scoreValue_);
		mInfo.env->DeleteLocalRef(pkgName_);
		mInfo.env->DeleteLocalRef(scoreId_);
		mInfo.env->DeleteLocalRef(mInfo.classID);
	}

#endif
}

void BazinamaAcLe::openLeaderBoard(const char* pkgName, const char* scoreId, const char* timeScope)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JniMethodInfo mInfo;
	if (JniHelper::getStaticMethodInfo(mInfo, "org/cocos2dx/cpp/InAppBillingWrapper", "openLeaderBoard", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
	{
		jstring pkgName_ = mInfo.env->NewStringUTF(pkgName);
		jstring scoreId_ = mInfo.env->NewStringUTF(scoreId);
		jstring timeScope_ = mInfo.env->NewStringUTF(timeScope);

		mInfo.env->CallStaticVoidMethod(mInfo.classID, mInfo.methodID, pkgName_, scoreId_, timeScope_);
		mInfo.env->DeleteLocalRef(pkgName_);
		mInfo.env->DeleteLocalRef(scoreId_);
		mInfo.env->DeleteLocalRef(timeScope_);
		mInfo.env->DeleteLocalRef(mInfo.classID);
	}

#endif
}
