#import "CDVSightCall.h"
#if !(TARGET_OS_SIMULATOR)
typedef void (^CordovaCompletionHandler)(CDVCommandStatus, id);
typedef void (^CordovaExecutionBlock)(NSArray *args, CordovaCompletionHandler completionHandler);

/**
 *  The use cases fetch block that is fired in _fetchUseCases:
 *
 *  @param success      YES if the fetch did succeed
 *  @param msg  The error message if there was an error
 *  @param usecaselist          List of use cases if the fetch did succeed
 */
typedef void (^fetchUseCasesBlock)(BOOL success, NSString *msg,  NSArray<NSObject<LSMAUsecase> *> *usecaselist);

// Events
NSString *const EVENT_TYPE = @"eventType";
NSString *const EVENT_DATA = @"eventData";
NSString *const CALL_REPORT_EVENT_RECEIVED = @"sightcall.callreportevent";
NSString *const STATUS_EVENT_RECEIVED = @"sightcall.statusevent";

// UNIVERSAL STATUS
NSString *const IDLE_STATUS = @"IDLE";
NSString *const CONNECTING_STATUS = @"CONNECTING";
NSString *const ACTIVE_STATUS = @"ACTIVE";

// UNIVERSAL END REASON
NSString *const END_FOR_UNEXPECTED_ERROR = @"UNEXPECTED";
NSString *const END_LOCAL = @"LOCAL";
NSString *const END_REMOTE = @"REMOTE";

@implementation CDVSightCall

#pragma mark - Plugin Initialization

- (void)pluginInitialize
{
    self.lsUniversal = [[LSUniversal alloc] init];
}

- (void)registerListener:(CDVInvokedUrlCommand *)command {
    self.listenerCallbackID = command.callbackId;
    self.lsUniversal.delegate = self;
}

-(void) connectionEvent:(lsConnectionStatus_t)status
{
    // We format the status data to return the same values in Android and iOS.
    NSString *statusData = NULL;
    switch (status) {
        case lsConnectionStatus_idle:
        {
            statusData = IDLE_STATUS;
            break;
        }
        case lsConnectionStatus_connecting:
        {
            statusData = CONNECTING_STATUS;
            break;
        }
        case lsConnectionStatus_callActive:
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                [self.viewController presentViewController:self.lsUniversal.callViewController animated:YES completion:nil];
            });
            statusData = ACTIVE_STATUS;
            break;
        }
        case lsConnectionStatus_disconnecting:
            [self.viewController dismissViewControllerAnimated:TRUE completion:nil];
            break;
        default: break;
    }
    if (statusData != NULL) {
        [self notifyListener:STATUS_EVENT_RECEIVED data:@{ @"status": statusData }];
    }
}

- (void)connectionError:(lsConnectionError_t)error
{
    NSLog(@"SightCall connection error...");
}


- (void)callReport:(lsCallReport_s)callEnd
{
    NSString *callEndReason = NULL;
    switch (callEnd.callEnd) {
        case lsCallEnd_local:
        {
            callEndReason = END_LOCAL;
            break;
        }
        case lsCallEnd_remote:
        {
            callEndReason = END_REMOTE;
            break;
        }
        case lsCallEnd_unexpected:
        {
            callEndReason = END_FOR_UNEXPECTED_ERROR;
            break;
        }
        default: break;
    }
    if (callEndReason != NULL) {
        NSNumber *duration = [NSNumber numberWithDouble:callEnd.callLength];
        [self notifyListener:CALL_REPORT_EVENT_RECEIVED data:@{ @"endReason": callEndReason, @"duration": duration }];
    }
}

- (void)demo:(CDVInvokedUrlCommand*)command
{
    NSLog(@"SightCall: demo function invoked");
}

- (void)isAgentAvailable:(CDVInvokedUrlCommand*)command
{
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        if ([self.lsUniversal.agentHandler isAvailable]) {
            completionHandler(CDVCommandStatus_OK, @"Agent is available");
            return;
        } else {
            completionHandler(CDVCommandStatus_ERROR, @"Agent not available");
        }
    }];
}

