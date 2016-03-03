//
//  CallBackBlock.m
//  IOSDemo
//
//  Created by yuxiang on 15/1/30.
//  Copyright (c) 2015年 yu. All rights reserved.
//

#import "CallBackBlock.h"

@implementation CallBackBlock


-(id)initWithCallback:(MyCallbackBlock)block{
    if (self = [super init]) {
        self.callbackBlock = block;
    }
    return self;
}

- (void)playMovie:(NSString *)title {
    // play the movie
    self.callbackBlock(title);
}

- (void)dealloc {
    self.callbackBlock  = nil;
}

@end
