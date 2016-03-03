//
//  DataBaseViewController.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/14.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "DataBaseViewController.h"
#import "YTKKeyValueStore.h"

#define kDBName  @"database.sqlite"


@interface DataBaseViewController ()
@property(nonatomic,strong) YTKKeyValueStore *store;

- (IBAction)add:(id)sender;
- (IBAction)del:(id)sender;
- (IBAction)query:(id)sender;

- (IBAction)update:(id)sender;
@end



@implementation DataBaseViewController
@synthesize store = _store;

-(YTKKeyValueStore *)store
{
    if (!_store) {
         _store = [[YTKKeyValueStore alloc] init];
    }
    return _store;
}

- (IBAction)add:(id)sender {
    
//    - (void)putString:(NSString *)string withId:(NSString *)stringId intoTable:(NSString *)tableName;
//    - (void)putNumber:(NSNumber *)number withId:(NSString *)numberId intoTable:(NSString *)tableName;
//    - (void)putObject:(id)object withId:(NSString *)objectId intoTable:(NSString *)tableName;
//    createTableWithName
    NSString *tableName = @"user";
    [self.store createTableWithName:tableName];
    
    [self.store putString:@"one" withId:@"1" intoTable:tableName];
    
}

- (IBAction)del:(id)sender {
    [self.store deleteObjectById:@"1" fromTable:@"user"];
}

- (IBAction)query:(id)sender {
    NSArray *arr =   [self.store getAllItemsFromTable:@"user"];
    for (NSArray *a in arr) {
        NSLog(@"NSArray :%@",a);
    }
    
}

- (IBAction)update:(id)sender {

}

- (void)viewDidLoad {
    [super viewDidLoad];
     self.navigationItem.title = @"数据库操作";


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

@end
