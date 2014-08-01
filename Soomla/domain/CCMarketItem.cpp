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

// Created by Fedor Shubin on 5/19/13.

#include "CCMarketItem.h"

USING_NS_CC;

namespace soomla {

    CCMarketItem *CCMarketItem::create(__String *productId, __Integer *consumable, __Double *price) {
        CCMarketItem *ret = new CCMarketItem();
        if (ret->init(productId, consumable, price)) {
            ret->autorelease();
        }
        else {
            CC_SAFE_DELETE(ret);
        }

        return ret;
    }

    bool CCMarketItem::init(__String *productId, __Integer *consumable, __Double *price) {
        setProductId(productId);
        setConsumable(consumable);
        setPrice(price);

        return true;
    }

    bool CCMarketItem::initWithDictionary(__Dictionary *dict) {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        char const* key = CCStoreConsts::JSON_MARKET_ITEM_ANDROID_ID;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        char const* key = CCStoreConsts::JSON_MARKET_ITEM_IOS_ID;
#endif
        cocos2d::Ref* obj = dict->objectForKey(key);
        CCAssert(obj == NULL || dynamic_cast<__String *>(obj), "invalid object type in dictionary");
        setProductId((__String *)obj);

        if (mProductId == NULL) {
            fillProductIdFromDict(dict);
        }

        fillConsumableFromDict(dict);
        fillPriceFromDict(dict);

        return true;
    }

    CCMarketItem::~CCMarketItem() {
        CC_SAFE_RELEASE(mProductId);
        CC_SAFE_RELEASE(mConsumable);
        CC_SAFE_RELEASE(mPrice);
    }

    __Dictionary *CCMarketItem::toDictionary() {
        __Dictionary *dict = __Dictionary::create();

        putProductIdToDict(dict);
        putConsumableToDict(dict);
        putPriceToDict(dict);

        return this->putTypeData(dict, CCStoreConsts::JSON_JSON_TYPE_MARKET_ITEM);
    }
};

