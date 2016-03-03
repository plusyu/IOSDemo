//
//  TestViewController.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/20.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "AutoLayoutController.h"

@interface AutoLayoutController ()

- (IBAction)code:(id)sender;


@end

@implementation AutoLayoutController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
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

- (IBAction)buttonTapped:(id)sender {
    if ([[sender titleForState:UIControlStateNormal] isEqualToString:@"X"]) {
        [sender setTitle:@"A very long title for this button"
                forState:UIControlStateNormal];
    } else {
        [sender setTitle:@"X" forState:UIControlStateNormal];
    }
    
}
- (IBAction)buttonTap:(id)sender {
}
- (IBAction)code:(id)sender {
    
    
    
}
@end
