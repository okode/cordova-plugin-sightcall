//
//  LSNotification.h
//  LSUniversalSDK
//
//  Created by Martin Prot on 24/08/2020.
//  Copyright © 2020 SightCall. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <LSUniversalSDK/LSMATypes.h>

@class LSDeeplinkCommand;

typedef NS_ENUM(NSInteger, LSNotificationType)
{
    LSNotificationTypeUnknown,
    LSNotificationTypeTest,
    LSNotificationTypeFeature
};


NS_ASSUME_NONNULL_BEGIN

@interface LSNotification : NSObject

@property (nonatomic, readonly) LSNotificationType type;
@property (nonnull, nonatomic, readonly, strong) NSString *title;
@property (nonnull, nonatomic, readonly, strong) NSString *text;

@property (nullable, nonatomic, readonly, strong) LSDeeplinkCommand *feature;

- (nullable instancetype)initWithAPNPayload:(NSDictionary *)payload;

+ (BOOL)canHandlePayload:(NSDictionary*)payload;

@end

NS_ASSUME_NONNULL_END
