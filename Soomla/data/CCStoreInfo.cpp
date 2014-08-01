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

#include "CCStoreInfo.h"
#include "../domain/virtualGoods/CCSingleUseVG.h"
#include "../domain/virtualGoods/CCEquippableVG.h"
#include "../domain/virtualGoods/CCSingleUsePackVG.h"
#include "../domain/virtualCurrencies/CCVirtualCurrency.h"
#include "../domain/virtualCurrencies/CCVirtualCurrencyPack.h"
#include "../domain/CCNonConsumableItem.h"
#include "../domain/CCMarketItem.h"
#include "../CCStoreUtils.h"
#include "CCNdkBridge.h"
#include "CCDomainFactory.h"
#include "CCDomainHelper.h"

namespace soomla {

#define TAG "SOOMLA StoreInfo"

#define SAFE_CREATE(__T__, __ret__, __retParams__)			\
    __Dictionary *retDict = (__Dictionary *)__retParams__->objectForKey("return"); \
    soomla::CCDomain *domain = CCDomainFactory::getInstance()->createWithDictionary(retDict); \
	__T__ __ret__ = dynamic_cast<__T__>(domain);			\
	CC_ASSERT(__ret__);
//========================

    USING_NS_CC;

    static CCStoreInfo *s_SharedStoreInfo = NULL;

    CCStoreInfo *CCStoreInfo::sharedStoreInfo() {
        return s_SharedStoreInfo;
    }

    void CCStoreInfo::createShared(CCStoreAssets *storeAssets) {
        CCStoreInfo *ret = new CCStoreInfo();
        if (ret->init(storeAssets)) {
            s_SharedStoreInfo = ret;
        } else {
            delete ret;
        }
    }

    bool CCStoreInfo::init(CCStoreAssets *storeAssets) {
        __Array *currenciesJSON = __Array::create();
        {
            __Array *currencies = storeAssets->getCurrencies();
            Ref *obj;
            CCARRAY_FOREACH(currencies, obj) {
				currenciesJSON->addObject(((CCVirtualCurrency *)obj)->toDictionary());
			}
        }

        __Array *packsJSON = __Array::create();
        {
            __Array *packs = storeAssets->getCurrencyPacks();
            Ref *obj;
            CCARRAY_FOREACH(packs, obj) {
				packsJSON->addObject(((CCVirtualCurrencyPack *)obj)->toDictionary());
			}
        }

        __Array *suGoods = __Array::create();
        __Array *ltGoods = __Array::create();
        __Array *eqGoods = __Array::create();
        __Array *upGoods = __Array::create();
        __Array *paGoods = __Array::create();

        Ref *obj;
        CCARRAY_FOREACH(storeAssets->getGoods(), obj) {
			if (dynamic_cast<CCSingleUseVG *>(obj)) {
				suGoods->addObject(((CCSingleUseVG *)obj)->toDictionary());
			} else if (dynamic_cast<CCEquippableVG *>(obj)) {
				eqGoods->addObject(((CCEquippableVG *)obj)->toDictionary());
			} else if (dynamic_cast<CCLifetimeVG *>(obj)) {
				ltGoods->addObject(((CCLifetimeVG *)obj)->toDictionary());
			} else if (dynamic_cast<CCSingleUsePackVG *>(obj)) {
				paGoods->addObject(((CCSingleUsePackVG *)obj)->toDictionary());
			} else if (dynamic_cast<CCUpgradeVG *>(obj)) {
				upGoods->addObject(((CCUpgradeVG *)obj)->toDictionary());
			}
		}

        __Dictionary *goodsJSON = __Dictionary::create();
        goodsJSON->setObject(suGoods, CCStoreConsts::JSON_STORE_GOODS_SU);
        goodsJSON->setObject(ltGoods, CCStoreConsts::JSON_STORE_GOODS_LT);
        goodsJSON->setObject(eqGoods, CCStoreConsts::JSON_STORE_GOODS_EQ);
        goodsJSON->setObject(upGoods, CCStoreConsts::JSON_STORE_GOODS_UP);
        goodsJSON->setObject(paGoods, CCStoreConsts::JSON_STORE_GOODS_PA);

        __Array *categoriesJSON = __Array::create();
        {
            __Array *categories = storeAssets->getCategories();
            Ref *obj;
            CCARRAY_FOREACH(categories, obj) {
				categoriesJSON->addObject(((CCVirtualCategory *)obj)->toDictionary());
			}
        }


        __Array *nonConsumablesJSON = __Array::create();
        {
            __Array *nonConsumables = storeAssets->getNonConsumableItems();
            Ref *obj;
            CCARRAY_FOREACH(nonConsumables, obj) {
				nonConsumablesJSON->addObject(((CCNonConsumableItem *)obj)->toDictionary());
			}
        }

        __Dictionary *storeAssetsObj = __Dictionary::create();
        storeAssetsObj->setObject(categoriesJSON, CCStoreConsts::JSON_STORE_CATEGORIES);
        storeAssetsObj->setObject(currenciesJSON, CCStoreConsts::JSON_STORE_CURRENCIES);
        storeAssetsObj->setObject(packsJSON, CCStoreConsts::JSON_STORE_CURRENCY_PACKS);
        storeAssetsObj->setObject(goodsJSON, CCStoreConsts::JSON_STORE_GOODS);
        storeAssetsObj->setObject(nonConsumablesJSON, CCStoreConsts::JSON_STORE_NON_CONSUMABLES);

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreAssets::init"), "method");
        params->setObject(__Integer::create(storeAssets->getVersion()), "version");
        params->setObject(storeAssetsObj, "storeAssets");
        CCNdkBridge::callNative (params, NULL);


        return true;
    }

