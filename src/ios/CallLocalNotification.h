//
//  CallLocalNotification.h
//  SightCallDemo
//
//  Created by Rubén Panadero Navarrete on 18/10/17.
//  Copyright © 2017 Okode. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UserNotifications/UserNotifications.h>
#import <UIKit/UIKit.h>

@interface CallLocalNotification : NSObject

extern NSString *const CallLocalNotificationCategory;
extern NSString *const CallLocalNotificationID;
extern NSString *const CallLocalNotificationAcceptActionID;
extern NSString *const CallLocalNotificationRejectActionID;

/** Used on iOS 10+. On iOS9 or lower, use buildUILocalNotification
 **/
+ (UNMutableNotificationContent*) buildCallNotificationContent: (NSDictionary *)sightcallPushPayload;
/** Used on iOS 9 or lower. On iOS10+, use buildCallNotificationContent
 **/
+ (UILocalNotification*) buildUILocalNotification: (NSDictionary *)sightcallPushPayload;
/** Used on iOS 10+. On iOS9 or lower, use buildUILocalNotification
 **/
+ (UNNotificationCategory*) getUNNNotificationCategory;
/** Used on iOS 9 or lower. On iOS10+, use buildCallNotificationContent
 **/
+ (UIMutableUserNotificationCategory*) getUIMutableUserNotificationCategory;

+(bool) isLocalCallNotification:(NSDictionary*)payload;
@end
