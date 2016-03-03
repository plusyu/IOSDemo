//
//  BaseModel.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/24.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "BaseModel.h"

@implementation BaseModel


+(NSDictionary *)JSONKeyPathsByPropertyKey
{
    return @{};
}



//+ (NSValueTransformer *)dateJSONTransformer {
//    return [MTLValueTransformer reversibleTransformerWithForwardBlock:^(NSNumber *dateNum) {
//        return [NSDate dateWithTimeIntervalSince1970:dateNum.floatValue];
//    } reverseBlock:^(NSDate *date) {
//        return [NSString stringWithFormat:@"%f",[date timeIntervalSince1970]];
//    }];
//}

+ (NSValueTransformer *)dateUpdatedJSONTransformer
{
    return [MTLValueTransformer reversibleTransformerWithForwardBlock:^(NSString *str) {
        NSDate *date = [self.dateFormatter dateFromString:str];
        return date;
    } reverseBlock:^(NSDate *date) {
        return [self.dateFormatter stringFromDate:date];
    }];
}


+ (NSDateFormatter *)dateFormatter
{
    static NSDateFormatter *_formatter;
    
    if (!_formatter) {
        _formatter = [NSDateFormatter new];
        _formatter.dateFormat = @"yyyy-MM-dd HH:mm:ss";
    }
    return _formatter;
}
@end
