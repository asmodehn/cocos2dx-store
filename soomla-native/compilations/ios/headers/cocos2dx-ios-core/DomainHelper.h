//
// Created by Fedor Shubin on 6/11/14.
//

#import <Foundation/Foundation.h>

@interface DomainHelper : NSObject
+ (id)sharedDomainHelper;
- (void) registerType: (NSString *)type withClassName: (NSString *)className;
- (id) getDomainsFromDictList: (NSArray *)dictList;
- (id) getDictListFromDomains: (NSArray *)domains;
- (NSDictionary *) domainToDict: (NSObject *)domain;
@end