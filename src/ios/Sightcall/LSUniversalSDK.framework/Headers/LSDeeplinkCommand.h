//
//  LSDeeplinkCommand.h
//  LSUniversalSDK
//
//  Created by Martin Prot on 27/08/2020.
//  Copyright © 2020 SightCall. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <LSUniversalSDK/LSMATypes.h>

typedef NS_ENUM(NSInteger, LSDeeplinkCommandType)
{
    LSDeeplinkCommandTypeVideoSession,
    LSDeeplinkCommandTypeUploadFile,
    LSDeeplinkCommandTypeSmartOCR,
    LSDeeplinkCommandTypeAROverlay
};

NS_ASSUME_NONNULL_BEGIN

@interface LSDeeplinkCommand : NSObject

@property (nonatomic, readonly) LSDeeplinkCommandType feature;
@property (nullable, nonatomic, readonly, strong) NSString *usecaseId;
@property (nullable, nonatomic, readonly, strong) NSString *objectId;
@property (nullable, nonatomic, readonly, strong) NSString *reference;
@property (nonatomic, readonly) LSMAUsecaseMedia_t sendType;
@property (nullable, nonatomic, readonly, strong) NSString *recipient;

- (nullable instancetype)initWithURL:(NSURL *)url;
- (nullable instancetype)initWithDictionary:(NSDictionary *)dictionary;
+ (BOOL)canHandlePayload:(NSDictionary*)payload;

@end

NS_ASSUME_NONNULL_END
