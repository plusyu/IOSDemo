//
//  LoginTask.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/24.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "LoginTask.h"
#import "V6HttpClient.h"
#import "AchorInfo.h"
#import "V6Success.h"
#import "BaseModel.h"
#import "V6Error.h"
//通过昵称搜索主播接口
//
//地址：http://v.6.cn/coop/mobile/index.php
//
//参数：
//    padapi   =  coop-mobile-searchAnchor.php ，
//    encpass  => encpass，统计用，可以不传递
//    logiuid  => 当前登录者的uid，统计用。可以不传递
//    rate  => 获取数据级别
//    key  => 搜索关键字
//    p  => 页码
//    size  => 每页显示数
//    t => 搜索类型，默认为昵称搜索，r为搜索房间号
// http://v.6.cn/coop/mobile/index.php?padapi=coop-mobile-searchAnchor.php&rate=1&key=3098&p=1&size=1&t=r

#define KURL @"http://v.6.cn/coop/mobile/index.php"

@implementation LoginTask


-(void)execute:(NSString *)str onSuccess:(SuccessBlock)success exception:(ExceptionBlock)exception
{
        NSDictionary *params = @{@"padapi": @"coop-mobile-searchAnchor.php",
                                 @"rate": @"1",
                                 @"key": @"3098",
                                 @"p": @"1",
                                 @"size": @"1",
                                 @"t": @"r",
                                 };
    
        V6HttpClient *client = [[V6HttpClient alloc]init];
        [client getWithURL:KURL parameters:params prepareExecute:^{
            //显示对话框
            [super showPrepareWithMsg:@"正在登录中"];
        } success:^(NSURLSessionDataTask *task, id json) {
            [super dismissPrepare];
            id result =   [super handlerJson:json model:[AchorInfo class]];
           [result isMemberOfClass:[AchorInfo class]] ?success(result):exception(result);
        } failure:^(NSURLSessionDataTask *task, NSError *error) {
            [super dismissPrepare];
            exception(error);
        }];
//        [client postWithURL:KURL parameters:params prepareExecute:^{
//            //显示对话框
//            [super showPrepareWithMsg:@"正在登录中"];
//        } success:^(NSURLSessionDataTask *task, id json) {
//            [super dismissPrepare];
//            id result =   [super handlerJson:json model:[AchorInfo class]];
//            [result isMemberOfClass:[AchorInfo class]] ?success(result):exception(result);
//        } failure:^(NSURLSessionDataTask *task, NSError *error) {
//            [super dismissPrepare];
//            exception(error);
//        }];
}


@end
