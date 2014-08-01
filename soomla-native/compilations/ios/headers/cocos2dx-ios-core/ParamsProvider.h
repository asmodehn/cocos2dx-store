//
// Created by Fedor Shubin on 6/25/14.
//

#import <Foundation/Foundation.h>


@interface ParamsProvider : NSObject
+ (id)sharedParamsProvider;

- (void)setParams:(NSDictionary *)params withKey:(NSString *)key;
- (NSDictionary *)getParamsForKey:(NSString *)key;
@end

