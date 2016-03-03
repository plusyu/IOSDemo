//
//  ChoosyAchorInfo.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/25.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "ChoosyAchorInfo.h"

@implementation ChoosyAchorInfo

+(NSDictionary *) JSONKeyPathsByPropertyKey
{
    return @{
             @"uid": @"uid",
             @"rid": @"rid",
             @"username": @"username",
             @"picuser": @"picuser",
             @"coin6rank": @"coin6rank",
             @"wealthrank": @"wealthrank",
             @"ltime": @"ltime",
             @"isLive": @"isLive",
             };
}



@end
