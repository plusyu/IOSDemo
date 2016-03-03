//
//  Lighter.h
//  IOSDemo
//
//  Created by yuxiang on 14/11/21.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Lighter : NSObject

@property (nonatomic) int64_t identifier;
@property (nonatomic, copy) NSString* name;
@property (nonatomic, strong) NSDate* creationDate;
@property (nonatomic) double rating;


- (NSString*)authorFullName;
- (double)adjustedRating;
@end
