//
//  BlockViewController.m
//  IOSDemo
//
//  Created by yuxiang on 15/1/30.
//  Copyright (c) 2015年 yu. All rights reserved.
//

#import "BlockViewController.h"
#import "CallBackBlock.h"

@interface BlockViewController()

@property (weak, nonatomic) IBOutlet UIButton *blockAction;
- (IBAction)myBlockAction:(id)sender;
@property(nonatomic,strong)CallBackBlock* block;

@end

@implementation BlockViewController

-(void)viewWillAppear:(BOOL)animated
{
    
    NSLog(@"viewWillAppear");
    
    self.block = [[CallBackBlock alloc]initWithCallback:^(NSString *title) {
        NSLog(@"title:%@",title);
    }];
    
}

-(void)viewDidAppear:(BOOL)animated
{
    
    
}

- (IBAction)myBlockAction:(id)sender {
    float random = arc4random();
    [self.block playMovie:[@(random) stringValue]];
//    [self addCalculationResultHandler:^(float result) {
//        NSLog(@"%f",result);
//    }];
    
}

-(void)addCalculationResultHandler:(completionHandler)handler
{
   float random = arc4random();
    handler(random);
}

@end
