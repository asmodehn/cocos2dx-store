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

//  Created by Igor Yegoroff on 5/16/13.

#ifndef __CCVirtualItem_H__
#define __CCVirtualItem_H__

#include "cocos2d.h"
#include "CCDomain.h"
#include "CCStoreConsts.h"
#include "CCSoomlaMacros.h"
#include "CCSoomlaEntity.h"

namespace soomla {
	/** 
     @class CCVirtualItem
     @brief A virtual item.

     This is the parent class of all virtual items in the application.
     Almost every entity in your virtual economy will be a virtual item. There 
     are many types of virtual items - each one of the various types extends
     `CCVirtualItem` and adds its own behavior on top of it.
	*/    
    class CCVirtualItem : public CCSoomlaEntity {
    public:

		/**
         Creates an instance of `CCVirtualItem`.
         @param name The item's name.
         @param description The item's description.
         @param itemId The item's itemId.
         @return The virtual item.
		*/
        static CCVirtualItem * create(cocos2d::__String* name, cocos2d::__String* description, cocos2d::__String* itemId);

        /**
         Creates a `CCVirtualItem` from a dictionary.
         @param dict A dictionary containing keys to each of the parameters 
                required by the create function.
         @return The item.
		*/
        SL_CREATE_WITH_DICTIONARY(CCVirtualItem);

        /**
        Converts this `CCVirtualItem` to a `CCDictionary`.
        @return `CCDictionary` representation of this `CCVirtualItem`.
        */
        virtual cocos2d::__Dictionary* toDictionary();

        cocos2d::__String *getItemId();
    };
    
}

#endif /* defined(__CCVirtualItem_H__) */
