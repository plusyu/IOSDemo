//
//  ViewB.m
//  IOSDemo
//
//  Created by yuxiang on 15/3/20.
//  Copyright (c) 2015年 yu. All rights reserved.
//

#import "ViewB.h"

@interface ViewB()

@property(nonatomic,strong)UIButton *buttonB;

@end

@implementation ViewB


- (instancetype)init
{
    self = [super init];
    if (self) {
            [self setupUI];
            [self addConstraints];
    }
    return self;
}


-(void)setupUI
{
    self.buttonB = [[UIButton alloc]init];
    [self addSubview:self.buttonB];
    [self.buttonB setTitle:@"我是buttonB" forState:UIControlStateNormal];
    [self.buttonB setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [self.buttonB setBackgroundColor:[UIColor blueColor]];
    
    [self.buttonB addTarget:self action:@selector(bAction:) forControlEvents:UIControlEventTouchUpInside];
}

-(void)addConstraints
{
    
    NSDictionary *views = @{@"buttonB":self.buttonB};
    
    
    NSDictionary *metrics = @{@"width": @(50)};
    [self.buttonB setTranslatesAutoresizingMaskIntoConstraints:NO];
    [self addConstraints:[NSLayoutConstraint
                                constraintsWithVisualFormat:@"H:|-0-[buttonB]-0-|" options:NSLayoutFormatDirectionLeadingToTrailing metrics:metrics views:views]];
    
    [self addConstraints:[NSLayoutConstraint
                                constraintsWithVisualFormat:@"V:|-200-[buttonB(==100)]" options:NSLayoutFormatDirectionLeadingToTrailing metrics:metrics views:views]];
}


-(void)bAction:(id)sender
{
    NSLog(@"你点点点了buttonB");
}
- (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent *)event
{
    // 本View不响应用户事件
    return NO;
}

#pragma mark - touches
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSLog(@"B - touchesBeagan..");
    
    // 把事件传递下去给父View或包含他的ViewController
    [self.nextResponder touchesBegan:touches withEvent:event];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSLog(@"B - touchesCancelled..");
    // 把事件传递下去给父View或包含他的ViewController
    [self.nextResponder touchesBegan:touches withEvent:event];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSLog(@"B - touchesEnded..");
    // 把事件传递下去给父View或包含他的ViewController
    [self.nextResponder touchesBegan:touches withEvent:event];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSLog(@"B - touchesMoved..");
    // 把事件传递下去给父View或包含他的ViewController
    [self.nextResponder touchesBegan:touches withEvent:event];
    
}


@end
