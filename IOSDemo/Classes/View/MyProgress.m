//
//  MyProgress.m
//  IOSDemo
//
//  Created by yuxiang on 15/1/28.
//  Copyright (c) 2015年 yu. All rights reserved.
//

#import "MyProgress.h"

@interface MyProgress()

@property(nonatomic,assign)float startAngle;
@property(nonatomic,assign)float endAngle;



@end


@implementation MyProgress

-(CAShapeLayer *)shapeLayer
{
    if (!_shapeLayer) {
        _shapeLayer = [CAShapeLayer new];
    }
    return _shapeLayer;
}


-(float)startAngle
{
    if(!_startAngle)
    {
        _startAngle = 0.0;
    }
    return _startAngle;
}

-(float)endAngle
{
    if(!_endAngle)
    {
        _endAngle = 0.0;
    }
    return _endAngle;
}


- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
    }
    return self;
}

-(void)updateProgress:(CGFloat)progress
{
    [CATransaction begin];
    [CATransaction setValue: (id)kCFBooleanTrue forKey:kCATransactionDisableActions];
    self.shapeLayer.strokeEnd = progress;
    [CATransaction commit];
}

-(void)layoutSubviews
{
    
    NSLog(@"layoutSubviews");
    [super layoutSubviews];
    if (self.startAngle==self.endAngle) {
        self.endAngle = self.startAngle + (M_PI *2 );
    }
    self.shapeLayer.path = self.shapeLayer.path == nil?[self layoutPath].CGPath:self.shapeLayer.path;
}

-(UIBezierPath *)layoutPath
{
    CGFloat halfWidth = self.frame.size.width/2.0;
//    return [UIBezierPath bezierPathWithArcCenter:CGPointMake(halfWidth, halfWidth) radius:halfWidth-self.shapeLayer.lineWidth startAngle:self.startAngle endAngle:self.endAngle clockwise:true];
    
    return [UIBezierPath bezierPathWithArcCenter:CGPointMake(160, 200) radius:self.frame.size.width/3.0 startAngle:M_PI * 0.72 endAngle:M_PI * 2.28 clockwise:true];
}

@end
