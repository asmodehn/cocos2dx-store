//
// Created by Fedor Shubin on 6/19/14.
//

#ifndef __CCStoreEventDispatcher_H_
#define __CCStoreEventDispatcher_H_

#include "CCStoreEventHandler.h"
#include "CCAbstractAggregatedEventHandler.h"

namespace soomla {
    class CCStoreEventDispatcher: public CCAbstractAggregatedEventHandler<CCStoreEventHandler>, public CCStoreEventHandler {
    public:
        /**
        This class is singleton, access it with this function.
        */
        static CCStoreEventDispatcher *getInstance();

        bool init();

        virtual void onBillingNotSupported();

        virtual void onBillingSupported();

        virtual void onCurrencyBalanceChanged(CCVirtualCurrency *virtualCurrency, int balance, int amountAdded);

        virtual void onGoodBalanceChanged(CCVirtualGood *virtualGood, int balance, int amountAdded);

        virtual void onGoodEquipped(CCEquippableVG *equippableVG);

        virtual void onGoodUnEquipped(CCEquippableVG *equippableVG);

        virtual void onGoodUpgrade(CCVirtualGood *virtualGood, CCUpgradeVG *upgradeVG);

        virtual void onItemPurchased(CCPurchasableVirtualItem *purchasableVirtualItem);

        virtual void onItemPurchaseStarted(CCPurchasableVirtualItem *purchasableVirtualItem);

        virtual void onMarketPurchaseCancelled(CCPurchasableVirtualItem *purchasableVirtualItem);

        virtual void onMarketPurchase(CCPurchasableVirtualItem *purchasableVirtualItem, cocos2d::__String *token, cocos2d::__String *payload);

        virtual void onMarketPurchaseStarted(CCPurchasableVirtualItem *purchasableVirtualItem);

        virtual void onMarketPurchaseVerification(CCPurchasableVirtualItem *purchasableVirtualItem);

        virtual void onRestoreTransactionsFinished(bool success);

        virtual void onRestoreTransactionsStarted();

        virtual void onUnexpectedErrorInStore();

        virtual void onStoreControllerInitialized();

        virtual void onMarketItemsRefreshed();

/** Android only events*/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

        virtual void onMarketRefund(CCPurchasableVirtualItem *purchasableVirtualItem);

        virtual void onIabServiceStarted();

        virtual void onIabServiceStopped();
#endif

        virtual void onMarketItemsRefreshStarted();
    };
}


#endif //__CCStoreEventDispatcher_H_
