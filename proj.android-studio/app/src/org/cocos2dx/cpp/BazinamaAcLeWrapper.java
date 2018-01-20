package org.cocos2dx.cpp;

import com.example.android.trivialdrivesample.util.IabHelper;
//import android.os.RemoteException;
import android.util.Log;


public final class BazinamaAcLeWrapper {

	static final String TAG = "TrivialDrive";
	
	static IabHelper mHelper;

	static void complain(String message) {
		Log.e(TAG, "**** TrivialDrive Error: " + message);
	}

	public static void setIabHelper(IabHelper h){
		mHelper = h;
	}

	public static String getUserAchievements(String packageName)
	{
		String userAchievements = "";
//		try {
//			userAchievements = mHelper.getUserAchievements(packageName);
//		} catch (Exception e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}
		//message to cpp
		return userAchievements;
	}
	
	public static String unlockUserAchievement(String packageName, String achievementId)
	{
		String userAchievement = "";
//		userAchievement = mHelper.unlockUserAchievement(packageName, achievementId);
		//message to cpp
		return userAchievement;
	}
	
	public static void incrementAchievement(String packageName, String achievementId, int step)
	{
//		mHelper.incrementAchievement(packageName, achievementId, step);

		//message to cpp
	}

	public static void submitScore(String packageName, String scoreId, int scoreValue)
	{
//		try{
//			mHelper.submitScore(packageName, scoreId, scoreValue);
//		}
//		catch(RemoteException e){
//			e.printStackTrace();
//		}

		//message to cpp
	}

	public static void openLeaderBoard(String packageName, String scoreId, String timeScope)
	{
//		try{
//			mHelper.openLeaderBoard(packageName, scoreId, timeScope);
//		}
//		catch(RemoteException e){
//			e.printStackTrace();
//		}

		//message to cpp
	}
}
