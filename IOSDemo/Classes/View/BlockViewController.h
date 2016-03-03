//
//  BlockViewController.h
//  IOSDemo
//
//  Created by yuxiang on 15/1/30.
//  Copyright (c) 2015年 yu. All rights reserved.
//

#import "ViewController.h"
typedef void(^SuccessBlock)(id obj);
typedef void (^completionHandler)(float result);

@interface BlockViewController : ViewController

-(void) addCalculationResultHandler:(completionHandler)handler;

@end
