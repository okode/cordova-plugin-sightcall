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

@implementation CDVSightCall

#pragma mark - Plugin Initialization

- (void)pluginInitialize
{
    self.lsUniversal = [[LSUniversal alloc] init];
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

- (void)registerAgent:(CDVInvokedUrlCommand*)command
{
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        if ([self.lsUniversal.agentHandler isAvailable]) {
            completionHandler(CDVCommandStatus_OK, @"Agent is already registered");
            return;
        }
        NSString *token = [[args objectAtIndex:0] stringValue];
        NSString *pin = [[args objectAtIndex:1] stringValue];
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
    if ([self.lsUniversal.agentHandler isAvailable]) {
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
                NSString *phoneNumber = [[args objectAtIndex:0] stringValue];
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
        NSString *url = [[args objectAtIndex:0] stringValue];
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
