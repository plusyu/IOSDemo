//
//  BaseTask.h
//  IOSDemo
//
//  Created by yuxiang on 14/11/24.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void(^SuccessBlock)(id obj);
typedef void(^ExceptionBlock)(id obj);

@interface BaseTask : NSObject

-(void)showPrepareWithMsg:(NSString *)msg;
-(void)dismissPrepare;

-(id)handlerJson:(id)json model:(Class)baseClass;

@end
