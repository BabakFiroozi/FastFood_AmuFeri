#pragma once

#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include "jni.h"

#ifdef __cplusplus
extern "C" {
#endif

	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_BazinamaAcLeWrapper_achievement(JNIEnv*, jobject, jstring);

#ifdef __cplusplus
}
#endif

class BazinamaAcLe
{
public:
	BazinamaAcLe();
	~BazinamaAcLe();

	static BazinamaAcLe& getInstance();

	void getUserAchievements(const char* pkgName);
	void unlockUserAchievements(const char* pkgName, const char* achievementId);
	void incrementAchievement(const char* pkgName, const char* achievementId, int step);
	void submitScore(const char* pkgName, const char* scoreId, int scoreValue);
	void openLeaderBoard(const char* pkgName, const char* scoreId, const char* timeScope);

private:

};

