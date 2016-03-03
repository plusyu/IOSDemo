//
//  LighterCell+Config.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/21.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "LighterCell+Config.h"
#import "Lighter.h"

@implementation LighterCell (Config)


- (void)configureForLighterCell:(Lighter *)lighter
{
    self.title.text = lighter.name;
    self.detail.text = @"detailName";
}

- (NSDateFormatter *)dateFormatter
{
    static NSDateFormatter *dateFormatter;
    if (!dateFormatter) {
        dateFormatter = [[NSDateFormatter alloc] init];
        dateFormatter.timeStyle = NSDateFormatterMediumStyle;
        dateFormatter.dateStyle = NSDateFormatterMediumStyle;
    }
    return dateFormatter;
}

@end
