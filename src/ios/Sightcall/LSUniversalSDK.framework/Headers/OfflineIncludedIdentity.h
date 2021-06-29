//
//  OfflineIncludedIdentity.h
//  LSUniversalSDK
//
//  Created by Ben Maad on 16/03/2020.
//  Copyright © 2020 SightCall. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface OfflineIncludedIdentity : NSObject

@property (nonatomic) NSString *idIncluded;
@property (nonatomic) NSString *typeIncluded;
@property (nonatomic) NSString *attributeName;
@property (nonatomic) BOOL objId;
@property (nonatomic) NSString *objIdLabel;
@property (nonatomic) BOOL objIdMandatory;
@property (nonatomic) NSString *objIdPlaceholder;
@property (nonatomic) BOOL reference;
@property (nonatomic) NSString *referenceLabel;
@property (nonatomic) BOOL referenceMandatory;
@property (nonatomic) NSString *referencePlaceholder;

- (instancetype)initWithDescription:(NSDictionary *)d;

@end

NS_ASSUME_NONNULL_END
