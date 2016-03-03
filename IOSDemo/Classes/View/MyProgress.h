//
//  MyProgress.h
//  IOSDemo
//
//  Created by yuxiang on 15/1/28.
//  Copyright (c) 2015年 yu. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MyProgress : UIView
@property(nonatomic,strong)CAShapeLayer * shapeLayer;

-(void)updateProgress:(CGFloat)progress;
@end
