//
//  LighterAdapter.h
//  IOSDemo
//
//  Created by yuxiang on 14/11/21.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


typedef void (^TableViewCellConfigureBlock)(id cell, id item);
@interface LighterAdapter : NSObject<UITableViewDataSource,UITableViewDelegate>

- (id)initWithItems:(NSArray *)anItems
     cellIdentifier:(NSString *)aCellIdentifier
 configureCellBlock:(TableViewCellConfigureBlock)aConfigureCellBlock;

- (id)itemAtIndexPath:(NSIndexPath *)indexPath;

-(id)initWithTable;
@end
