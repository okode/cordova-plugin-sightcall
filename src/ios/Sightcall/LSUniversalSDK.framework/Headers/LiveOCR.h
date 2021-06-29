//
//  LiveOCR.h
//  LSUniversalSDK
//
//  Created by Ben Maad on 18/06/2020.
//  Copyright © 2020 SightCall. All rights reserved.
//

#import <Foundation/Foundation.h>

@class LiveOCRAttributes;

NS_ASSUME_NONNULL_BEGIN

@interface LiveOCR : NSObject

@property (nonatomic) NSString *idOCR;
@property (nonatomic) NSString *typeOCR;
@property (nonatomic) LiveOCRAttributes *attributes;

- (instancetype)initWithDescription:(NSDictionary *)d;

@end

NS_ASSUME_NONNULL_END
