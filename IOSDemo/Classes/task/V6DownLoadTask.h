//
//  V6DownLoadTask.h
//  IOSDemo
//
//  Created by yuxiang on 14/11/27.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "AFNetworking.h"


@interface V6DownLoadTask : NSObject
-(void)execute:(NSString *)url view:(UIView *)progress;
-(void)cancel;
@end
