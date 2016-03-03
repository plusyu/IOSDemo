//
//  TestDataModel.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/24.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "TestDataModel.h"

@implementation TestDataModel

+ (NSDictionary *)JSONKeyPathsByPropertyKey {
    return @{
             @"date": @"dt", //将JSON字典里dt键对应的值，赋值给date属性
             @"locationName": @"name",
             @"humidity": @"main.humidity",
             @"temperature": @"main.temp",//这个点是什么意思呢，表示将main键对应的子字典里，
             @"tempHigh": @"main.temp_max",
             @"tempLow": @"main.temp_min",
             @"sunrise": @"sys.sunrise",
             @"sunset": @"sys.sunset",
             @"conditionDescription": @"weather.description",
             @"condition": @"weather.main",
             @"icon": @"weather.icon",
             @"windBearing": @"wind.deg",
             @"windSpeed": @"wind.speed",
             };
    }



+ (NSValueTransformer *)dateJSONTransformer {
    return [MTLValueTransformer reversibleTransformerWithForwardBlock:^(NSNumber *dateNum) {
        return [NSDate dateWithTimeIntervalSince1970:dateNum.floatValue];
    } reverseBlock:^(NSDate *date) {
        return [NSString stringWithFormat:@"%f",[date timeIntervalSince1970]];
    }];
}
@end
