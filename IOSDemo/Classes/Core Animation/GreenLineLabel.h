//
//  GreenLineLabel.h
//  IOSDemo
//
//  Created by yuxiang on 14/12/3.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface GreenLineLabel : UILabel


//NSMutableArray *keyTimes;
//NSMutableArray *values;
//CGFloat duration;
-(void)startAnimation:(NSMutableArray *)keyTime values:(NSMutableArray *)values duration:(CGFloat)duration;

@end