- (void)setNotificationToken:(CDVInvokedUrlCommand*)command
{
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        [self.lsUniversal.agentHandler setNotificationToken:[args objectAtIndex:0]];
    }];
}

- (void)registerAgent:(CDVInvokedUrlCommand*)command
{
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        if ([self.lsUniversal.agentHandler isAvailable]) {
            completionHandler(CDVCommandStatus_OK, @"Agent is already registered");
            return;
        }
        NSString *token = [args objectAtIndex:0];
        NSString *pin = [args objectAtIndex:1];
        if (pin == NULL || token == NULL) {
            completionHandler(CDVCommandStatus_ERROR, @"Error, token or pin param is NULL");
        }
        [self.lsUniversal.agentHandler registerWithPin:pin andToken:token onSignIn:^(BOOL success, NSInteger statusCode, RegistrationError_t status){
            if (success) {
                completionHandler(CDVCommandStatus_OK, @"Agent registration succeeded");
            } else {
                completionHandler(CDVCommandStatus_ERROR, @"Agent registration failed");
            }
        }];
    }];
}

- (void)_fetchUseCases:(nonnull fetchUseCasesBlock) handler
{
    if (![self.lsUniversal.agentHandler isAvailable]) {
        handler(false, @"Register the agent before", NULL);
        return;
    }
    [self.lsUniversal.agentHandler fetchUsecases:^(BOOL success, NSArray<NSObject<LSMAUsecase> *> *usecaselist) {
        handler(success, NULL, usecaselist);
    }];
}

- (void)fetchUseCases:(CDVInvokedUrlCommand*)command
{
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        [self _fetchUseCases:^(BOOL success, NSString *msg, NSArray<NSObject<LSMAUsecase> *> *usecaselist) {
            if (success) {
                completionHandler(CDVCommandStatus_OK, @"Use cases fetched successfully");
            } else {
                completionHandler(CDVCommandStatus_ERROR,  [NSString stringWithFormat:@"Error fetching use cases. Reason: %@", msg]);
            }
        }];
    }];
}


- (void)invite:(CDVInvokedUrlCommand*)command
{
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        [self _fetchUseCases:^(BOOL success, NSString *msg, NSArray<NSObject<LSMAUsecase> *> *usecaselist) {
            if (success) {
                NSString *phoneNumber = [args objectAtIndex:0];
                if (phoneNumber == NULL) {
                    completionHandler(CDVCommandStatus_ERROR, @"Error, phone number is NULL");
                    return;
                }
                [self.lsUniversal.agentHandler sendNotificationForUsecase:(LSMAGuestUsecase*)[usecaselist objectAtIndex:0] toPhone:phoneNumber andDisplayName:@"GUEST" andNotify:^(NSInteger statusCode) {
                    if (statusCode == 200) {
                        completionHandler(CDVCommandStatus_OK, @"Invitation was sent successfully");
                    } else {
                        completionHandler(CDVCommandStatus_ERROR, @"Invitation couldn't be sent");
                    }
                }];
            } else {
                completionHandler(CDVCommandStatus_ERROR,  [NSString stringWithFormat:@"Error sending the invitation. Unexpected error fetching use cases. Reason: %@", msg]);
            }
        }];
    }];
}

- (void)generateURL:(CDVInvokedUrlCommand*)command
{
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        [self _fetchUseCases:^(BOOL success, NSString *msg, NSArray<NSObject<LSMAUsecase> *> *usecaselist) {
            if (success) {
                [self.lsUniversal.agentHandler createInvitationForUsecase:(LSMAGuestUsecase*)[usecaselist objectAtIndex:0]  usingSuffix:@"GUEST" andNotify:^(BOOL didSucceed, NSString * _Nullable invite) {
                    if (didSucceed) {
                        completionHandler(CDVCommandStatus_OK, invite);
                    } else {
                        completionHandler(CDVCommandStatus_ERROR, @"Error generating the call URL");
                    }
                }];
            } else {
                completionHandler(CDVCommandStatus_ERROR,  [NSString stringWithFormat:@"Error generating the call URL. Unexpected error fetching use cases. Reason: %@", msg]);
            }
        }];
    }];
}

