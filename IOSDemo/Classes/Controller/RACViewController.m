//
//  RACViewController.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/28.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "RACViewController.h"
#import <ReactiveCocoa/ReactiveCocoa.h>


@interface RACViewController()
@property (weak, nonatomic) IBOutlet UITableView *searchResultTable;
@property (weak, nonatomic) IBOutlet UITextField *searchTextField;
@property (weak, nonatomic) IBOutlet UIButton *searchSubmit;

@end


@implementation RACViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    NSLog(@"ssssss");
}


@end
