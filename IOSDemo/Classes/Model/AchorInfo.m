//
//  AchorInfo.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/25.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "AchorInfo.h"
#import "ChoosyAchorInfo.h"

@implementation AchorInfo



+ (NSDictionary *)JSONKeyPathsByPropertyKey {
    return @{
             @"flag": @"flag", //将JSON字典里dt键对应的值，赋值给date属性
             @"searchKey": @"content.searchKey",
             @"page": @"content.page",
             @"itemTotal": @"content.itemTotal",//这个点是什么意思呢，表示将main键对应的子字典里，
             @"total_page_number": @"content.total_page_number",
             @"data": @"content.data",
             };
}

+(NSValueTransformer *)dataJSONTransformer
{
    return [NSValueTransformer mtl_JSONArrayTransformerWithModelClass:[ChoosyAchorInfo class]];
}


+ (NSValueTransformer *)dateJSONTransformer {
    return [MTLValueTransformer reversibleTransformerWithForwardBlock:^(NSNumber *dateNum) {
        return [NSDate dateWithTimeIntervalSince1970:dateNum.floatValue];
    } reverseBlock:^(NSDate *date) {
        return [NSString stringWithFormat:@"%f",[date timeIntervalSince1970]];
    }];
}

@end
