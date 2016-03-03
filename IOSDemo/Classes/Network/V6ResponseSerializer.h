//
//  V6ResponseSerializer.h
//  IOSDemo
//
//  Created by yuxiang on 14/11/25.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "AFURLResponseSerialization.h"

@protocol V6ResponseSerializer <NSObject>
@required
- (Class)modelClassForResponse:(NSURLResponse *)response data:(NSData *)data error:(NSError *)error;
@end

@interface V6ResponseSerializer : AFJSONResponseSerializer

@property (nonatomic, strong, readonly) id<V6ResponseSerializer> modelMatcher;

+ (instancetype)serializerForModelClass:(Class)modelClass;

+ (instancetype)serializerWithModelMatcher:(id<V6ResponseSerializer>)modelMatcher;

@end
