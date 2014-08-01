//
// Created by Fedor on 04/06/14.
//

#include "CCVirtualItemReward.h"
#include "CCStoreConsts.h"

soomla::CCVirtualItemReward *soomla::CCVirtualItemReward::create(
        cocos2d::__String *rewardId,
        cocos2d::__String *name,
        cocos2d::__Bool *repeatable,
        cocos2d::__Integer *amount,
        cocos2d::__String *associatedItemId
) {

    CCVirtualItemReward *ret = new CCVirtualItemReward();
    if (ret->init(rewardId, name, repeatable, amount, associatedItemId)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool soomla::CCVirtualItemReward::init(
        cocos2d::__String *rewardId,
        cocos2d::__String *name,
        cocos2d::__Bool *repeatable,
        cocos2d::__Integer *amount,
        cocos2d::__String *associatedItemId
) {

    bool result = CCReward::init(rewardId, name, repeatable);

    if (result) {
        setAmount(amount);
        setAssociatedItemId(associatedItemId);
        return true;
    } else {
        return false;
    }
}

bool soomla::CCVirtualItemReward::initWithDictionary(cocos2d::__Dictionary *dict) {
    CCReward::initWithDictionary(dict);

    fillAmountFromDict(dict);
    fillAssociatedItemIdFromDict(dict);

    return true;
}

cocos2d::__Dictionary *soomla::CCVirtualItemReward::toDictionary() {
    cocos2d::__Dictionary *dict = CCReward::toDictionary();

    putAmountToDict(dict);
    putAssociatedItemIdToDict(dict);

    return dict;
}

soomla::CCVirtualItemReward::~CCVirtualItemReward() {
    CC_SAFE_RELEASE(mAmount);
    CC_SAFE_RELEASE(mAssociatedItemId);
}

const char *soomla::CCVirtualItemReward::getType() {
    return CCStoreConsts::JSON_JSON_TYPE_ITEM;
}
