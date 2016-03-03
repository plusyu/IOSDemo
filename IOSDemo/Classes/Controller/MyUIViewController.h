//
//  MyUIViewController.h
//  IOSDemo
//
//  Created by yuxiang on 14/11/13.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AFNetworking.h"

@interface MyUIViewController : UIViewController<UITextFieldDelegate,NSURLSessionDownloadDelegate>
@property (atomic, strong) NSData *partialData;
@end
