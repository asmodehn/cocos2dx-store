//
// Created by Fedor Shubin on 6/10/14.
//

#import <Foundation/Foundation.h>


@interface DomainFactory : NSObject
+ (id)sharedDomainFactory;
- (void)registerCreatorForKey: (NSString *)key withBlock: (id (^)(NSDictionary *dict)) creator;
- (id)createWithDict: (NSDictionary *)dict;
@end


