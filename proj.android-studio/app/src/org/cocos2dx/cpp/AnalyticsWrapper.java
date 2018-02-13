package org.cocos2dx.cpp;

import android.app.Activity;
import android.os.Bundle;

import com.gameanalytics.sdk.*;

import java.util.regex.PatternSyntaxException;

/**
 * Created by Babak on 1/21/2018.
 */

public class AnalyticsWrapper {

    public static void create(Activity act)
    {
        // Enable log
        GameAnalytics.setEnabledInfoLog(true);
        GameAnalytics.setEnabledVerboseLog(true);

        GameAnalytics.configureBuild("1.0");

        GameAnalytics.initializeWithGameKey(act, "b45acfaedd73d4892d9cbe2c182163f9", "df9937bdabe10d0a03850a4ca67cf7653364b4a6");

    }

    public static void logEvent(String name, String value)
    {
        //double val = Double.parseDouble(value);
        //GameAnalytics.addDesignEventWithEventId(name, val);
        GameAnalytics.addDesignEventWithEventId(name);
    }
}
