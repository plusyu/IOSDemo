//
//  AlertUtil.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/25.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "AlertUtil.h"
#import "MBProgressHUD.h"
#import "V6ResponseError.h"
#import "SVProgressHUD.h"
#import <UIKit/UIKit.h>

NSString *const defalutMsg = @"网络连接出错";

@implementation AlertUtil


+(void)handleExcption:(NSError *)error
{

    NSString *msg;
    if ([error isMemberOfClass:[V6ResponseError class]]) {
        msg = [error domain];
    }else{
        msg = defalutMsg;
    }
      NSLog(@"提示信息:%@",msg);
    [self showExceptionDialog:msg];
}

//弹出网络错误提示框
+ (void)showExceptionDialog:(NSString *)msg
{
    [[[UIAlertView alloc] initWithTitle:@"提示"
                                message:msg
                               delegate:nil
                      cancelButtonTitle:@"好的"
                      otherButtonTitles:nil, nil] show];
}
@end
