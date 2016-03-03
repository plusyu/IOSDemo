//
//  LighterTableViewController.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/21.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "LighterTableViewController.h"
#import "LighterAdapter.h"
#import "LighterCell.h"
#import "Lighter.h"
#import "LighterCell+Config.h"

@interface LighterTableViewController ()
@property (nonatomic, strong) LighterAdapter* adapter;
@end

@implementation LighterTableViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self stepTableCell];
    
}

-(void)stepTableCell
{
    NSLog(@"stepTableCell");
    
    TableViewCellConfigureBlock cellBlock = ^(LighterCell *cell,Lighter *lighter){
        [cell configureForLighterCell:lighter];
    };
    
    self.adapter = [[LighterAdapter alloc]initWithTable];
    self.tableView.dataSource = self.adapter;
    self.tableView.delegate = self.adapter;
    
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
