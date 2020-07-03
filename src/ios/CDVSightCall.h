#import <Cordova/CDVPlugin.h>
#if !(TARGET_OS_SIMULATOR)
#import <UserNotifications/UserNotifications.h>
#import <LSUniversalSDK/LSUniversalSDK.h>
#import "CallLocalNotification.h"

@interface CDVSightCall : CDVPlugin<LSUniversalDelegate, LSPictureProtocol, UNUserNotificationCenterDelegate, LSUniversalLogDelegate>

#if !(TARGET_OS_SIMULATOR)
@property (strong, nonatomic) LSUniversal* lsUniversal;
#endif
@property (nonatomic, copy) NSString *listenerCallbackID;

@property NSString *apnsToken;

- (void)registerListener:(CDVInvokedUrlCommand *)command;
- (void)demo:(CDVInvokedUrlCommand*)command;
- (void)enableLogger:(CDVInvokedUrlCommand*)command;
- (void)setEnvironment:(CDVInvokedUrlCommand*)command;
- (void)isAgentAvailable:(CDVInvokedUrlCommand*)command;
- (void)registerAgent:(CDVInvokedUrlCommand*)command;
- (void)fetchUseCases:(CDVInvokedUrlCommand*)command;
- (void)generateCallURL:(CDVInvokedUrlCommand*)command;
- (void)startCall:(CDVInvokedUrlCommand*)command;
- (void)handleCallLocalNotification:(CDVInvokedUrlCommand*)command;

+ (void)setNotificationDeviceToken:(NSString*)token;
+ (void)handleSightcallPush:(NSDictionary*)userInfo;
@end
#endif

