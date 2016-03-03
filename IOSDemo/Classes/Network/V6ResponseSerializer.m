//
//  V6ResponseSerializer.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/25.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "V6ResponseSerializer.h"
#import "Mantle.h"

@implementation V6ResponseSerializer


- (id)responseObjectForResponse:(NSURLResponse *)response
                           data:(NSData *)data
                          error:(NSError *__autoreleasing *)error
{
    NSLog(@"responseObjectForResponse");
    if (![self validateResponse:(NSHTTPURLResponse *)response data:data error:error]) {
        NSLog(@"validateResponse");
        if (*error != nil) {
            NSMutableDictionary *userInfo = [(*error).userInfo mutableCopy];
            userInfo[@"JSONResponseSerializerWithDataKey"] = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
            NSError *newError = [NSError errorWithDomain:(*error).domain code:(*error).code userInfo:userInfo];
            (*error) = newError;
        }
        return (nil);
    }
    return ([super responseObjectForResponse:response data:data error:error]);
}

@end
