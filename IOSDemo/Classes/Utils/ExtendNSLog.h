//
//  ExtendNSLog.h
//  IOSDemo
//
//  Created by yuxiang on 14/11/13.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import <Foundation/Foundation.h>


#ifdef DEBUG
#define NSLog(args...) ExtendNSLogs(__FILE__,__LINE__,__PRETTY_FUNCTION__,args);
#else
#define NSLog(x...)
#endif



@interface ExtendNSLog : NSObject
void ExtendNSLogs(const char *file, int lineNumber, const char *functionName, NSString *format, ...);
@end
