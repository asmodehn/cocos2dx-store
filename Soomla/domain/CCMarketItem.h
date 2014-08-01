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

#ifndef __CCMarketItem_H_
#define __CCMarketItem_H_

#include "cocos2d.h"
#include "CCSoomlaMacros.h"
#include "CCDomain.h"
#include "CCStoreConsts.h"

namespace soomla {
	/** 
     @class CCMarketItem
     @brief An item you can purchase in the market (App Store, Google Play...).

     This class represents an item in the market (App Store, Google Play...).
     Every `CCPurchasableVirtualItem` with `CCPurchaseType` of 
     `CCPurchaseWithMarket` has an instance of this class.
	 */
    class CCMarketItem : public CCDomain {
        SL_SYNTHESIZE_RETAIN_WITH_DICT(cocos2d::__String *, mProductId, ProductId, CCStoreConsts::JSON_MARKET_ITEM_PRODUCT_ID);
        SL_SYNTHESIZE_RETAIN_WITH_DICT(cocos2d::__Integer *, mConsumable, Consumable, CCStoreConsts::JSON_MARKET_ITEM_CONSUMABLE);
        SL_SYNTHESIZE_RETAIN_WITH_DICT(cocos2d::__Double *, mPrice, Price, CCStoreConsts::JSON_MARKET_ITEM_PRICE);
        CC_SYNTHESIZE_RETAIN(cocos2d::__String *, mMarketPrice, MarketPrice);
        CC_SYNTHESIZE_RETAIN(cocos2d::__String *, mMarketTitle, MarketTitle);
        CC_SYNTHESIZE_RETAIN(cocos2d::__String *, mMarketDescription, MarketDescription);
    public:
        
        /**
         Every item offered in the market (App Store, Google Play...) has one 
         of the five following purchase types.
         */
        enum Consumable {
            NONCONSUMABLE,
            CONSUMABLE,
            SUBSCRIPTION,
        };
        CCMarketItem(): mProductId(NULL), mConsumable(NULL), mPrice(NULL), mMarketPrice(NULL), mMarketTitle(NULL), mMarketDescription(NULL) {};

		/**
         Creates a `CCMarketItem`.
         @param productId The item's id in the native store.
         @param consumable Whether or not the item is consumable.
         @param price The price of the item, in USD.
         @return The Market item.
		 */
        static CCMarketItem *create(cocos2d::__String *productId, cocos2d::__Integer *consumable, cocos2d::__Double * price);

		/**
         Creates a `CCMarketItem` from a dictionary.
         @param dict A dictionary containing keys for the item's product id,
                price and consumable status.
         @return The Market item.
		*/
        SL_CREATE_WITH_DICTIONARY(CCMarketItem);

        bool init(cocos2d::__String *productId, cocos2d::__Integer *consumable, cocos2d::__Double * price);
        virtual bool initWithDictionary(cocos2d::__Dictionary* dict);

        virtual ~CCMarketItem();

        /**
        Converts this `CCMarketItem` to a `CCDictionary`.
        @return `CCDictionary` representation of this `CCMarketItem`.
        */
        virtual cocos2d::__Dictionary* toDictionary();
    };
};


#endif //__CCMarketItem_H_
