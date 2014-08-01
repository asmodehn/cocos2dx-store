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
        CCNdkBridge::callNative (params, error);
    }

    int CCStoreInventory::getItemBalance(char const *itemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling getItemBalance with: %s", itemId)->getCString());
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::getItemBalance"), "method");
        params->setObject(__String::create(itemId), "itemId");
        __Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, error);

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
        CCNdkBridge::callNative (params, error);
    }

    void CCStoreInventory::takeItem(char const *itemId, int amount, CCError **error) {
        CCStoreUtils::logDebug(TAG, __String::createWithFormat(
                "SOOMLA/COCOS2DX Calling takeItem with itemId: %s and amount %d", itemId, amount)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::takeItem"), "method");
        params->setObject(__String::create(itemId), "itemId");
        params->setObject(__Integer::create(amount), "amount");
        CCNdkBridge::callNative (params, error);
    }

    void CCStoreInventory::equipVirtualGood(char const *itemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling equipVirtualGood with: %s", itemId)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::equipVirtualGood"), "method");
        params->setObject(__String::create(itemId), "itemId");
        CCNdkBridge::callNative (params, error);
   }

    void CCStoreInventory::unEquipVirtualGood(char const *itemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling unEquipVirtualGood with: %s", itemId)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::unEquipVirtualGood"), "method");
        params->setObject(__String::create(itemId), "itemId");
        CCNdkBridge::callNative (params, error);
    }

    bool CCStoreInventory::isVirtualGoodEquipped(char const *itemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling isVirtualGoodEquipped with: %s", itemId)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::isVirtualGoodEquipped"), "method");
        params->setObject(__String::create(itemId), "itemId");
        __Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, error);

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
        __Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, error);

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
        __Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, error);

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
        CCNdkBridge::callNative (params, error);
    }

    void CCStoreInventory::removeGoodUpgrades(char const *goodItemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling removeGoodUpgrades with: %s", goodItemId)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::removeGoodUpgrades"), "method");
        params->setObject(__String::create(goodItemId), "goodItemId");
        CCNdkBridge::callNative (params, error);
    }

    bool CCStoreInventory::nonConsumableItemExists(char const *nonConsItemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling nonConsumableItemExists with: %s", nonConsItemId)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::nonConsumableItemExists"), "method");
        params->setObject(__String::create(nonConsItemId), "nonConsItemId");
        __Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, error);

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
        CCNdkBridge::callNative (params, error);
   }

    void CCStoreInventory::removeNonConsumableItem(char const *nonConsItemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
                __String::createWithFormat("SOOMLA/COCOS2DX Calling removeNonConsumableItem with: %s", nonConsItemId)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInventory::removeNonConsumableItem"), "method");
        params->setObject(__String::create(nonConsItemId), "nonConsItemId");
        CCNdkBridge::callNative (params, error);
    }
}
