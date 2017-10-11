//
//  LSMobile2MobileProtocol.h
//  LSUniversalSDK
//
//  Created by Charles Thierry on 21/11/16.
//  Copyright © 2016 SightCall. All rights reserved.
//

#import <LSUniversalSDK/LSUniversalSDK.h>

/**
 */
@protocol LSMobile2Mobile <NSObject>

/**
 *  TRUE if the module has credentials. FALSE otherwise.
 *  Having credentials is not a guarantee of having any usecases.
 */
@property(nonatomic, readonly, getter=isRegistered) BOOL registered;

/**
 *  If this is different from nil, notifications can be sent. This array can be empty. The UsecaseID can be found in each entry of
 *  this array.
 */
@property(nonatomic, readonly) NSArray *usecases;

/**
 *	 The application notification token. It must be set for the registration to work.
 */
@property(nonatomic) NSString *notificationToken;

/**
 *  Removes the credentials from the memory
 */
- (void)clearCredentials;

/**
 *  Sends a notification to a guest. This notification will be sent both as a sms and a VOIP notification.
 *
 *  @param usecaseID   The usecase ID.
 *  @param phoneNumber The Guest Phone number. Either in international format (ex +33601234567) or national format and we'll try to guess the country code.
 *  @param block  The notification block
 */
- (void)sendNotificationForUsecase:(NSInteger)usecaseID toPhone:(NSString *)phoneNumber andNotify:(void (^)(NSInteger))block;

/**
 *  Sends a notification to a guest. This notification will be sent both as an email and a VOIP notification.
 *
 *  @param usecaseID The usecase ID.
 *  @param email     The email to send the notification to.
 *  @param block  The notification block
 */
- (void)sendNotificationForUsecase:(NSInteger)usecaseID toEmail:(NSString *)email andNotify:(void (^)(NSInteger))block;

@end


