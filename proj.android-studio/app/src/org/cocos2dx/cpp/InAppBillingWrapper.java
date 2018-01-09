/**
package org.cocos2dx.cpp;

import com.android.trivialdrive.util.IabHelper;
import com.android.trivialdrive.util.IabResult;
import com.android.trivialdrive.util.Purchase;

import android.app.AlertDialog;
import android.util.Log;

public class InAppBillingWrapper {
	
	public static native void onIabPurchaseFinishedSuccess(String sku);
	public static native void onIabPurchaseFinishedFailure(String sku);
	public static native void onConsumeFinishedSuccess(String sku);
	public static native void onConsumeFinishedFailure(String sku);
	
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
	
	public static void startSetup()
	{
		String base64EncodedPublicKey = "CONSTRUCT_YOUR_KEY_AND_PLACE_IT_HERE";

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
		
		mHelper.launchPurchaseFlow(_activity, sku, req,
                mPurchaseFinishedListener, payload);
	}
	
	
	// Callback for when a purchase is finished
    static IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
            Log.d(TAG, "Purchase finished: " + result + ", purchase: " + purchase);

            // if we were disposed of in the meantime, quit.
            if (mHelper == null) return;

            if (result.isFailure()) {
                complain("Error purchasing: " + result); 
                onIabPurchaseFinishedFailure(purchase.getSku());
                return;
            }          
           
        	onIabPurchaseFinishedSuccess(purchase.getSku());    
        	onConsumeFinishedSuccess(purchase.getSku());

            Log.d(TAG, "Purchase successful.");

            if (purchase.getSku().equals(purchase.getSku())) {
                // bought 1/4 tank of gas. So consume it.
                Log.d(TAG, "Purchase is gas. Starting gas consumption.");
                //mHelper.consumeAsync(purchase, mConsumeFinishedListener);
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
            	onConsumeFinishedFailure(purchase.getSku());
                complain("Error while consuming: " + result);
            }
            
            Log.d(TAG, "End consumption flow.");
        }
    }; 

}
*/