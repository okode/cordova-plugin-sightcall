#import <Cordova/CDVPlugin.h>
#import <LSUniversalSDK/LSUniversalSDK.h>

@interface CDVSightCall : CDVPlugin

@property (strong, nonatomic) LSUniversal* lsUniversal;

- (void)demo:(CDVInvokedUrlCommand*)command;
- (void)registerAgent:(CDVInvokedUrlCommand*)command;
- (void)fetchUseCases:(CDVInvokedUrlCommand*)command;
- (void)invite:(CDVInvokedUrlCommand*)command;
- (void)generateURL:(CDVInvokedUrlCommand*)command;
- (void)startCall:(CDVInvokedUrlCommand*)command;
- (void)canHandleNotification:(CDVInvokedUrlCommand*)command;
- (void)handleNotification:(CDVInvokedUrlCommand*)command;

@end
