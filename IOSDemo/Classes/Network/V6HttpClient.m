//
//  V6HttpClient.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/24.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "V6HttpClient.h"
#import "AFNetworking.h"
#import "Reachability.h"
#import "V6ResponseSerializer.h"

@interface V6HttpClient()

@property(nonatomic,strong)AFHTTPSessionManager *sessionManger;

@end

@implementation V6HttpClient

- (id)init{
    if (self = [super init]){
        self.sessionManger = [AFHTTPSessionManager manager];
        //请求参数序列化类型
        self.sessionManger.requestSerializer = [AFJSONRequestSerializer serializer];
        //添加头部
        [self.sessionManger.requestSerializer setValue:@"1.1" forHTTPHeaderField:@"UserAgent"];
        //响应结果序列化类型
        self.sessionManger.responseSerializer = [V6ResponseSerializer serializer];
        self.sessionManger.responseSerializer.acceptableContentTypes = [NSSet setWithObject:@"text/html"];
        
    }
    return self;
}

+ (V6HttpClient *)defaultClient
{
    static V6HttpClient *instance = nil;
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        instance = [[self alloc] init];
    });
    return instance;
}

-(void)requestWithURL:(NSString *)url method:(NSInteger)method parameters:(id)parameters prepareExecute:(PrepareExecuteBlock)prepare success:(void (^)(NSURLSessionDataTask *, id))success failure:(void (^)(NSURLSessionDataTask *, NSError *))failure
{
    if (prepare) {
        prepare();
    }
    switch (method) {
        case V6HttpRequestGet:
        {
            [self.sessionManger GET:url parameters:parameters success:success failure:failure];
             break;
        }
        case V6HttpRequestPost:
        {
            [self.sessionManger POST:url parameters:parameters success:success failure:failure];
              break;
        }
    }
}


-(void)getWithURL:(NSString *)url parameters:(id)parameters prepareExecute:(PrepareExecuteBlock) prepare success:(void (^)(NSURLSessionDataTask *task, id json))success
          failure:(void (^)(NSURLSessionDataTask * task, NSError *error))failure
{
    if (prepare) {
        prepare();
    }
    [self.sessionManger GET:url parameters:parameters success:success failure:failure];
}


-(void)postWithURL:(NSString *)url parameters:(id)parameters prepareExecute:(PrepareExecuteBlock)prepare success:(void (^)(NSURLSessionDataTask *task, id json))success failure:(void (^)(NSURLSessionDataTask *task, NSError *error))failure
{

        if (prepare) {
            prepare();
        }
        [self.sessionManger POST:url parameters:parameters success:success failure:failure];
}

@end
