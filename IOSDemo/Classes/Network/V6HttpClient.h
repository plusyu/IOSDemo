//
//  V6HttpClient.h
//  IOSDemo
//
//  Created by yuxiang on 14/11/24.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import <Foundation/Foundation.h>


typedef NS_ENUM(NSInteger, V6HttpRequestType) {
    V6HttpRequestGet,
    V6HttpRequestPost,
    V6HttpRequestDelete,
    V6HttpRequestPut,
};

typedef void(^PrepareExecuteBlock)(void);


@interface V6HttpClient : NSObject

+ (V6HttpClient *)defaultClient;

-(void)getWithURL:(NSString *)url parameters:(id)parameters prepareExecute:(PrepareExecuteBlock) prepare success:(void (^)(NSURLSessionDataTask *task, id json))success
          failure:(void (^)(NSURLSessionDataTask *task, NSError *error))failure;

-(void)postWithURL:(NSString *)url parameters:(id)parameters  prepareExecute:(PrepareExecuteBlock) prepare success:(void (^)(NSURLSessionDataTask *task, id json))success
           failure:(void (^)(NSURLSessionDataTask *task, NSError *))failure;

- (void)requestWithURL:(NSString *)url
                 method:(NSInteger)method
             parameters:(id)parameters
         prepareExecute:(PrepareExecuteBlock) prepare
                success:(void (^)(NSURLSessionDataTask *task, id responseObject))success
                failure:(void (^)(NSURLSessionDataTask *task, NSError *error))failure;


@end
