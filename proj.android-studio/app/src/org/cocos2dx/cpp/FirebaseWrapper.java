package org.cocos2dx.cpp;

import android.app.Activity;
import android.os.Bundle;

import com.google.firebase.analytics.FirebaseAnalytics;

import java.util.regex.PatternSyntaxException;

/**
 * Created by Babak on 1/21/2018.
 */

public class FirebaseWrapper {

    private static FirebaseAnalytics mFirebaseAnalytics;

    public static void create(Activity act)
    {
        mFirebaseAnalytics = FirebaseAnalytics.getInstance(act);
    }

    public static void logEvent(String keys, String values)
    {
        String[] keysArr = null;
        String[] valuesArr = null;
        try{
            keysArr = keys.split(", ");
            valuesArr = values.split(", ");

            Bundle params = new Bundle();

            for(int c = 0; c < keysArr.length; ++c){
                String key = keysArr[c];
                String val = valuesArr[c];
                params.putString(key, val);
            }

            mFirebaseAnalytics.logEvent("share_image", params);
        }
        catch(PatternSyntaxException e){
            e.printStackTrace();
        }

    }
}
