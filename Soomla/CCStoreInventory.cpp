/*
 Copyright (C) 2012-2014 Soomla Inc.
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

// Created by Fedor Shubin on 5/21/13.

#include "CCStoreInventory.h"
#include "CCStoreInfo.h"
#include "CCStoreEventDispatcher.h"
#include "CCVirtualCurrencyPack.h"
#include "CCStoreUtils.h"
#include "CCNdkBridge.h"

namespace soomla {
#define TAG "SOOMLA StoreInventory"

    USING_NS_CC;

    static CCStoreInventory *s_SharedStoreInventory = NULL;

    CCStoreInventory *CCStoreInventory::sharedStoreInventory() {
        if (!s_SharedStoreInventory) {
          s_SharedStoreInventory = new CCStoreInventory();
          s_SharedStoreInventory->init();
        }
        
        return s_SharedStoreInventory;
    }

    CCStoreInventory::CCStoreInventory() {

    }

    CCStoreInventory::~CCStoreInventory() {

    }

    bool CCStoreInventory::init() {
        return true;
    }

    void CCStoreInventory::buyItem(char const *itemId, CCError **error) {
        buyItem(itemId, nullptr, error);
    }

    void CCStoreInventory::buyItem(char const *itemId, const char *payload, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling buyItem with: %s", itemId)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::buyItem"), "method");
        params->setObject(__String::create(itemId), "itemId");
        params->setObject(__String::create(payload != nullptr ? payload : ""), "payload");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//no native store implementation for win32 or linux
		//-> we fake success
		CCError* err = NULL;
		CCVirtualItem* item = CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId, &err);
		if (!err)
		{
			CCPurchasableVirtualItem * pvi = dynamic_cast<CCPurchasableVirtualItem *> (item);
			if (pvi)
			{
				std::string pviID(pvi->getItemId()->getCString());
				CCStoreEventDispatcher::getInstance()->onItemPurchaseStarted(pvi);

				//we are in fake store
				CCStoreEventDispatcher::getInstance()->onBillingNotSupported();

				//handling android static tests
				if (pviID == "android.test.item_unavailable")
				{
					//nothing to do
				}
				else if (pviID == "android.test.canceled")
				{
					//nothing to do
				}
				else if (pviID == "android.test.refunded")
				{
					CCError* err = NULL;
					takeItem(itemId,1,&err);
					if (!err)
					{
						//done!!
					}
				}
				else //if (pviID == "android.test.purchased") and all other cases
				{
					CCError* err = NULL;
					giveItem(itemId, 1, &err);
					if (!err)
					{
						//done!!
						CCStoreEventDispatcher::getInstance()->onItemPurchased(pvi);
					}
				}
			}

			CCVirtualCurrencyPack * vcp = dynamic_cast<CCVirtualCurrencyPack *> (item);
			if (vcp)
			{
				if (err)
				{
					//TODO : handle error
				}
			}
			//TODO : virtual item
		}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        CCNdkBridge::callNative (params, error);
#endif
    }

    int CCStoreInventory::getItemBalance(char const *itemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling getItemBalance with: %s", itemId)->getCString());
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::getItemBalance"), "method");
        params->setObject(__String::create(itemId), "itemId");

		__Dictionary *retParams = NULL;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//no store implementation for win32
		//TODO : we fake success
		auto itemstored = m_inventory.find(itemId);
		if (itemstored != m_inventory.end())
		{
			retParams = __Dictionary::create();
			retParams->setObject(CCInteger::create(itemstored->second), "return");
		}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		retParams = (__Dictionary *)CCNdkBridge::callNative(params, error);
#endif
        if (retParams == NULL) {
        	return 0;
        }

		__Integer *retValue = (__Integer *) retParams->objectForKey("return");
		if (retValue) {
			return retValue->getValue();
		} else {
			return 0;
		}
    }

    void CCStoreInventory::giveItem(char const *itemId, int amount, CCError **error) {
        CCStoreUtils::logDebug(TAG, __String::createWithFormat(
                "SOOMLA/COCOS2DX Calling giveItem with itemId: %s and amount %d", itemId, amount)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::giveItem"), "method");
        params->setObject(__String::create(itemId), "itemId");
        params->setObject(__Integer::create(amount), "amount");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//no store implementation for win32
		//we fake success

		CCError* err = NULL;
		auto iteminfo = CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId,&err);
		if (!err)
		{
			CCVirtualCurrencyPack* pack = dynamic_cast<CCVirtualCurrencyPack*> (iteminfo);
			if (pack)
			{
				//replacing parameters if it s a virtual currency pack
				itemId = pack->getCurrencyItemId()->getCString();
				amount = amount * pack->getCurrencyAmount()->getValue();
			}
			//storing any virtual item
			auto itemstored = m_inventory.find(itemId);
			if (itemstored != m_inventory.end())
			{
				m_inventory.at(itemId) += amount;
			}
			else
			{
				m_inventory.insert(std::make_pair(itemId, amount));
			}

			CCVirtualItem* item = CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId, &err);
			if (!err)
			{
				CCVirtualCurrency * vc = dynamic_cast<CCVirtualCurrency *> (item);
				if (vc)
				{
					CCStoreEventDispatcher::getInstance()->onCurrencyBalanceChanged(vc, m_inventory.at(itemId), amount);
				}
			}

		}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CCNdkBridge::callNative (params, error);
#endif
    }

    void CCStoreInventory::takeItem(char const *itemId, int amount, CCError **error) {
        CCStoreUtils::logDebug(TAG, __String::createWithFormat(
                "SOOMLA/COCOS2DX Calling takeItem with itemId: %s and amount %d", itemId, amount)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::takeItem"), "method");
        params->setObject(__String::create(itemId), "itemId");
        params->setObject(__Integer::create(amount), "amount");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//no store implementation for win32
		//we fake success
		CCError* err = NULL;
		auto iteminfo = CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId, &err);
		if (!err)
		{
			CCVirtualCurrencyPack* pack = dynamic_cast<CCVirtualCurrencyPack*> (iteminfo);
			if (pack)
			{
				//replacing parameters if it s a virtual currency pack
				itemId = pack->getCurrencyItemId()->getCString();
				amount = amount * pack->getCurrencyAmount()->getValue();
			}
			//storing any virtual item
			auto itemstored = m_inventory.find(itemId);
			if (itemstored != m_inventory.end() && m_inventory.at(itemId) > amount)
			{
				m_inventory.at(itemId) -= amount;
			}
			else
			{
				//TODO : trigger error
			}
		}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CCNdkBridge::callNative(params, error);
#endif
    }

    void CCStoreInventory::equipVirtualGood(char const *itemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling equipVirtualGood with: %s", itemId)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::equipVirtualGood"), "method");
        params->setObject(__String::create(itemId), "itemId");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//no store implementation for win32
		//TODO : we fake success

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CCNdkBridge::callNative(params, error);
#endif
   }

    void CCStoreInventory::unEquipVirtualGood(char const *itemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling unEquipVirtualGood with: %s", itemId)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::unEquipVirtualGood"), "method");
        params->setObject(__String::create(itemId), "itemId");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//no store implementation for win32
		//TODO : we fake success

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CCNdkBridge::callNative(params, error);
#endif
    }

    bool CCStoreInventory::isVirtualGoodEquipped(char const *itemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling isVirtualGoodEquipped with: %s", itemId)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::isVirtualGoodEquipped"), "method");
        params->setObject(__String::create(itemId), "itemId");
		__Dictionary *retParams = NULL;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//no store implementation for win32
		//TODO : we fake success

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		retParams = (__Dictionary *)CCNdkBridge::callNative(params, error);
#endif

        if (retParams == NULL) {
        	return false;
        }

		__Bool *retValue = (__Bool *) retParams->objectForKey("return");
		if (retValue) {
			return retValue->getValue();
		} else {
			return false;
		}
    }

    int CCStoreInventory::getGoodUpgradeLevel(char const *goodItemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling getGoodUpgradeLevel with: %s", goodItemId)->getCString());
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::getGoodUpgradeLevel"), "method");
        params->setObject(__String::create(goodItemId), "goodItemId");
		__Dictionary *retParams = NULL;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//no store implementation for win32
		//TODO : we fake success

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		retParams = (__Dictionary *)CCNdkBridge::callNative(params, error);
#endif

        if (retParams == NULL) {
        	return 0;
        }

		__Integer *retValue = (__Integer *) retParams->objectForKey("return");
		if (retValue) {
			return retValue->getValue();
		} else {
			return 0;
		}
    }

    std::string CCStoreInventory::getGoodCurrentUpgrade(char const *goodItemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling getGoodCurrentUpgrade with: %s", goodItemId)->getCString());
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::getGoodCurrentUpgrade"), "method");
        params->setObject(__String::create(goodItemId), "goodItemId");
		__Dictionary *retParams = NULL;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//no store implementation for win32
		//TODO : we fake success

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		retParams = (__Dictionary *)CCNdkBridge::callNative(params, error);
#endif

        if (retParams == NULL) {
        	return "";
        }

		__String *retValue = (__String *) retParams->objectForKey("return");
		if (retValue) {
			return retValue->getCString();
		} else {
			return "";
		}
    }

    void CCStoreInventory::upgradeGood(char const *goodItemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling upgradeGood with: %s", goodItemId)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::upgradeGood"), "method");
        params->setObject(__String::create(goodItemId), "goodItemId");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//no store implementation for win32
		//TODO : we fake success

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CCNdkBridge::callNative(params, error);
#endif
    }

    void CCStoreInventory::removeGoodUpgrades(char const *goodItemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling removeGoodUpgrades with: %s", goodItemId)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::removeGoodUpgrades"), "method");
        params->setObject(__String::create(goodItemId), "goodItemId");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//no store implementation for win32
		//TODO : we fake success

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CCNdkBridge::callNative(params, error);
#endif
    }

    bool CCStoreInventory::nonConsumableItemExists(char const *nonConsItemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling nonConsumableItemExists with: %s", nonConsItemId)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::nonConsumableItemExists"), "method");
        params->setObject(__String::create(nonConsItemId), "nonConsItemId");
		__Dictionary *retParams = NULL;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//no store implementation for win32
		// we fake success
		auto ncitemstored = m_inventory.find(nonConsItemId);
		if (ncitemstored != m_inventory.end())
		{
			retParams = __Dictionary::create();
			retParams->setObject(CCBool::create(true), "return");
		}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		retParams = (__Dictionary *)CCNdkBridge::callNative(params, error);
#endif

        if (retParams == NULL) {
        	return false;
        }

		__Bool *retValue = (__Bool *) retParams->objectForKey("return");
		if (retValue) {
			return retValue->getValue();
		} else {
			return false;
		}
    }

    void CCStoreInventory::addNonConsumableItem(char const *nonConsItemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling addNonConsumableItem with: %s", nonConsItemId)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::addNonConsumableItem"), "method");
        params->setObject(__String::create(nonConsItemId), "nonConsItemId");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//no store implementation for win32
		// we fake success

		auto ncitemstored = m_inventory.find(nonConsItemId);
		if (ncitemstored != m_inventory.end())
		{
			m_inventory.at(nonConsItemId) += 1;
		}
		else
		{
			m_inventory.insert(std::make_pair(nonConsItemId, 1));
		}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CCNdkBridge::callNative(params, error);
#endif
   }

    void CCStoreInventory::removeNonConsumableItem(char const *nonConsItemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling removeNonConsumableItem with: %s", nonConsItemId)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::removeNonConsumableItem"), "method");
        params->setObject(__String::create(nonConsItemId), "nonConsItemId");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//no store implementation for win32
		// we fake success
		auto ncitemstored = m_inventory.find(nonConsItemId);
		if (ncitemstored != m_inventory.end() && m_inventory.at(nonConsItemId)> 1)
		{
			m_inventory.at(nonConsItemId) -= 1;
		}
		else
		{
			//TODO : trigger error
		}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CCNdkBridge::callNative(params, error);
#endif
    }
}
