//
// Created by Fedor Shubin on 6/15/14.
//

#import <Foundation/Foundation.h>


@interface ServiceManager : NSObject
+ (id)sharedServiceManager;

- (void)registerService:(NSObject *)service;
@end