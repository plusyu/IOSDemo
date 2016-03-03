//
//  ViewA.m
//  IOSDemo
//
//  Created by yuxiang on 15/3/20.
//  Copyright (c) 2015年 yu. All rights reserved.
//

#import "ViewA.h"
#import "ViewB.h"


@interface ViewA()

@property(nonatomic,strong)UIButton *buttonA;


@property(nonatomic,strong)ViewB* viewB;



@end

@implementation ViewA


- (void)drawRect:(CGRect)rect {

    [self setupUI];
    [self addConstraints];
}

-(void)setupUI
{
    self.buttonA = [[UIButton alloc]init];
    [self addSubview:self.buttonA];
    [self.buttonA setTitle:@"我是ButtonA" forState:UIControlStateNormal];
    [self.buttonA setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [self.buttonA setBackgroundColor:[UIColor yellowColor]];
    [self.buttonA addTarget:self action:@selector(aAction:) forControlEvents:UIControlEventTouchUpInside];
    
    self.viewB = [[ViewB alloc]init];
    [self addSubview:self.viewB];
    [self.viewB  setBackgroundColor:[UIColor clearColor]];
    
    [self.viewB setNeedsDisplay];
}

-(void)aAction:(id)sender
{
    NSLog(@"你点击了buttonA");
}

-(void)bAction:(id)sender
{
    NSLog(@"你点击了buttonA");
}



-(void)addConstraints
{

    [self.buttonA setTranslatesAutoresizingMaskIntoConstraints:NO];
    [self.viewB setTranslatesAutoresizingMaskIntoConstraints:NO];

    NSDictionary *views = @{@"buttonA":self.buttonA,@"viewB":self.viewB
                            };

    NSDictionary *metrics = @{@"width": @(50)};
    

    [self addConstraints:[NSLayoutConstraint
                                constraintsWithVisualFormat:@"H:|-0-[buttonA]-0-|" options:NSLayoutFormatDirectionLeadingToTrailing metrics:metrics views:views]];
    [self addConstraints:[NSLayoutConstraint
                                constraintsWithVisualFormat:@"V:|-65-[buttonA(==width)]" options:NSLayoutFormatDirectionLeadingToTrailing metrics:metrics views:views]];
    
    [self addConstraints:[NSLayoutConstraint
                                constraintsWithVisualFormat:@"H:|-0-[viewB]-0-|" options:NSLayoutFormatDirectionLeadingToTrailing metrics:metrics views:views]];
    
    [self addConstraints:[NSLayoutConstraint
                          constraintsWithVisualFormat:@"V:|-0-[viewB]-0-|" options:NSLayoutFormatDirectionLeadingToTrailing metrics:metrics views:views]];
}


-(UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    // 当touch point是在_btn上，则hitTest返回_btn
    CGPoint btnPointInA = [self.buttonA convertPoint:point fromView:self];
    if ([self.buttonA pointInside:btnPointInA withEvent:event]) {
        return self.buttonA;
    }
    // 否则，返回默认处理
    return [super hitTest:point withEvent:event];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSLog(@"A - touchesBeagan..");
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSLog(@"A - touchesCancelled..");
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSLog(@"A - touchesEnded..");
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSLog(@"A - touchesMoved..");
    
}

@end