- (void)startCall:(CDVInvokedUrlCommand*)command
{
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        NSString *url = [args objectAtIndex:0];
        [self.lsUniversal startWithString:url];
    }];
}

- (void)canHandleNotification:(CDVInvokedUrlCommand*)command
{
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        NSDictionary *payload = [args objectAtIndex:0];
        if ([self.lsUniversal canHandleNotification:payload]) {
            completionHandler(CDVCommandStatus_OK, @"The notification could be handled");
        } else {
            completionHandler(CDVCommandStatus_ERROR, @"The notification couldn't be handled");
        }
    }];
}

- (void)handleNotification:(CDVInvokedUrlCommand*)command
{
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        NSDictionary *payload = [args objectAtIndex:0];
        if ([self.lsUniversal canHandleNotification:payload]) {
            [self.lsUniversal handleNotification:payload];
        }
    }];
}

- (BOOL)notifyListener:(NSString *)eventType data:(NSDictionary *)data {
    if (!self.listenerCallbackID) {
        UA_LTRACE(@"Listener callback unavailable.  event %@", eventType);
        return NO;
    }
    
    NSMutableDictionary *message = [NSMutableDictionary dictionary];
    [message setValue:eventType forKey:EVENT_TYPE];
    [message setValue:data forKey:EVENT_DATA];
    
    CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:message];
    [result setKeepCallbackAsBool:YES];
    
    [self.commandDelegate sendPluginResult:result callbackId:self.listenerCallbackID];
    return YES;
}

#pragma mark Cordova plugin utilities


/**
 * Helper method to create a plugin result with the specified value.
 *
 * @param value The result's value.
 * @param status The result's status.
 * @returns A CDVPluginResult with specified value.
 */
- (CDVPluginResult *)pluginResultForValue:(id)value status:(CDVCommandStatus)status
{
    /*
     NSString -> String
     NSNumber --> (Integer | Double)
     NSArray --> Array
     NSDictionary --> Object
     NSNull --> no return value
     nil -> no return value
     */
    
    // String
    if ([value isKindOfClass:[NSString class]]) {
        return [CDVPluginResult resultWithStatus:status
                                 messageAsString:[value stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    }
    
    // Number
    if ([value isKindOfClass:[NSNumber class]]) {
        CFNumberType numberType = CFNumberGetType((CFNumberRef)value);
        //note: underlyingly, BOOL values are typedefed as char
        if (numberType == kCFNumberIntType || numberType == kCFNumberCharType) {
            return [CDVPluginResult resultWithStatus:status messageAsInt:[value intValue]];
        } else  {
            return [CDVPluginResult resultWithStatus:status messageAsDouble:[value doubleValue]];
        }
    }
    
    // Array
    if ([value isKindOfClass:[NSArray class]]) {
        return [CDVPluginResult resultWithStatus:status messageAsArray:value];
    }
    
    // Object
    if ([value isKindOfClass:[NSDictionary class]]) {
        return [CDVPluginResult resultWithStatus:status messageAsDictionary:value];
    }
    
    // Null
    if ([value isKindOfClass:[NSNull class]]) {
        return [CDVPluginResult resultWithStatus:status];
    }
    
    // Nil
    if (!value) {
        return [CDVPluginResult resultWithStatus:status];
    }
    
    NSLog(@"Cordova callback block returned unrecognized type: %@", NSStringFromClass([value class]));
    return [CDVPluginResult resultWithStatus:status];
}

/**
 * Helper method to perform a cordova command asynchronously.
 *
 * @param command The cordova command.
 * @param block The UACordovaExecutionBlock to execute.
 */
- (void)performCallbackWithCommand:(CDVInvokedUrlCommand *)command withBlock:(CordovaExecutionBlock)block
{
    [self.commandDelegate runInBackground:^{
        CordovaCompletionHandler completionHandler = ^(CDVCommandStatus status, id value) {
            CDVPluginResult *result = [self pluginResultForValue:value status:status];
            [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
        };
        
        if (!block) {
            completionHandler(CDVCommandStatus_OK, nil);
        } else {
            block(command.arguments, completionHandler);
        }
    }];
}


@end
#endif
