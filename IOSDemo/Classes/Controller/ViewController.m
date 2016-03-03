//
//  ViewController.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/12.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "ViewController.h"
#import "UINavigationBar+Awesome.h"


@interface ViewController ()


@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    self.navigationItem.title = @"测试标题";
    UIColor * color = [UIColor colorWithRed:0/255.0 green:175/255.0 blue:240/255.0 alpha:1];
    CGFloat alpha = 0.5f;
    [self.navigationController.navigationBar lt_setBackgroundColor:[color colorWithAlphaComponent:alpha]];
        double delayInSeconds = 2;
        dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
        dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
              [self playMusic];
        });
}

-(void)playMusic
{
    NSLog(@"playMusic");
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];

}


@end
