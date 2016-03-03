//
//  LighterCell.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/21.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "LighterCell.h"

@interface LighterCell()
{
    __weak IBOutlet UILabel *one;

}

@end


@implementation LighterCell


- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

}

-(void)setHighlighted:(BOOL)highlighted animated:(BOOL)animated
{
    [super setHighlighted:highlighted animated:animated];
    if (highlighted) {
        self.title.shadowColor = [UIColor darkGrayColor];
        self.title.shadowOffset = CGSizeMake(3, 3);
    } else {
        self.title.shadowColor = nil;
    }
}



@end
