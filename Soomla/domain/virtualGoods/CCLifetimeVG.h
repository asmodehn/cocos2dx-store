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

#ifndef __CCLifetimeVG_H_
#define __CCLifetimeVG_H_

#include "CCVirtualGood.h"

namespace soomla {
	/** 
     @class CCLifetimeVG
     @brief A lifetime virtual good.
    
     A `CCLifetimeVG` is a virtual good that is bought once and kept forever.
     
     The `CCLifetimeVG`'s characteristics are:
     1. Can only be purchased once.
     2. Your users cannot have more than one of this item.
     
     Real Games Examples: 'No Ads', 'Double Coins'
     
     NOTE: In case you want this item to be available for purchase with real $$
     you'll need to define it in the Market (Google Play, App Store, etc...).
     
     Inheritance: CCLifeTimeVG > CCVirtualGood > CCPurchasableVirtualItem >
     CCVirtualItem
     */
    class CCLifetimeVG : public CCVirtualGood {
    
    public:
        
		/**
         Creates a `CCLifetimeVG`.
         @param name see parent.
         @param description see parent.
         @param itemId see parent.
         @param purchaseType see parent.
         @return The Lifetime virtual good.
		*/
        static CCLifetimeVG *create(cocos2d::__String* name, cocos2d::__String* description,
                cocos2d::__String* itemId, CCPurchaseType * purchaseType);
		
		/**
         Creates a `CCLifetimeVG` from a dictionary.
         @param dict A dictionary containing keys to each of the parameters 
                required by the `create` function.
         @return The Lifetime virtual good.
		*/
        SL_CREATE_WITH_DICTIONARY(CCLifetimeVG);

        virtual cocos2d::__Dictionary *toDictionary() override;
    };
};

#endif //__CCLifetimeVG_H_
