//
//  AchorInfo.h
//  IOSDemo
//
//  Created by yuxiang on 14/11/25.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "BaseModel.h"

@interface AchorInfo : BaseModel
@property (nonatomic, copy)     NSString    *flag;
@property (nonatomic, copy)     NSString    *searchKey;
@property (nonatomic, copy)     NSString    *page;
@property (nonatomic, copy)     NSString    *itemTotal;
@property (nonatomic, copy)     NSString    *total_page_number;
@property (nonatomic,strong)    NSArray     *data;
@end
