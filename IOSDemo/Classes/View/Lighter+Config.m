//
//  Lighter+Config.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/21.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "Lighter+Config.h"
#import "LighterCell.h"

@implementation Lighter (Config)

- (void)configureForLighter:(Lighter *)lighter
{
    self.photoTitleLabel.text = photo.name;
    self.photoDateLabel.text = [self.dateFormatter stringFromDate:photo.creationDate];
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
