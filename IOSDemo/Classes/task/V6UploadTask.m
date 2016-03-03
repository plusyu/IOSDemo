//
//  V6UploadTask.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/27.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "V6UploadTask.h"
#import "AFNetworking.h"

@interface V6UploadTask()
@property (nonatomic, strong) NSData *partialData;
@property (nonatomic, strong) NSURLSessionDownloadTask *task;
@property(nonatomic, strong)NSURLRequest *request;
@property(nonatomic,strong)NSURLSession *session;
@property(nonatomic,strong)UIImageView *imageView;
@end

@implementation V6UploadTask
@synthesize task;
@synthesize session;
@synthesize request;


-(id)init
{
    if (self = [super init]){
        NSURLSessionConfiguration *sessionConfig = [NSURLSessionConfiguration defaultSessionConfiguration];
        session = [NSURLSession sessionWithConfiguration:sessionConfig
                                                 delegate:self delegateQueue:nil];
    }
    return self;

}

-(void)execute:(NSString *)url view:(UIImageView *)progress
{
    self.imageView = progress;
    if (self.partialData) {
        task = [session downloadTaskWithResumeData:self.partialData];

    }else{
        NSLog(@"no partialData");
        NSURL *urls = [NSURL URLWithString:url];
        request = [NSURLRequest requestWithURL:urls];
        task = [session downloadTaskWithRequest:request];
    }
    [task resume];
}

#pragma mark 取消任务
-(void)cancel
{
    [task cancelByProducingResumeData:^(NSData *resumeData) {
        NSLog(@"cancelByProducingResumeData");
        self.partialData = resumeData;
    }];
    
}


#pragma mark - download delegate


-(void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didWriteData:(int64_t)bytesWritten totalBytesWritten:(int64_t)totalBytesWritten totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite
{
    //刷新进度条的delegate方法，同样的，获取数据，调用主线程刷新UI
    double currentProgress = totalBytesWritten/(double)totalBytesExpectedToWrite;
    NSLog(@"下载:%f",currentProgress);
    
}


-(void)URLSession:(NSURLSession *)session didReceiveChallenge:(NSURLAuthenticationChallenge *)challenge completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition, NSURLCredential *))completionHandler
{
    NSLog(@"didReceiveChallenge");
}

-(void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didResumeAtOffset:(int64_t)fileOffset expectedTotalBytes:(int64_t)expectedTotalBytes
{
    NSLog(@"didResumeAtOffset");
}

-(void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didFinishDownloadingToURL:(NSURL *)location
{
    NSLog(@"didFinishDownloadingToURL");
    dispatch_async(dispatch_get_main_queue(), ^{
        UIImage *image =[UIImage imageWithData:[NSData dataWithContentsOfURL:location]];
        self.imageView.image = image;
        self.imageView.contentMode = UIViewContentModeScaleAspectFit;
        self.imageView.hidden = NO;
    });
}

-(void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didCompleteWithError:(NSError *)error
{
    NSLog(@"didCompleteWithError:%@",error);

}




@end
