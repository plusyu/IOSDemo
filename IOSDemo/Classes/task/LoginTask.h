//
//  LoginTask.h
//  IOSDemo
//
//  Created by yuxiang on 14/11/24.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "BaseTask.h"

@interface LoginTask : BaseTask

-(void)execute:(NSString *)str onSuccess:(SuccessBlock)success exception:(ExceptionBlock)exception;
@end
