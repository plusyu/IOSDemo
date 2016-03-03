//
//  Lighter.m
//  IOSDemo
//
//  Created by yuxiang on 14/11/21.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import "Lighter.h"

static NSString * const IdentifierKey = @"identifier";
static NSString * const NameKey = @"name";
static NSString * const CreationDateKey = @"creationDate";
static NSString * const RatingKey = @"rating";
@implementation Lighter

- (NSString*)description
{
    return [NSString stringWithFormat:@"<%@: %p> (%lld) \"%@\"",
            [self class], self, (long long) self.identifier, self.name];
}

- (id)initWithCoder:(NSCoder*)coder
{
    self = [super init];
    if (self) {
        self.identifier = [coder decodeInt64ForKey:IdentifierKey];
        self.name = [coder decodeObjectOfClass:[NSString class] forKey:NameKey];
        self.creationDate = [coder decodeObjectOfClass:[NSDate class] forKey:CreationDateKey];
        self.rating = [coder decodeDoubleForKey:RatingKey];
    }
    return self;
}

- (void)encodeWithCoder:(NSCoder*)coder
{
    [coder encodeInt64:self.identifier forKey:IdentifierKey];
    [coder encodeObject:self.name forKey:NameKey];
    [coder encodeObject:self.creationDate forKey:CreationDateKey];
    [coder encodeDouble:self.rating forKey:RatingKey];

}

- (NSString*)authorFullName
{
    return @"authorFullName";
}

- (double)adjustedRating
{
    double adjustedRating = (self.rating - 97) / 3.0;
    if (adjustedRating < 0) {
        adjustedRating = 0;
    }
    return adjustedRating;
}


@end
