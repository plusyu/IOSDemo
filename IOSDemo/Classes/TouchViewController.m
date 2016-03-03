//
//  TouchViewController.m
//  IOSDemo
//
//  Created by yuxiang on 15/3/20.
//  Copyright (c) 2015年 yu. All rights reserved.
//http://www.cnblogs.com/Quains/p/3369132.html

#import "TouchViewController.h"
#import "ViewA.h"

@interface TouchViewController ()

@property(nonatomic,strong)ViewA* viewA;



@end

@implementation TouchViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self setupUI];
    [self addConstraints];
    
}

-(void)addConstraints
{
    [self.viewA setTranslatesAutoresizingMaskIntoConstraints:NO];

    
    
    NSDictionary *views = @{@"viewA": self.viewA};


    NSDictionary *metrics = @{@"width": @(50)};
    

    
    [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-0-[viewA]-0-|" options:NSLayoutFormatDirectionLeadingToTrailing metrics:metrics views:views]];
    
     [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-0-[viewA]-0-|" options:NSLayoutFormatDirectionLeadingToTrailing metrics:metrics views:views]];
    
    
}

-(void)setupUI
{
    self.viewA  = [[ViewA alloc]init];
    [self.view addSubview:self.viewA];
    [self.viewA setBackgroundColor:[UIColor whiteColor]];
    
 
 
}

-(void)aAction:(id)sender
{
    NSLog(@"你点击了buttonA");
}

-(void)bAction:(id)sender
{
     NSLog(@"你点点点了buttonB");
}




- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}


@end