    CCVirtualItem *CCStoreInfo::getItemByItemId(char const *itemId, CCError **error) {
        CCStoreUtils::logDebug(TAG,
							   __String::createWithFormat("Trying to fetch an item with itemId: %s", itemId)->getCString());

        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInfo::getItemByItemId"), "method");
        params->setObject(__String::create(itemId), "itemId");
        __Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, error);
        if (!*error) {
            SAFE_CREATE(CCVirtualItem *, ret, retParams);
            return ret;
        } else {
            CCStoreUtils::logDebug(TAG, __String::createWithFormat(
                    "getItemByItemId: Cannot get item: %s", itemId)->getCString());
            return NULL;
        }
    }

    CCPurchasableVirtualItem *CCStoreInfo::getPurchasableItemWithProductId(char const *productId, CCError **error) {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInfo::getPurchasableItemWithProductId"), "method");
        params->setObject(__String::create(productId), "productId");
        __Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, error);
        if (!*error) {
            SAFE_CREATE(CCPurchasableVirtualItem *, ret, retParams);
            return ret;
        } else {
            CCStoreUtils::logDebug(TAG, __String::createWithFormat(
                    "getPurchasableItemWithProductId: Cannot get item: %s", productId)->getCString());
            return NULL;
        }
    }

    CCVirtualCategory *CCStoreInfo::getCategoryForVirtualGood(char const *goodItemId, CCError **error) {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInfo::getCategoryForVirtualGood"), "method");
        params->setObject(__String::create(goodItemId), "goodItemId");
        __Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, error);
        if (!*error) {
            SAFE_CREATE(CCVirtualCategory *, ret, retParams);
            return ret;
        } else {
            CCStoreUtils::logDebug(TAG, __String::createWithFormat(
                    "getCategoryForVirtualGood: Cannot get item: %s", goodItemId)->getCString());
            return NULL;
        }
    }

    CCUpgradeVG *CCStoreInfo::getFirstUpgradeForVirtualGood(char const *goodItemId) {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInfo::getFirstUpgradeForVirtualGood"), "method");
        params->setObject(__String::create(goodItemId), "goodItemId");
        __Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, NULL);
        SAFE_CREATE(CCUpgradeVG *, ret, retParams);
        return ret;
    }

    CCUpgradeVG *CCStoreInfo::getLastUpgradeForVirtualGood(char const *goodItemId) {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInfo::getLastUpgradeForVirtualGood"), "method");
        params->setObject(__String::create(goodItemId), "goodItemId");
        __Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, NULL);
        SAFE_CREATE(CCUpgradeVG *, ret, retParams);
        return ret;
    }

    __Array *CCStoreInfo::getUpgradesForVirtualGood(char const *goodItemId) {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInfo::getUpgradesForVirtualGood"), "method");
        params->setObject(__String::create(goodItemId), "goodItemId");
		__Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, NULL);
		__Array *retArray = (__Array *)retParams->objectForKey("return");

        return CCDomainHelper::getInstance()->getDomainsFromDictArray(
                retArray, CCStoreConsts::JSON_JSON_TYPE_UPGRADE_VG);
    }

    __Array *CCStoreInfo::getVirtualCurrencies() {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInfo::getVirtualCurrencies"), "method");
		__Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, NULL);
		__Array *retArray = (__Array *)retParams->objectForKey("return");

        return CCDomainHelper::getInstance()->getDomainsFromDictArray(
                retArray, CCStoreConsts::JSON_JSON_TYPE_VIRTUAL_CURRENCY);
    }

    __Array *CCStoreInfo::getVirtualGoods() {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInfo::getVirtualGoods"), "method");
		__Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, NULL);
		__Array *retArray = (__Array *)retParams->objectForKey("return");

        return CCDomainHelper::getInstance()->getDomainsFromDictArray(retArray);
    }

    __Array *CCStoreInfo::getVirtualCurrencyPacks() {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInfo::getVirtualCurrencyPacks"), "method");
        __Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, NULL);
		__Array *retArray = (__Array *)retParams->objectForKey("return");

        return CCDomainHelper::getInstance()->getDomainsFromDictArray(
                retArray, CCStoreConsts::JSON_JSON_TYPE_VIRTUAL_CURRENCY_PACK);
    }

    __Array *CCStoreInfo::getNonConsumableItems() {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInfo::getNonConsumableItems"), "method");
		__Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, NULL);
		__Array *retArray = (__Array *)retParams->objectForKey("return");

        return CCDomainHelper::getInstance()->getDomainsFromDictArray(
                retArray, CCStoreConsts::JSON_JSON_TYPE_NON_CONSUMABLE_ITEM);
    }

    __Array *CCStoreInfo::getVirtualCategories() {
        __Dictionary *params = __Dictionary::create();
        params->setObject(__String::create("CCStoreInfo::getVirtualCategories"), "method");
		__Dictionary *retParams = (__Dictionary *) CCNdkBridge::callNative (params, NULL);
		__Array *retArray = (__Array *)retParams->objectForKey("return");

        return CCDomainHelper::getInstance()->getDomainsFromDictArray(
                retArray, CCStoreConsts::JSON_JSON_TYPE_VIRTUAL_CATEGORY);
    }

#undef SAFE_CREATE

}

