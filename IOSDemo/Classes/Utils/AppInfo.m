//
//  AppInfo.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/12.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "AppInfo.h"


@implementation AppInfo

+(NSString *)appVersion
{
    NSDictionary* infoDict =[[NSBundle mainBundle] infoDictionary];
    NSString* versionNum =[infoDict objectForKey:@"CFBundleVersion"];
    return versionNum;
}


+(NSString *)appName
{
    NSDictionary* infoDict =[[NSBundle mainBundle] infoDictionary];
    NSString*appName =[infoDict objectForKey:@"CFBundleDisplayName"];
    return appName;
}

//NSLog(@"uniqueIdentifier: %@", [[UIDevice currentDevice] uniqueIdentifier]);
//NSLog(@"name: %@", [[UIDevice currentDevice] name]);
//NSLog(@"systemName: %@", [[UIDevice currentDevice] systemName]);
//NSLog(@"systemVersion: %@", [[UIDevice currentDevice] systemVersion]);
//NSLog(@"model: %@", [[UIDevice currentDevice] model]);
//NSLog(@"localizedModel: %@", [[UIDevice currentDevice] localizedModel]);

+(NSString *)miei
{
    return nil;
}






@end
