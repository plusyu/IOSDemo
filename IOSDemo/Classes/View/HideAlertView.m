//
//  HideAlertView.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/13.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "HideAlertView.h"

@implementation HideAlertView

- (id)init
{
    self = [super init];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        
        UIActivityIndicatorView *activity = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
        activity.frame = CGRectMake(180, 15, 80, 80);
        [activity startAnimating];
        [activity sizeToFit];
        [self addSubview:activity];
        
    }
    return self;
}


- (void)layoutSubviews {

    [super layoutSubviews];
    for (id obj in self.subviews) {
        if ([obj isKindOfClass:[UILabel class]]) {
            UILabel *label = obj;
            label.frame = CGRectMake(92, 15, 100, 23);
            label.textAlignment = UITextAlignmentLeft;
        }
    }
}

- (void)drawRect:(CGRect)rect {
    [super drawRect:rect];
}

- (void)loaded {
    for (id obj in self.subviews) {
        if ([obj isKindOfClass:[UILabel class]]) {
            UILabel *label = obj;
            label.text = @"加载完毕";
            label.textAlignment = UIBaselineAdjustmentAlignCenters;
        }
        else if ([obj isKindOfClass:[UIActivityIndicatorView class]]) {
            [obj stopAnimating];
        }
    }
}


@end
