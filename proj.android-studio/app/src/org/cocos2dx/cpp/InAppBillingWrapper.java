
package org.cocos2dx.cpp;

import com.example.android.trivialdrivesample.util.IabHelper;
import com.example.android.trivialdrivesample.util.IabResult;
import com.example.android.trivialdrivesample.util.Purchase;

import android.app.AlertDialog;
import android.os.RemoteException;
import android.util.Log;

public class InAppBillingWrapper {
	
	public static native void onIabPurchaseFinishedSuccess(String sku);
	public static native void onConsumeFinishedSuccess(String sku);

	// Debug tag, for logging
    static final String TAG = "FastFood";
	
	static AppActivity _activity;
	
	 // The helper object
    static IabHelper mHelper;
		
    static void complain(String message) {
        Log.e(TAG, "**** TrivialDrive Error: " + message);        
    }

    static void alert(String message) {
        AlertDialog.Builder bld = new AlertDialog.Builder(_activity);
        bld.setMessage(message);
        bld.setNeutralButton("OK", null);
        Log.d(TAG, "Showing alert dialog: " + message);
        bld.create().show();
    }
	
	public static void setActivity(AppActivity act)
	{
		_activity = act;	
	}

	public static IabHelper getmHelper()
    {
        return mHelper;
    }
	
	public static void startSetup()
	{

        IabHelper.StoreName = IabHelper.StoreNames.Bazinama;

        if(IabHelper.StoreName == IabHelper.StoreNames.Bazik)
            return;

        //cafebazar
//        String base64EncodedPublicKey = "MIHNMA0GCSqGSIb3DQEBAQUAA4G7ADCBtwKBrwDU8L4rPgshDKNDmivKyA0hcTlKQ+UYvHAK4TQljTofR+KgIVa7tml7Zx1d1DANjFBYSvrZGgJuCCb2j5jER/LLd9u9GcOm50/DzJZxvwhKkYSi/syDGL58WjPbpkcaWx5YEajHrMDT8wJs9DyXQYawtBvTKgTmzkaH4u8m0fRuCdfL0swVPx5XExWTmF8XpNnOYY+6P5f43xpdA4xVeSjCTT631VDZy1n4D9mytscCAwEAAQ==";

        //bazinama
        String base64EncodedPublicKey = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC41J6c/nPGD1woc4Uy3SQsZd4DmgobhpUeH55Jq6UqOytja/rstV0AMBv3ci8T8jYQLXI8ncdVdgPHmuuy03txHu4fQdk6oNRSE2sss6FHc1p1dI/OH/dad+ZoWf9JT3RWLQDqjhmg8isQ0Gc3KWhnkUlXpqAznLRKpR3iG+vqmQIDAQAB";

        //myket
//        String base64EncodedPublicKey = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDFxj4cGIm/MSBieoDXZXAv9nWG9KwOzIHVVmSbtd++yVTfutnPBiiTBTtFlrwAT5VJ5MsLm0vYXD8p2TBrwSTUP+bC3/4MYLPgvfLyHiZ2Y7+DLs/kK4JQfHRFTcPFLUCL5Zh8hsJncI0Swgc6t/44Kj0bGONFK4BtqR2K0DEtaQIDAQAB";
		
		//iranapps
//        String base64EncodedPublicKey = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDzebhIg7sO1H6i76MoS7/ThARa1r0F52y3AZUb/u/IXAvBG06G/wSMpd3pcZzn+qCu+Ikv0c+rtp5q3HC6dLXd3iRjeWUxCN/+OF214lFQ61W6hp9J6IljPdIoF8i4l5phrsNv0cHz0yWxhhzwGypzso2Wqfe+ZdHvA4xyb7iF2QIDAQAB";


        mHelper = new IabHelper(_activity, base64EncodedPublicKey);

        // enable debug logging (for a production application, you should set this to false).
        mHelper.enableDebugLogging(true);

        // Start setup. This is asynchronous and the specified listener
        // will be called once setup completes.
        Log.d(TAG, "Starting setup.");
        mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
            public void onIabSetupFinished(IabResult result) {
                Log.d(TAG, "Setup finished.");

                if (!result.isSuccess()) {
                    // Oh noes, there was a problem.
                    complain("Problem setting up in-app billing: " + result);
                    return;
                }

                // Have we been disposed of in the meantime? If so, quit.
                if (mHelper == null) return;


                // IAB is fully set up. Now, let's get an inventory of stuff we own.
                Log.d(TAG, "Setup successful. Querying inventory.");
                //mHelper.queryInventoryAsync(mGotInventoryListener);
            }
        });
	}
	
	public static void launchPurchaseFlow(String sku, int req)
	{
		String payload = "";

//        mHelper.launchPurchaseFlow(_activity, sku, req,
//                mPurchaseFinishedListener, payload);

		try{
			mHelper.launchPurchaseFlow(_activity, sku, req,
					mPurchaseFinishedListener, payload);
		}
		catch(IabHelper.IabAsyncInProgressException e){
			e.printStackTrace();
		}
	}
	
	
	// Callback for when a purchase is finished
    static IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
            Log.d(TAG, "Purchase finished: " + result + ", purchase: " + purchase);

            // if we were disposed of in the meantime, quit.
            if (mHelper == null) return;

            if (result.isFailure()) {
                complain("Error purchasing: " + result);
                return;
            }          
           
        	onIabPurchaseFinishedSuccess(purchase.getSku());

            Log.d(TAG, "Purchase successful.");

