#import <Cordova/CDVPlugin.h>
#if !(TARGET_OS_SIMULATOR)
#import <UserNotifications/UserNotifications.h>
#import <LSUniversalSDK/LSUniversalSDK.h>
#import "CallLocalNotification.h"
#import <PushKit/PushKit.h>

@interface CDVSightCall : CDVPlugin<LSUniversalDelegate, LSPictureProtocol, UNUserNotificationCenterDelegate, PKPushRegistryDelegate>

#if !(TARGET_OS_SIMULATOR)
@property (strong, nonatomic) LSUniversal* lsUniversal;
#endif
@property (nonatomic, copy) NSString *listenerCallbackID;

- (void)registerListener:(CDVInvokedUrlCommand *)command;
- (void)demo:(CDVInvokedUrlCommand*)command;
- (void)isAgentAvailable:(CDVInvokedUrlCommand*)command;
- (void)registerAgent:(CDVInvokedUrlCommand*)command;
- (void)fetchUseCases:(CDVInvokedUrlCommand*)command;
- (void)invite:(CDVInvokedUrlCommand*)command;
- (void)generateCallURL:(CDVInvokedUrlCommand*)command;
- (void)startCall:(CDVInvokedUrlCommand*)command;
- (void)handleCallLocalNotification:(CDVInvokedUrlCommand*)command;
- (void)revokeInvitation:(CDVInvokedUrlCommand*)command;

@end
#endif

