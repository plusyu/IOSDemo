//
//  CallBackBlock.h
//  IOSDemo
//
//  Created by yuxiang on 15/1/30.
//  Copyright (c) 2015年 yu. All rights reserved.
//

#import <Foundation/Foundation.h>


typedef void (^MyCallbackBlock)(NSString *);
@interface CallBackBlock : NSObject

@property(nonatomic,copy)MyCallbackBlock callbackBlock;

-(id)initWithCallback:(MyCallbackBlock)block;
- (void)playMovie:(NSString *)title;
@end
