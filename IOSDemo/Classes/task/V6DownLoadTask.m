//
//  V6DownLoadTask.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/27.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "V6DownLoadTask.h"
#import "AFDownloadRequestOperation.h"



@interface V6DownLoadTask()

@end

@implementation V6DownLoadTask


-(id)init
{
    if (self = [super init]){
    
    }
    return self;
}



-(void)execute:(NSString *)url view:(UIView *)progress
{
    NSString *paths = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSURL *downloadUrl = [NSURL URLWithString:url];
    NSString *downloadPath = [paths stringByAppendingPathComponent:[url lastPathComponent]];

    NSURLRequest *request = [NSURLRequest requestWithURL:downloadUrl];
    AFDownloadRequestOperation *operation = [[AFDownloadRequestOperation alloc] initWithRequest:request targetPath:downloadPath shouldResume:YES];
    
    // done saving!
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSLog(@"Done downloading");
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        NSLog(@"Error: %ld", (long)[error code]);
    }];
    // set the progress
    [operation setProgressiveDownloadProgressBlock:^(AFDownloadRequestOperation *operation, NSInteger bytesRead, long long totalBytesRead, long long totalBytesExpected, long long totalBytesReadForFile, long long totalBytesExpectedToReadForFile) {
        float progress = ((float)totalBytesReadForFile) / totalBytesExpectedToReadForFile;
//        [progressBar setProgress:progress];
        NSLog(@"progress:%f",progress);
    }];
    [operation start];
}


-(void)cancel
{
}


//获取已下载的文件大小
- (unsigned long long)fileSizeForPath:(NSString *)path {
    signed long long fileSize = 0;
    NSFileManager *fileManager = [NSFileManager new]; // default is not thread safe
    if ([fileManager fileExistsAtPath:path]) {
        NSError *error = nil;
        NSDictionary *fileDict = [fileManager attributesOfItemAtPath:path error:&error];
        if (!error && fileDict) {
            fileSize = [fileDict fileSize];
        }
    }
    return fileSize;
}
//开始下载
- (void)startDownload:(NSString *)downloadUrl {
//    NSString *downloadUrl = @"http://www.xxx.com/xxx.zip";
    NSString *cacheDirectory = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    
    NSURL *url = [NSURL URLWithString:downloadUrl];
    NSString *downloadPath = [cacheDirectory stringByAppendingPathComponent:[url lastPathComponent]];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    //检查文件是否已经下载了一部分
    unsigned long long downloadedBytes = 0;
    if ([[NSFileManager defaultManager] fileExistsAtPath:downloadPath]) {
        //获取已下载的文件长度
        downloadedBytes = [self fileSizeForPath:downloadPath];
        if (downloadedBytes > 0) {
            NSMutableURLRequest *mutableURLRequest = [request mutableCopy];
            NSString *requestRange = [NSString stringWithFormat:@"bytes=%llu-", downloadedBytes];
            [mutableURLRequest setValue:requestRange forHTTPHeaderField:@"Range"];
            request = mutableURLRequest;
            NSLog(@"requestRange");
        }
    }
    //不使用缓存，避免断点续传出现问题
    [[NSURLCache sharedURLCache] removeCachedResponseForRequest:request];
    //下载请求
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    //下载路径
    operation.outputStream = [NSOutputStream outputStreamToFileAtPath:downloadPath append:YES];
    //下载进度回调
    [operation setDownloadProgressBlock:^(NSUInteger bytesRead, long long totalBytesRead, long long totalBytesExpectedToRead) {
        //下载进度
        float progress = ((float)totalBytesRead + downloadedBytes) / (totalBytesExpectedToRead + downloadedBytes);
        NSLog(@"progress:%f",progress);
    }];
    //成功和失败回调
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSLog(@"成功");
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
         NSLog(@"failure:%@",error);
    }];
    [operation start];
}


@end
