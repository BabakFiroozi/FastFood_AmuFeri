#ifndef __IN_APP_BILLING_H__
#define __IN_APP_BILLING_H__

#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include "jni.h"


#ifdef __cplusplus
extern "C" {
#endif

	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_InAppBillingWrapper_onIabPurchaseFinishedSuccess(JNIEnv*, jobject, jstring);

	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_InAppBillingWrapper_onConsumeFinishedSuccess(JNIEnv*, jobject, jstring);

#ifdef __cplusplus
}
#endif


class InAppBilling
{
public:
	InAppBilling();
	~InAppBilling();

	typedef std::function<void(int)> PurchanseResultFuncCallback;

	void launchPurchaseFlow(const char* sku, int req);

	static InAppBilling& getInstance();

	void setPurchaseResultCallback(const PurchanseResultFuncCallback& callback);
	void callPurchaseResultCallback(int amount) const;

private:

	PurchanseResultFuncCallback _purchaseResultCallback;
};


#endif //__IN_APP_BILLING_H__
