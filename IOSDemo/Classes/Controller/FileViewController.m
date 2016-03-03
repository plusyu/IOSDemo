//
//  FileViewController.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/14.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "FileViewController.h"
#import "AFDownloadRequestOperation.h"
#import "AFNetworking.h"

@interface FileViewController ()
- (IBAction)download:(id)sender;
- (IBAction)stop:(id)sender;
@property (weak, nonatomic) IBOutlet UIButton *stopText;

@end

@implementation FileViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.navigationItem.title = @"文件";
    
    
    CGRect playRect = CGRectMake(self.view.bounds.size.width/2-100, 100, 169, 53);
    UIButton  *playBt = [[UIButton alloc]initWithFrame:playRect];
    [playBt setTitle:@"" forState:UIControlStateNormal];
    [playBt setTitle:@"" forState:UIControlStateSelected];
    [playBt setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    UIImage *greenImage = [UIImage imageNamed:@"1136-960-72.png"] ;
    [playBt setBackgroundImage:greenImage forState:UIControlStateNormal];
    [self.view addSubview:playBt];
    
    CGRect playRect1 = CGRectMake(self.view.bounds.size.width/2-100, 163, 263, 41);
    UIButton  *playBt1 = [[UIButton alloc]initWithFrame:playRect1];
    [playBt1 setTitle:@"" forState:UIControlStateNormal];
    [playBt1 setTitle:@"" forState:UIControlStateSelected];
    [playBt1 setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    UIImage *greenImage1 = [UIImage imageNamed:@"1136-960-144.png"] ;
    [playBt1 setBackgroundImage:greenImage1 forState:UIControlStateNormal];
    [self.view addSubview:playBt1];
    
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (IBAction)download:(id)sender {
    NSString *  imageUrl =@"http://vr0.6.cn/mobile/test/nba.gif";
//    NSFileManager *fileManager = [NSFileManager defaultManager];
//    NSArray *urls = [fileManager URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask];
//    NSURL *documentsDirectory = urls[0];
//    NSURL *path = [documentsDirectory URLByAppendingPathComponent:[location lastPathComponent]];
    NSString *target = @" file:///private/var/mobile/Containers/Data/Application/D8B4FF57-D99A-458B-883C-47896FB122CA/tmp/aaa.gif";
    AFDownloadRequestOperation *operation = [[AFDownloadRequestOperation alloc]initWithRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:imageUrl]] targetPath:target shouldResume:YES];


    [operation setProgressiveDownloadProgressBlock:^(AFDownloadRequestOperation *operation, NSInteger bytesRead, long long totalBytesRead, long long totalBytesExpected, long long totalBytesReadForFile, long long totalBytesExpectedToReadForFile) {
        NSLog(@"operation");
    }];
}

- (IBAction)stop:(id)sender {
}
@end
