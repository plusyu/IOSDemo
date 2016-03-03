//
//  SearchViewModel.h
//  IOSDemo
//
//  Created by yuxiang on 14/11/28.
//  Copyright (c) 2014年 yu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ReactiveCocoa/ReactiveCocoa.h>

@interface SearchViewModel : NSObject

/// The current search text
@property (nonatomic, strong) NSString *searchText;

/// An array of CETweetViewModel instances which indicate the current search results
@property (nonatomic, strong) NSArray *searchResults;

/// A command which when executed searches twitter using the current searchText
@property (nonatomic, strong) RACCommand *searchCommand;

/// A command which when executed when a tweet is selected
@property (nonatomic, strong) RACCommand *tweetSelectedCommand;

@end
