//
//  V6Error.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/25.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "V6Error.h"

@implementation V6Error

+(NSDictionary *) JSONKeyPathsByPropertyKey
{
    return @{
             @"flag": @"flag",
             @"content": @"content",
             };
}

@end
