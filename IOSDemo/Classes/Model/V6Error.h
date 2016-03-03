//
//  V6Error.h
//  IOSDemo
//
//  Created by yuxiang on 14/11/25.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "BaseModel.h"

@interface V6Error : BaseModel

@property(nonatomic,copy)NSString *flag;
@property(nonatomic,copy)NSString *content;

@end
