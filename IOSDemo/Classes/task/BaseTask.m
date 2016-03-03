//
//  BaseTask.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/24.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "BaseTask.h"
#import "BaseModel.h"
#import "V6ResponseError.h"
#import "SVProgressHUD.h"




@implementation BaseTask

-(void)showPrepareWithMsg:(NSString *)msg
{
    [SVProgressHUD showWithStatus:msg];
}
-(void)dismissPrepare
{
    [SVProgressHUD dismiss];
}

-(id)handlerJson:(id)json model:(Class)baseClass
{
    NSDictionary *dic =(NSDictionary *) json;
    NSString  *flag = [dic objectForKey:@"flag"];
    if ([@"001" isEqualToString:flag]) {
        BaseModel *base = [MTLJSONAdapter modelOfClass:[baseClass class]
                                     fromJSONDictionary:json
                                                  error:nil];
        return base;
    }else
    {
        NSInteger code = [flag intValue];
        NSString  *content = [dic objectForKey:@"content"];
        
        V6ResponseError *newError = [V6ResponseError errorWithDomain:content code:code userInfo:nil];
        return newError;
    }

}
@end
