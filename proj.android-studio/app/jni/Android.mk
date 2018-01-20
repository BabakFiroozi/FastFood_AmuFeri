LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/Food.cpp \
                   $(LOCAL_PATH)/../../../Classes/FoodFactory.cpp \
                   $(LOCAL_PATH)/../../../Classes/SplashScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/GameChoice.cpp \
                   $(LOCAL_PATH)/../../../Classes/GameplayScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/ShopScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/MenuScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/PlayerPrefs.cpp \
                   $(LOCAL_PATH)/../../../Classes/Inventories.cpp \
                   $(LOCAL_PATH)/../../../Classes/StandbyLayout.cpp \
                   $(LOCAL_PATH)/../../../Classes/InAppBilling.cpp \
                   $(LOCAL_PATH)/../../../Classes/BazinamaAcLe.cpp \
                   $(LOCAL_PATH)/../../../Classes/Tapligh.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
