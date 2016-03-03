//
//  ProgressView.m
//  IOSDemo
//
//  Created by yuxiang on 15/1/29.
//  Copyright (c) 2015年 yu. All rights reserved.
//

#import "ProgressView.h"
#import "MyProgress.h"

#define UIColorFromRGB(rgbValue) \
[UIColor colorWithRed:((float)((rgbValue & 0xFF0000) >> 16))/255.0 \
green:((float)((rgbValue & 0x00FF00) >>  8))/255.0 \
blue:((float)((rgbValue & 0x0000FF) >>  0))/255.0 \
alpha:1.0]


@interface ProgressView()

@property(nonatomic,strong)MyProgress *progressView;
@property(nonatomic,strong)CAGradientLayer * gradientLayer;

@end


@implementation ProgressView



-(CAGradientLayer * ) gradientLayer
{
    if (!_gradientLayer) {
        _gradientLayer = [CAGradientLayer new];

    }
    return _gradientLayer;
}


-(MyProgress *)progressView
{
    if(!_progressView)
    {
        _progressView = [[MyProgress alloc]initWithFrame:self.bounds];
    }
    return _progressView;
}


- (id)initWithFrame:(CGRect)frame
{
    if (self =[super initWithFrame:frame]) {
        // 初始化代码
        [self setup];
    }
    return self;
}

-(void)setup
{
    self.progressView.shapeLayer.lineWidth = 2;
//    self.progressView.shapeLayer.fillColor = [UIColor clearColor].CGColor;
    self.gradientLayer.frame = self.progressView.frame;
    self.gradientLayer.startPoint = CGPointMake(0, 0.5);
    self.gradientLayer.endPoint = CGPointMake(1, 0.5);
    self.gradientLayer.mask = self.progressView.shapeLayer;
    self.gradientLayer.colors = [NSArray arrayWithObjects:(id)[[UIColor yellowColor] CGColor], (id)[[UIColor blueColor] CGColor], nil];;
    self.progressView.shapeLayer.strokeColor = self.tintColor.CGColor;
    [self.layer addSublayer:self.gradientLayer];
    [self.progressView updateProgress:10];
}




















@end
