//
//  SearchViewModel.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/28.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "SearchViewModel.h"
#import <Accounts/Accounts.h>
#import <Social/Social.h>

@interface SearchViewModel ()

@property (strong, nonatomic) ACAccountStore *accountStore;
@property (strong, nonatomic) ACAccountType *twitterAccountType;

@end

typedef NS_ENUM(NSInteger, YUSinaWeiboInstantError) {
   YUSinaWeiboInstantErrorAccessDenied,
   YUSinaWeiboInstantErrorNoTwitterAccounts,
   YUSinaWeiboInstantErrorInvalidResponse
};

static NSString * const YUSinaWeiboInstantDomain = @"SinaWeiboInstant";

@implementation SearchViewModel

- (id)init {
    if (self = [super init]) {
        [self initialize];
    }
    return self;
}

-(void)initialize
{
    // set up access to weibo
    self.accountStore = [[ACAccountStore alloc] init];
    self.twitterAccountType = [self.accountStore accountTypeWithAccountTypeIdentifier:ACAccountTypeIdentifierSinaWeibo];
}


- (RACSignal *)requestAccessToTwitterSignal {
    // 1 - define an error
    NSError *accessError = [NSError errorWithDomain:YUSinaWeiboInstantDomain
                                               code:YUSinaWeiboInstantErrorAccessDenied
                                           userInfo:nil];
    // 2 - create the signal
//    @weakify(self)
//    return [RACSignal createSignal:^RACDisposable *(id<RACSubscriber> subscriber) {}];
    return nil;
    
    
}

@end
