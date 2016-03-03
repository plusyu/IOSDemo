//
//  GreenLineLabel.m
//  IOSDemo
//
//  Created by yuxiang on 14/12/3.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "GreenLineLabel.h"

@implementation GreenLineLabel
{
    CALayer *_maskLayer;
}


-(instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        _maskLayer = [CALayer layer];
        _maskLayer.backgroundColor = [[UIColor yellowColor] CGColor];    // Any color, only alpha channel matters
        _maskLayer.anchorPoint = CGPointZero;
        _maskLayer.frame = CGRectOffset(self.frame, -CGRectGetWidth(self.frame), 0);
        self.layer.mask = _maskLayer;
        self.backgroundColor = [UIColor clearColor];
    }
    return self;
}

-(void)startAnimation:(NSMutableArray *)keyTimes values:(NSMutableArray *)values duration:(CGFloat)duration{
    // Assume we calculated keyTimes and values
    CAKeyframeAnimation *animation = [CAKeyframeAnimation animationWithKeyPath:@"position"];
    animation.keyTimes = keyTimes;
    animation.values = values;
    animation.duration = duration;
    animation.calculationMode = kCAAnimationLinear;
    animation.fillMode = kCAFillModeForwards;
    animation.removedOnCompletion = NO;
    [_maskLayer addAnimation:animation forKey:@"position"];
}

@end
