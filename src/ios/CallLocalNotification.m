//
//  CallLocalNotification.m
//  SightCallDemo
//
//  Created by Rubén Panadero Navarrete on 18/10/17.
//  Copyright © 2017 Okode. All rights reserved.
//

#import "CallLocalNotification.h"

@implementation CallLocalNotification

NSString *const CallLocalNotificationCategory = @"SIGHTCALL_CALL_CATEGORY";
NSString *const CallLocalNotificationID = @"SIGHTCALL_LOCALNOTIFICATION";
NSString *const CallLocalNotificationAcceptActionID = @"ACCEPT_CALL";
NSString *const CallLocalNotificationRejectActionID = @"REJECT_CALL";

NSString *const NOTIFICATION_TITLE = @"Llamada entrante...";
NSString *const NOTIFICATION_MSG = @"Pulse para aceptar la llamada";
NSString *const NOTIFICATION_ACCEPT_ACTION = @"Accept";
NSString *const NOTIFICATION_REJECT_ACTION = @"Reject";


+ (UNMutableNotificationContent*) buildCallNotificationContent: (NSString *)callUrl {
    UNMutableNotificationContent* content = [[UNMutableNotificationContent alloc] init];
    content.title = [NSString localizedUserNotificationStringForKey:NOTIFICATION_TITLE arguments:nil];
    content.body = [NSString localizedUserNotificationStringForKey:NOTIFICATION_MSG
                                                         arguments:nil];
    content.sound = [UNNotificationSound defaultSound];
    
    content.categoryIdentifier = CallLocalNotificationCategory;
    content.userInfo = [CallLocalNotification buildNotificationInfo:callUrl];
    return content;
}

+ (UILocalNotification*) buildUILocalNotification: (NSString *)callUrl {
    UILocalNotification *localNotification = [[UILocalNotification alloc] init];
    localNotification.alertBody = NOTIFICATION_TITLE;
    localNotification.soundName = UILocalNotificationDefaultSoundName;
    localNotification.category = CallLocalNotificationCategory;
    localNotification.userInfo = [CallLocalNotification buildNotificationInfo:callUrl];
    return localNotification;
}

+ (NSDictionary*) buildNotificationInfo: (NSString *)callUrl {
    return @{ @"callUrl": callUrl, @"notificationId": CallLocalNotificationID };
}

+ (UNNotificationCategory*) getUNNNotificationCategory {
    // Create the custom actions for expired timer notifications.
    UNNotificationAction* acceptCallAction = [UNNotificationAction
                                              actionWithIdentifier:CallLocalNotificationAcceptActionID
                                              title:NOTIFICATION_ACCEPT_ACTION
                                              options:UNNotificationActionOptionForeground];
    
    UNNotificationAction* rejectCallAction = [UNNotificationAction
                                              actionWithIdentifier:CallLocalNotificationRejectActionID
                                              title:NOTIFICATION_REJECT_ACTION
                                              options:UNNotificationActionOptionNone];
    
    // Create the category with the custom actions.
    UNNotificationCategory* callCategory = [UNNotificationCategory
                                            categoryWithIdentifier:CallLocalNotificationCategory
                                            actions:@[acceptCallAction, rejectCallAction]
                                            intentIdentifiers:@[]
                                            options:UNNotificationCategoryOptionNone];
    return callCategory;
}

+ (UIMutableUserNotificationCategory*) getUIMutableUserNotificationCategory {
    UIMutableUserNotificationAction *acceptCallAction = [[UIMutableUserNotificationAction alloc] init];
    acceptCallAction.identifier = CallLocalNotificationAcceptActionID;
    acceptCallAction.title = NOTIFICATION_ACCEPT_ACTION;
    acceptCallAction.activationMode = UIUserNotificationActivationModeForeground;
    acceptCallAction.destructive = false;
    
    UIMutableUserNotificationAction *rejectCallAction = [[UIMutableUserNotificationAction alloc] init];
    rejectCallAction.identifier =CallLocalNotificationRejectActionID;
    rejectCallAction.title = NOTIFICATION_REJECT_ACTION;
    rejectCallAction.activationMode = UIUserNotificationActivationModeBackground;
    rejectCallAction.authenticationRequired = false;
    rejectCallAction.destructive = true;
    
    UIMutableUserNotificationCategory *callCategory = [[UIMutableUserNotificationCategory alloc] init];
    callCategory.identifier = CallLocalNotificationCategory;
    [callCategory setActions:@[acceptCallAction, rejectCallAction] forContext: UIUserNotificationActionContextDefault];
    [callCategory setActions:@[acceptCallAction, rejectCallAction] forContext: UIUserNotificationActionContextMinimal];
    return callCategory;
}

+(bool) isLocalCallNotification:(NSDictionary*)payload {
    return payload != NULL && [CallLocalNotificationID isEqualToString:payload[@"notificationId"]];
}

@end
