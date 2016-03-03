//
//  NetWorkUtil.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/26.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "NetWorkUtil.h"
#import "AFNetworking.h"

@implementation NetWorkUtil

+(void)isOK
{
    NSURL *baseURL = [NSURL URLWithString:@"http://www.baidu.com/"];
    AFHTTPRequestOperationManager *manager = [[AFHTTPRequestOperationManager alloc] initWithBaseURL:baseURL];
    
    NSOperationQueue *operationQueue = manager.operationQueue;
    [manager.reachabilityManager setReachabilityStatusChangeBlock:^(AFNetworkReachabilityStatus status) {
        switch (status) {
            case AFNetworkReachabilityStatusReachableViaWWAN:
                NSLog(@"GPRS");
                break;
            case AFNetworkReachabilityStatusReachableViaWiFi:
                [operationQueue setSuspended:NO];
                  NSLog(@"WiFi");
                break;
            case AFNetworkReachabilityStatusNotReachable:
                 NSLog(@"NO network");
                break;
            default:
                [operationQueue setSuspended:YES];
                break;
        }
    }];
    
    [manager.reachabilityManager startMonitoring];

}

@end