//            mHelper.consumeAsync(purchase, mConsumeFinishedListener);

            try{
                mHelper.consumeAsync(purchase, mConsumeFinishedListener);
            }
            catch(IabHelper.IabAsyncInProgressException e){
                e.printStackTrace();
            }
        }
    };
    
    // Called when consumption is complete
    static IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {
        public void onConsumeFinished(Purchase purchase, IabResult result) {
            Log.d(TAG, "Consumption finished. Purchase: " + purchase + ", result: " + result);

            // if we were disposed of in the meantime, quit.
            if (mHelper == null) return;

            // We know this is the "gas" sku because it's the only one we consume,
            // so we don't check which sku was consumed. If you have more than one
            // sku, you probably should check...
            if (result.isSuccess()) {
                // successfully consumed, so we apply the effects of the item in our
                // game world's logic, which in our case means filling the gas tank a bit
                Log.d(TAG, "Consumption successful. Provisioning.");
                onConsumeFinishedSuccess(purchase.getSku());
            }
            else {
                complain("Error while consuming: " + result);
            }
            
            Log.d(TAG, "End consumption flow.");
        }
    };


    //Bazinama...........
    public static String getUserAchievements(String packageName)
    {
        if(mHelper.StoreName != IabHelper.StoreNames.Bazinama)
            return "";

        String userAchievements = "";
		try {
			userAchievements = mHelper.getUserAchievements(packageName);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        //message to cpp
        return userAchievements;
    }

    public static String unlockUserAchievement(String packageName, String achievementId)
    {
        if(mHelper.StoreName != IabHelper.StoreNames.Bazinama)
            return "";

        String userAchievement = "";
        userAchievement = mHelper.unlockUserAchievement(packageName, achievementId);
        //message to cpp
        return userAchievement;
    }

    public static void incrementAchievement(String packageName, String achievementId, int step)
    {
        if(mHelper.StoreName != IabHelper.StoreNames.Bazinama)
            return;

        mHelper.incrementAchievement(packageName, achievementId, step);

        //message to cpp
    }

    public static void submitScore(String packageName, String scoreId, int scoreValue)
    {
        if(mHelper.StoreName != IabHelper.StoreNames.Bazinama)
            return;

        try{
            mHelper.submitScore(packageName, scoreId, scoreValue);
        }
        catch(RemoteException e){
            e.printStackTrace();
        }

        //message to cpp
    }

    public static void openLeaderBoard(String packageName, String scoreId, String timeScope)
    {
        if(mHelper.StoreName != IabHelper.StoreNames.Bazinama)
            return;

        try{
            mHelper.openLeaderBoard(packageName, scoreId, timeScope);
        }
        catch(RemoteException e){
            e.printStackTrace();
        }

        //message to cpp
    }
    //Bazinama...........
}
