//
//  UtilViewController.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/13.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "UtilViewController.h"
#import "ExtendNSLog.h"
#import "TestDataModel.h"
#import "NetWorkUtil.h"
#import "AFNetworking.h"


@interface UtilViewController ()
- (IBAction)showNSLog:(id)sender;
- (IBAction)showExcept:(id)sender;
- (IBAction)parserJson:(id)sender;
- (IBAction)isNetWork:(id)sender;
- (IBAction)isFirstRun:(id)sender;
- (IBAction)testBlock:(id)sender;


- (IBAction)debug:(id)sender;



@end

@implementation UtilViewController

- (void)viewDidLoad {
    [super viewDidLoad];
   
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

- (IBAction)showNSLog:(id)sender {
    int result = 20;
    NSLog(@"Value of result : %d", result);
}

- (IBAction)showExcept:(id)sender {
    NSArray *arr = [NSArray arrayWithObjects:@"aaa", nil];
    NSLog(@"data:%@",arr[1]);
}

- (IBAction)parserJson:(id)sender {
    NSURL *url = [NSURL URLWithString:@"http://api.openweathermap.org/data/2.5/weather?lat=37.785834&lon=-122.406417&units=imperial"];
    [NSURLConnection sendAsynchronousRequest:[NSURLRequest requestWithURL:url]
                                       queue:[NSOperationQueue mainQueue]
                           completionHandler:^(NSURLResponse* response, NSData* data, NSError* connectionError){
                               if (!connectionError) {
                                   NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:data
                                                                                        options:NSJSONReadingMutableContainers
                                                                                          error:nil];
                                   //将JSON数据和Model的属性进行绑定
                                   TestDataModel *model = [MTLJSONAdapter modelOfClass:[TestDataModel class]
                                                                    fromJSONDictionary:dict
                                                                                 error:nil];
//                                   NSLog(@"%@",model);
                                   
                                   NSDictionary *jsonDictionary = [MTLJSONAdapter JSONDictionaryFromModel:model];

//                                   NSArray *keys = [jsonDictionary allKeys];// 所有key
//                                   for(int i=0;i<[keys count];i++){
//                                       NSString *key = [keys objectAtIndex:i];
//                                       NSLog(@"value:%@",[dict objectForKey:key]);
//                                   }
                                   
                                   for (NSString *key in dict) {
                                       NSLog(@"key: %@ value: %@", key, dict[key]);
                                   }
                                   
                               }
                           }];
    
}

- (IBAction)isNetWork:(id)sender {

//    [NetWorkUtil isOK];
    [[AFNetworkReachabilityManager sharedManager] setReachabilityStatusChangeBlock:^(AFNetworkReachabilityStatus status) {
        NSLog(@"Reachability: %@", AFStringFromNetworkReachabilityStatus(status));
    }];
}

- (IBAction)isFirstRun:(id)sender {
    
    // 以下这段代码，检查是否app的这个版本是否是第一次运行
    NSString *bundleVersion = [[NSBundle mainBundle] objectForInfoDictionaryKey:(NSString *)kCFBundleVersionKey];
    NSLog(@"bundleVersion:%@",bundleVersion);
    NSString *appFirstStartOfVersionKey = [NSString stringWithFormat:@"first_start_%@", bundleVersion];
    NSNumber *alreadyStartedOnVersion = [[NSUserDefaults standardUserDefaults] objectForKey:appFirstStartOfVersionKey];
    if(!alreadyStartedOnVersion || [alreadyStartedOnVersion boolValue] == NO) {
//        [self versionFirstStart];// app的bundleVersion这个版本第一次运行，你希望这时做点儿什么
        NSLog(@"app的bundleVersion这个版本第一次运行");
        [[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithBool:YES] forKey:appFirstStartOfVersionKey];
    }
}

- (IBAction)testBlock:(id)sender {
    int multiplier = 8;
    NSString * aa = @"ccc";

    int (^myBlock)(int) = ^(int num){
        
        return multiplier *num;
    };
    NSLog(@"%d",myBlock(56));
    
}

- (IBAction)debug:(id)sender {
    
    NSLog(@"we are going to break");
    NSMutableArray *cells = [[NSMutableArray alloc]init];

    NSLog(@"....",cells[0]);
    
}
@end
