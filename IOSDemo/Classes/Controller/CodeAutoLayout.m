//
//  CodeAutoLayout.m
//  IOSDemo
//
//  Created by yuxiang on 14/12/30.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "CodeAutoLayout.h"

@implementation CodeAutoLayout

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    if ([self.view respondsToSelector:@selector(addConstraints:)])
    {
        [self newStyle];
    } else
    {
        [self oldStyle];
    }
    
//    [self addBase];
}

-(void)oldStyle
{}

-(void)newStyle
{
    UIView *myView = [[UIView alloc]init];
    myView.backgroundColor = [UIColor whiteColor];
    
    UIView *redView = [[UIView alloc]init];
    redView.backgroundColor = [UIColor redColor];
    
    UIView *blueView = [[UIView alloc]init];
    blueView.backgroundColor = [UIColor blueColor];
    
    [myView addSubview:redView];
    [myView addSubview:blueView];
    
    [myView setTranslatesAutoresizingMaskIntoConstraints:NO];
    [redView setTranslatesAutoresizingMaskIntoConstraints:NO];
    [blueView setTranslatesAutoresizingMaskIntoConstraints:NO];
    
    NSMutableArray *tmpConstraints = [NSMutableArray array];
    
    
/*
 
 CGFloat const kLayoutPadding = 10.0f;
 NSDictionary *metrics = @{@"padding": @(kLayoutPadding)};
 NSMutableArray *updateConstraits = [NSMutableArray array];

 [updateConstraits addObjectsFromArray:[NSLayoutConstraint constraintsWithVisualFormat:@"|[photoImageView]-(padding)-[avatarImageView]-(padding)-[nameLabel]" options:0 metrics:metrics views:views]];
 
 [updateConstraits addObjectsFromArray:[NSLayoutConstraint constraintsWithVisualFormat:@"[avatarImageView]-(padding)-[timeLabel]" options:0 metrics:metrics views:views]];
 
 [updateConstraits addObjectsFromArray:[NSLayoutConstraint constraintsWithVisualFormat:@"[photoImageView]-(padding)-[descriptionLabel]" options:0 metrics:metrics views:views]];
 
 [updateConstraits addObjectsFromArray:[NSLayoutConstraint constraintsWithVisualFormat:@"V:[topLayoutGuide][photoImageView]|" options:0 metrics:metrics views:views]];
 
 [updateConstraits addObjectsFromArray:[NSLayoutConstraint constraintsWithVisualFormat:@"V:[topLayoutGuide]-(padding)-[avatarImageView]-(padding)-[descriptionLabel]" options:0 metrics:metrics views:views]];
 
 [updateConstraits addObjectsFromArray:[NSLayoutConstraint constraintsWithVisualFormat:@"V:[topLayoutGuide]-(padding)-[nameLabel]-(padding)-[timeLabel]" options:0 metrics:metrics views:views]];
 
 self.descriptionLabel.preferredMaxLayoutWidth = self.view.bounds.size.width - self.view.bounds.size.height + self.topLayoutGuide.length - 2 * kLayoutPadding;
 
 
 
 [updateConstraits addObject:[NSLayoutConstraint constraintWithItem:self.avatarImageView attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:self.avatarImageView attribute:NSLayoutAttributeWidth multiplier:1.0 constant:0.0]];
 [updateConstraits addObject:[NSLayoutConstraint constraintWithItem:self.photoImageView attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:self.photoImageView attribute:NSLayoutAttributeWidth multiplier:1.0 constant:0.0]];
 
 
 
 
 
 
 NSArray *constraintsStrings = @[@"H:|[_redView]|",
 @"H:|[_purpleView]|",
 [NSString stringWithFormat:@"V:|[_redView(>=80@1000)][_purpleView(==%@@%@)]|",
 @(floorf([self.slider value])), @(UILayoutPriorityDefaultHigh)],
 ];
 
 
    NSLayoutFormatDirectionLeadingToTrailing
    意思就是默认的排版方式，就是从左往右看，从上往下看。
    于是就引出了：
    NSLayoutFormatDirectionLeftToRight  与 NSLayoutFormatDirectionRightToLeft
    也不难理解，前一个是从屏幕左沿布局开始算起，后一个是从屏幕右沿开始算起。
*/
    //    水平方向布局
    [tmpConstraints addObjectsFromArray:
    [NSLayoutConstraint constraintsWithVisualFormat:@"|-50-[redView(==100)]-30-[blueView(==100)]"
                                             options:NSLayoutFormatDirectionLeadingToTrailing
                                             metrics:nil
                                               views:NSDictionaryOfVariableBindings(redView,blueView)]];
    //    垂直方向布局
    [tmpConstraints addObjectsFromArray:
    [NSLayoutConstraint constraintsWithVisualFormat:@"V:|-144-[redView(==30)]"
                                             options:NSLayoutFormatDirectionLeadingToTrailing
                                             metrics:nil
                                               views:NSDictionaryOfVariableBindings(redView)]];
    [tmpConstraints addObjectsFromArray:
    [NSLayoutConstraint constraintsWithVisualFormat:@"V:|-144-[blueView(==redView)]"
                                             options:NSLayoutFormatDirectionLeadingToTrailing
                                             metrics:nil
                                               views:NSDictionaryOfVariableBindings(blueView,redView)]];
    [myView addConstraints:tmpConstraints];
    
    //    按钮布局
    UIButton *button = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [button setTitle:@"我是测试的小酱油"forState:UIControlStateNormal];
    [button setTranslatesAutoresizingMaskIntoConstraints:NO];
    [button setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [myView addSubview:button];
    
    NSLayoutConstraint *lc = [NSLayoutConstraint
                              constraintWithItem:button
                              attribute:NSLayoutAttributeBottom
                              relatedBy:NSLayoutRelationEqual
                              toItem:myView
                              attribute:NSLayoutAttributeBottom
                              multiplier:1.0
                              constant:200];
    [myView addConstraint:lc];
    
    lc = [NSLayoutConstraint
          constraintWithItem:button
          attribute:NSLayoutAttributeCenterX
          relatedBy:NSLayoutRelationEqual
          toItem:myView
          attribute:NSLayoutAttributeCenterX
          multiplier:1.0
          constant:150];
    [myView addConstraint:lc];

    self.view = myView;
    
//    NSLayoutConstraint *constraintTop = [NSLayoutConstraint constraintWithItem:redView attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeTop multiplier:1.0 constant:100];
//    [self.view addConstraint:constraintTop];

}

-(void)addBase{
    
    UIButton *btnTest = [UIButton buttonWithType:UIButtonTypeCustom];//不需要去刻意指定x,y的坐标，可以用CGRectZero
    btnTest.backgroundColor = [UIColor redColor];
    btnTest.layer.borderColor = [UIColor blueColor].CGColor;
    btnTest.layer.borderWidth = 1.0;
    [btnTest setTitle:@"我只是测试的按钮" forState:UIControlStateNormal];
    [self.view addSubview:btnTest];
    
    [btnTest setTranslatesAutoresizingMaskIntoConstraints:NO];//将使用AutoLayout的方式布局
    //btnTest顶部相对于self.view的顶部距离为100
    NSLayoutConstraint *constraintTop = [NSLayoutConstraint constraintWithItem:btnTest attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeTop multiplier:1.0 constant:100];
    //btnTest左侧相对于self.view的左侧距离为100
    NSLayoutConstraint *constraintLeft = [NSLayoutConstraint constraintWithItem:btnTest  attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeLeft multiplier:1.0 constant:100];
    //btnTest右侧相对于self.view的右侧距离为100
    NSLayoutConstraint *constraintRight = [NSLayoutConstraint constraintWithItem:self.view  attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:btnTest attribute:NSLayoutAttributeRight multiplier:1.0 constant:100];
    //btnTest底部相对于self.view的底部距离为100
    NSLayoutConstraint *constraintBottom = [NSLayoutConstraint constraintWithItem:self.view attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:btnTest attribute:NSLayoutAttributeBottom multiplier:1.0 constant:100];
    //水平居中
    NSLayoutConstraint *constraintXCenter = [NSLayoutConstraint constraintWithItem:btnTest attribute:NSLayoutAttributeCenterX relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeCenterX multiplier:1.0 constant:00.0f];
    //垂直居中
    NSLayoutConstraint *constraintYCenter = [NSLayoutConstraint constraintWithItem:btnTest attribute:NSLayoutAttributeCenterY relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeCenterY multiplier:1.0 constant:00.0f];
    
    //将约束添加到父视图中
    [self.view addConstraint:constraintTop];
    [self.view addConstraint:constraintLeft];
    [self.view addConstraint:constraintRight];
    [self.view addConstraint:constraintBottom];
    [self.view addConstraint:constraintXCenter];
    [self.view addConstraint:constraintYCenter];
    
}


@end
