//
//  AppDelegate+Sightcall.m
//  APPCOL
//
//  Created by Rubén Panadero Navarrete on 28/02/2019.
//

#import "AppDelegate+Sightcall.h"
#import <objc/runtime.h>

@implementation AppDelegate (Sightcall)

// its dangerous to override a method from within a category.
// Instead we will use method swizzling. we set this up in the load call.
+ (void)load
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        Class class = [self class];
        
        SEL originalSelector = @selector(shouldAutorotate);
        SEL swizzledSelector = @selector(pluginSwizzledInit);
        
        Method originalMethod = class_getInstanceMethod(class, originalSelector);
        Method swizzledMethod = class_getInstanceMethod(class, swizzledSelector);
        
        BOOL didAddMethod = class_addMethod(class,
                                            originalSelector,
                                            method_getImplementation(swizzledMethod),
                                            method_getTypeEncoding(swizzledMethod));
        
        if (didAddMethod) {
            class_replaceMethod(class,
                                swizzledSelector,
                                method_getImplementation(originalMethod),
                                method_getTypeEncoding(originalMethod));
        } else {
            method_exchangeImplementations(originalMethod, swizzledMethod);
        }
    });
}

- (AppDelegate *)pluginSwizzledInit
{
    UIUserNotificationType notificationTypes =
    (UIUserNotificationTypeSound | UIUserNotificationTypeAlert | UIUserNotificationTypeBadge);
    UIUserNotificationSettings *settings = [UIUserNotificationSettings settingsForTypes:notificationTypes categories:nil];
    [[UIApplication sharedApplication] registerUserNotificationSettings:settings];
    [[UIApplication sharedApplication] registerForRemoteNotifications];
    // This actually calls the original init method over in AppDelegate. Equivilent to calling super
    // on an overrided method, this is not recursive, although it appears that way.
    return [self pluginSwizzledInit];
}

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    
    NSLog(@"Sightcall - Notifications device token received");
    NSString * deviceTokenString = [[[[deviceToken description]
                                      stringByReplacingOccurrencesOfString: @"<" withString: @""]
                                     stringByReplacingOccurrencesOfString: @">" withString: @""]
                                    stringByReplacingOccurrencesOfString: @" " withString: @""];
    [CDVSightCall setNotificationDeviceToken:deviceTokenString];
}


- (void)application:(UIApplication *)application
didReceiveRemoteNotification:(NSDictionary *)userInfo
fetchCompletionHandler:(void (^)(UIBackgroundFetchResult result))completionHandler {
    NSLog(@"Sightcall - Notification received");
    [CDVSightCall handleSightcallPush:userInfo];
}

@end
