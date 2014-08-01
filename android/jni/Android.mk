LOCAL_PATH := $(call my-dir)

# cocos2dx-store
include $(CLEAR_VARS)

COCOS2D_JAVASCRIPT = $(filter %-DCOCOS2D_JAVASCRIPT,$(APP_CPPFLAGS))

#$(call __ndk_warning,COCOS2D_JAVASCRIPT: "$(COCOS2D_JAVASCRIPT)")
#$(call __ndk_warning,APP_CPPFLAGS: "$(APP_CPPFLAGS)")

LOCAL_MODULE := cocos2dx_store_static
LOCAL_MODULE_FILENAME := libcocos2dxstore
LOCAL_SRC_FILES := ../../Soomla/CCStoreConsts.cpp \
	../../Soomla/CCSoomlaStore.cpp \
	../../Soomla/CCStoreEventDispatcher.cpp \
	../../Soomla/CCStoreInventory.cpp \
	../../Soomla/CCStoreService.cpp \
	../../Soomla/CCStoreUtils.cpp \
	../../Soomla/PurchaseTypes/CCPurchaseType.cpp \
	../../Soomla/PurchaseTypes/CCPurchaseWithMarket.cpp \
	../../Soomla/PurchaseTypes/CCPurchaseWithVirtualItem.cpp \
	../../Soomla/data/CCStoreInfo.cpp \
	../../Soomla/domain/CCMarketItem.cpp \
	../../Soomla/domain/CCNonConsumableItem.cpp \
	../../Soomla/domain/CCPurchasableVirtualItem.cpp \
	../../Soomla/domain/CCVirtualCategory.cpp \
	../../Soomla/domain/CCVirtualItem.cpp \
	../../Soomla/domain/virtualCurrencies/CCVirtualCurrency.cpp \
	../../Soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.cpp \
	../../Soomla/domain/virtualGoods/CCEquippableVG.cpp \
	../../Soomla/domain/virtualGoods/CCLifetimeVG.cpp \
	../../Soomla/domain/virtualGoods/CCSingleUsePackVG.cpp \
	../../Soomla/domain/virtualGoods/CCSingleUseVG.cpp \
	../../Soomla/domain/virtualGoods/CCUpgradeVG.cpp \
	../../Soomla/domain/virtualGoods/CCVirtualGood.cpp \
	../../Soomla/rewards/CCVirtualItemReward.cpp


ifneq '$(COCOS2D_JAVASCRIPT)' ''
LOCAL_SRC_FILES +=  \
        ../../Soomla/jsb/jsb_soomla.cpp \
        ../../Soomla/jsb/JSBinding.cpp
endif

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Soomla
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Soomla/data
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Soomla/domain
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Soomla/domain/virtualCurrencies
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Soomla/domain/virtualGoods
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Soomla/jsb
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Soomla/PurchaseTypes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Soomla/rewards
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../lib \
        $(LOCAL_PATH)/../../../bindings/manual

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_soomla_common_static

ifneq '$(COCOS2D_JAVASCRIPT)' ''
LOCAL_WHOLE_STATIC_LIBRARIES += spidermonkey_static
LOCAL_WHOLE_STATIC_LIBRARIES += scriptingcore-spidermonkey
endif

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../..
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../Soomla
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../Soomla/data
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../Soomla/domain
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../Soomla/domain/virtualCurrencies
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../Soomla/domain/virtualGoods
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../Soomla/jsb
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../Soomla/PurchaseTypes
LOCAL_EXPORT_C_INCLUDES += $(LOCAL_PATH)/../../Soomla/rewards

include $(BUILD_STATIC_LIBRARY)

$(call import-module,extensions/soomla-cocos2dx-core/android/jni)

ifneq '$(COCOS2D_JAVASCRIPT)' ''
$(call import-module,external/spidermonkey/prebuilt/android)
$(call import-module,bindings)
endif
