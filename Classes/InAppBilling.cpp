#include "InAppBilling.h"
#include "GameChoice.h"


USING_NS_CC;


JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_InAppBillingWrapper_onIabPurchaseFinishedSuccess(JNIEnv* env, jobject obj, jstring sku)
{
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_InAppBillingWrapper_onIabPurchaseFinishedFailure(JNIEnv* env, jobject obj, jstring sku)
{
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_InAppBillingWrapper_onConsumeFinishedSuccess(JNIEnv* env, jobject obj, jstring sku)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	std::string str = JniHelper::jstring2string(sku);

	int amount = 0;

	if (str == "CoinPack_1")
		amount = GameChoice::getInstance().getShopItem(0);
	if (str == "CoinPack_2")
		amount = GameChoice::getInstance().getShopItem(1);
	if (str == "CoinPack_3")
		amount = GameChoice::getInstance().getShopItem(2);
	if (str == "CoinPack_4")
		amount = GameChoice::getInstance().getShopItem(3);
	if (str == "CoinPack_5")
		amount = GameChoice::getInstance().getShopItem(4);
	if (str == "CoinPack_6")
		amount = GameChoice::getInstance().getShopItem(5);
	
	if(amount != 0)
		InAppBilling::getInstance().callPurchaseResultCallback(amount);

#endif
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_InAppBillingWrapper_onConsumeFinishedFailure(JNIEnv* env, jobject obj, jstring sku)
{
}


InAppBilling::InAppBilling()
{
}

InAppBilling::~InAppBilling()
{
}

InAppBilling& InAppBilling::getInstance()
{
	static InAppBilling instance;
	return instance;
}

void InAppBilling::launchPurchaseFlow(const char* sku, int req)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JniMethodInfo mInfo;
	if (JniHelper::getStaticMethodInfo(mInfo, "org/cocos2dx/cpp/InAppBillingWrapper", "launchPurchaseFlow", "(Ljava/lang/String;I)V"))
	{
		jstring sku_ = mInfo.env->NewStringUTF(sku);
		jint req_ = req;
	
		mInfo.env->CallStaticVoidMethod(mInfo.classID, mInfo.methodID, sku_, req_);
		mInfo.env->DeleteLocalRef(sku_);
		mInfo.env->DeleteLocalRef(mInfo.classID);
	}

#endif
}

void InAppBilling::setPurchaseResultCallback(const PurchanseResultFuncCallback& callback)
{
	_purchaseResultCallback = callback;
}

void InAppBilling::callPurchaseResultCallback(int amount) const
{
	if (_purchaseResultCallback != nullptr)
		_purchaseResultCallback(amount);
}
