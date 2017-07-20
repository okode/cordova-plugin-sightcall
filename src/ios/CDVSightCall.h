#import <Cordova/CDVPlugin.h>
#if !(TARGET_OS_SIMULATOR)
#import <LSUniversalSDK/LSUniversalSDK.h>
#endif

@interface CDVSightCall : CDVPlugin

#if !(TARGET_OS_SIMULATOR)
@property (strong, nonatomic) LSUniversal* lsUniversal;
#endif

- (void)demo:(CDVInvokedUrlCommand*)command;
- (void)isAgentAvailable:(CDVInvokedUrlCommand*)command;
- (void)registerAgent:(CDVInvokedUrlCommand*)command;
- (void)fetchUseCases:(CDVInvokedUrlCommand*)command;
- (void)invite:(CDVInvokedUrlCommand*)command;
- (void)generateURL:(CDVInvokedUrlCommand*)command;
- (void)startCall:(CDVInvokedUrlCommand*)command;
- (void)canHandleNotification:(CDVInvokedUrlCommand*)command;
- (void)handleNotification:(CDVInvokedUrlCommand*)command;

@end
