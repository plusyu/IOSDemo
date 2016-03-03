//
//  LighterAdapter.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/21.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "LighterAdapter.h"
#import "LighterCell.h"

@interface LighterAdapter()
@property (nonatomic, strong) NSMutableArray *items;
@property (nonatomic, copy) NSString *cellIdentifier;
@property (nonatomic, copy) TableViewCellConfigureBlock configureCellBlock;
@end

@implementation LighterAdapter
@synthesize items = _items;




- (id)initWithItems:(NSArray *)anItems
     cellIdentifier:(NSString *)aCellIdentifier
 configureCellBlock:(TableViewCellConfigureBlock)aConfigureCellBlock
{
    self = [super init];
    if (self) {
        NSLog(@"initWithItems");
        self.items = anItems;
        self.cellIdentifier = aCellIdentifier;
        self.configureCellBlock = [aConfigureCellBlock copy];
    }
    return self;
}

- (id)itemAtIndexPath:(NSIndexPath *)indexPath
{
    NSLog(@"indexPath:%d",indexPath.row);
    return self.items[(NSUInteger) indexPath.row];
}

-(id)initWithTable
{
    self = [super init];
    if (self) {
     self.items =  [[NSMutableArray alloc] init];
        for (int i=0; i<2150 ;i++) {
          NSString *item =  [NSString stringWithFormat: @"aaaaaaaaaaaaaaaaaaaa%d", i];
            [self.items addObject:item];
        }
        
    }
    return self;
    
}


#pragma mark - UITableViewDataSource

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    NSInteger count =   self.items.count;
    NSLog(@"count:%d",count);
    return count;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *identifier = @"LighterCell";

    LighterCell *cell = [tableView dequeueReusableCellWithIdentifier:identifier];
    cell.contentMode = UIViewContentModeScaleAspectFit;
    
    if (cell == nil) {
        NSLog(@"创建");
        cell = [[LighterCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identifier];
    }else{
        NSLog(@"不创建");
    }
    cell.title.text = self.items[indexPath.row];
    cell.detail.text = self.items[indexPath.row];
    
    CGRect titlRect = CGRectMake(0, 0, 10, 10);
    cell.title.bounds =titlRect;
    cell.title.frame =titlRect;


    
    
    return cell;
}


@end
