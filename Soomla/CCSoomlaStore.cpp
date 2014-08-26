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

// Created by Fedor Shubin on 5/20/13.

#include "CCSoomlaStore.h"
#include "CCNdkBridge.h"
#include "CCStoreEventDispatcher.h"

using namespace cocos2d;

namespace soomla {
    #define TAG "SOOMLA SoomlaStore"

    USING_NS_CC;

    static CCSoomlaStore *s_SharedSoomlaStore = NULL;

    CCSoomlaStore *CCSoomlaStore::getInstance() {
        if (!s_SharedSoomlaStore)
        {
            s_SharedSoomlaStore = new CCSoomlaStore();
            s_SharedSoomlaStore->retain();
        }
        return s_SharedSoomlaStore;
    }

    CCSoomlaStore::CCSoomlaStore() {
    }

    CCSoomlaStore::~CCSoomlaStore() {

    }

    void CCSoomlaStore::buyMarketItem(const char *productId, const char *payload, CCError **error) {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCSoomlaStore::buyMarketItem"), "method");
        params->setObject(__String::create(productId), "productId");
        params->setObject(__String::create(payload), "payload");
        CCNdkBridge::callNative (params, error);
    }

    void CCSoomlaStore::restoreTransactions() {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCSoomlaStore::restoreTransactions"), "method");
        CCNdkBridge::callNative (params, NULL);
    }

    void CCSoomlaStore::refreshInventory() {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCSoomlaStore::refreshInventory"), "method");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		CCStoreEventDispatcher::getInstance()->onMarketItemsRefreshStarted();
		//TODO : Simulate market refresh : set marketPrice on curency packs
		//onMarketItemRefreshed(soomla::CCMarketItem *mi)
		CCStoreEventDispatcher::getInstance()->onMarketItemsRefreshed();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        CCNdkBridge::callNative (params, NULL);
#endif
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    bool CCSoomlaStore::transactionsAlreadyRestored() {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCSoomlaStore::transactionsAlreadyRestored"), "method");
        __Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, NULL);
        __Bool *retValue = (__Bool *) retParams->objectForKey("return");
        return retValue->getValue();
    }

    void CCSoomlaStore::refreshMarketItemsDetails(CCError **error) {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCSoomlaStore::refreshMarketItemsDetails"), "method");
        CCNdkBridge::callNative (params, error);
    }
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    void CCSoomlaStore::startIabServiceInBg() {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCSoomlaStore::startIabServiceInBg"), "method");
        CCNdkBridge::callNative (params, NULL);
    }

    void CCSoomlaStore::stopIabServiceInBg() {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCSoomlaStore::stopIabServiceInBg"), "method");
        CCNdkBridge::callNative (params, NULL);
    }
#endif
}
