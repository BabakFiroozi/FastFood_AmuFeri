package org.cocos2dx.cpp;

import android.app.Activity;

import com.tapligh.sdk.ADView.ADUtils.ADResultListener;
import com.tapligh.sdk.ADView.ADUtils.AdLoadListener;
import com.tapligh.sdk.ADView.Tapligh;

/**
 * Created by Babak on 1/18/2018.
 */

public class TaplighWrapper {

    public static native void onAdReadyNative(String s);
    public static native void onLoadErrorNative(int status);
    public static native void onAdResultNative(int res, String s);
    public static native void onRewardReadyNative(String s);

    static Tapligh _tapligh;

    public static void create(Activity act, String token)
    {
        _tapligh = Tapligh.newInstance(act);
        _tapligh.setToken(token);
    }

    public static void loadAd(String unitCode){

        _tapligh.loadAd(unitCode, new AdLoadListener() {
            @Override
            public void onAdReady(String token) {
                onAdReadyNative(token);
            }

            @Override
            public void onLoadError(LoadErrorStatus loadErrorStatus) {
                onLoadErrorNative(loadErrorStatus.ordinal());
            }
        });
    }

    public static void showAd(String unitCode){

        _tapligh.showAd(unitCode, new ADResultListener() {
            @Override
            public void onAdResult(ADResult adResult, String token) {
                onAdResultNative(adResult.ordinal(), token);
            }

            @Override
            public void onRewardReady(String reward) {
                onRewardReadyNative(reward);
            }
        });
    }

}
