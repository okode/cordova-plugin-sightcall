#define SYSTEM_VERSION_GRATERTHAN_OR_EQUALTO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)
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
NSString *const MEDIA_EVENT_RECEIVED = @"sightcall.mediaevent";
NSString *const GUEST_READY_EVENT_RECEIVED = @"sightcall.ios.onguestready";
NSString *const CALL_ACCEPTED_EVENT_RECEIVED = @"sightcall.ios.oncallaccepted";

// UNIVERSAL STATUS
NSString *const IDLE_STATUS = @"IDLE";
NSString *const CONNECTING_STATUS = @"CONNECTING";
NSString *const ACTIVE_STATUS = @"ACTIVE";
NSString *const DISCONNECTING_STATUS = @"DISCONNECTING";

// UNIVERSAL END REASON
NSString *const END_FOR_UNEXPECTED_ERROR = @"UNEXPECTED";
NSString *const END_LOCAL = @"LOCAL";
NSString *const END_REMOTE = @"REMOTE";

@implementation CDVSightCall

#pragma mark - Plugin Initialization

- (void)pluginInitialize
{
    self.lsUniversal = [[LSUniversal alloc] init];
    PKPushRegistry *pushRegistry = [[PKPushRegistry alloc] initWithQueue:dispatch_get_main_queue()];
    pushRegistry.delegate = self;
    pushRegistry.desiredPushTypes = [NSSet setWithObject:PKPushTypeVoIP];
}

- (void)registerListener:(CDVInvokedUrlCommand *)command {
    self.listenerCallbackID = command.callbackId;
    self.lsUniversal.delegate = self;
    [self.lsUniversal setPictureDelegate: self];
}

#pragma mark - LSUniversalDelegate

- (void) connectionEvent:(lsConnectionStatus_t)status
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
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                [self.viewController dismissViewControllerAnimated:TRUE completion:nil];
            });
            statusData = DISCONNECTING_STATUS;
            break;
        }
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
        NSTimeInterval duration = callEnd.callLength;
        NSNumber *time = [NSNumber numberWithInt:0];
        if (!isnan(duration)) {
            time = [NSNumber numberWithDouble:callEnd.callLength];
        }
        [self notifyListener:CALL_REPORT_EVENT_RECEIVED data:@{ @"endReason": callEndReason, @"duration": time }];
    }
}

- (void)savedPicture:(UIImage *_Nullable)image andMetadata:(LSPictureMetadata *_Nullable)metadata {
    NSLog(@"A picture has been taken: %@", image);
    if (image != NULL) {
        [self savePictoreOnDisk:image];
    }
}

- (void)uploadingStop {
    NSLog(@"Uploading picture stop");
}

- (void)uploadingPicture:(NSInteger)pictureID of:(NSInteger)maxID andThumbnail:(UIImage *_Nullable)image {
    NSLog(@"Uploading picture");
}

- (void)callTheGuest:(NSString *)callURL {
    NSLog(@"Calling the guest");
    [self notifyListener:GUEST_READY_EVENT_RECEIVED data:NULL];
    [self showLocalCallNotification:callURL];
}

#pragma mark - Functions declared

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

- (void)generateCallURL:(CDVInvokedUrlCommand*)command
{
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        NSString *invId = [args objectAtIndex:0];
        [self _fetchUseCases:^(BOOL success, NSString *msg, NSArray<NSObject<LSMAUsecase> *> *usecaselist) {
            if (success) {
                [self.lsUniversal.agentHandler createInvitationForUsecase:(LSMAGuestUsecase*)[usecaselist objectAtIndex:0]  usingSuffix:invId andNotify:^(BOOL didSucceed, NSString * _Nullable invite) {
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

- (void)handleCallLocalNotification:(CDVInvokedUrlCommand*)command {
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        NSDictionary *userInfo = [args objectAtIndex:0];
        [self notifyListener:CALL_ACCEPTED_EVENT_RECEIVED data:userInfo];
    }];
}

- (void)revokeInvitation:(CDVInvokedUrlCommand*)command {
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        NSString *invId = [args objectAtIndex:0];
        [self.lsUniversal.agentHandler cancelInvitationOfSuffix: invId];
    }];
}

/** Other utility methods
 **/
- (void)savePictoreOnDisk: (UIImage *_Nullable) image {
    NSData *imageData = UIImagePNGRepresentation(image);
    
    NSString* tempDirectoryPath = NSTemporaryDirectory();
    
    long long now = (long long)([[NSDate date] timeIntervalSince1970] * 1000.0);
    NSString *imagePath =[tempDirectoryPath stringByAppendingPathComponent:[NSString stringWithFormat:@"%lld.png", now]];
    
    NSLog(@"pre writing to file");
    if (![imageData writeToFile:imagePath atomically:NO])
    {
        NSLog(@"Failed to cache image data to disk");
    }
    else
    {
        NSNumber *fileSize = [NSNumber numberWithDouble:[imageData length]];
        NSLog(@"Sightcall call image path is %@ and size %@", imagePath, fileSize);
        [self notifyListener:MEDIA_EVENT_RECEIVED data:@{ @"filePath": imagePath, @"size": fileSize }];
    }
}

- (void) registerCallNotificationCategory {
    if (SYSTEM_VERSION_GRATERTHAN_OR_EQUALTO(@"10.0")) {
        UNUserNotificationCenter *center = [UNUserNotificationCenter currentNotificationCenter];
        center.delegate = self;
        // Register the notification categories.
        UNNotificationCategory* callCategory = [CallLocalNotification getUNNNotificationCategory];
        [center setNotificationCategories:[NSSet setWithObjects:callCategory,
                                           nil]];
    } else {
        NSLog(@"iOS version, lower than iOS 10, not supported for this action %@", @"registerCallNotificationCategory");
    }
}

- (void)showLocalCallNotification: (NSString *)callUrl  {
    [self registerCallNotificationCategory];
    if (SYSTEM_VERSION_GRATERTHAN_OR_EQUALTO(@"10.0")) {
        UNUserNotificationCenter *center = [UNUserNotificationCenter currentNotificationCenter];
        UNMutableNotificationContent* content = [CallLocalNotification buildCallNotificationContent: callUrl];
        
        UNTimeIntervalNotificationTrigger *trigger = [UNTimeIntervalNotificationTrigger triggerWithTimeInterval:1 repeats:false];
        UNNotificationRequest* request = [UNNotificationRequest
                                          requestWithIdentifier:@"SIGHTCALL_CALL_ALARM" content:content trigger:trigger];
        [center addNotificationRequest:request withCompletionHandler:^(NSError * _Nullable error) {
            if (error != nil) {
                NSLog(@"%@", error.localizedDescription);
                [self removeLocalCallNotification];
            }
        }];
    } else {
        NSLog(@"iOS version, lower than iOS 10, not supported for this action %@", @"showLocalCallNotification");
    }
}

/** Removes the local call notification if the call has not been taken after 20 seconds
 **/
- (void) removeLocalCallNotification {
    // Delay execution of my block for 20 seconds.
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 10 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
        UNUserNotificationCenter *center = [UNUserNotificationCenter currentNotificationCenter];
        [center removeDeliveredNotificationsWithIdentifiers:@[@"SIGHTCALL_CALL_ALARM"]];
    });
}

- (BOOL)notifyListener:(NSString *)eventType data:(NSDictionary *)data {
    if (!self.listenerCallbackID) {
        NSLog(@"Listener callback unavailable.  event %@", eventType);
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

#pragma mark PushKit Delegate Methods

- (void)pushRegistry:(PKPushRegistry *)registry didUpdatePushCredentials:(PKPushCredentials *)credentials forType:(NSString *)type{
    if([credentials.token length] == 0) {
        NSLog(@"voip token NULL");
        return;
    }
    NSLog(@"PushCredentials: %@", credentials.token);
    NSString *deviceTokenString = [[[[credentials.token description]
                                     stringByReplacingOccurrencesOfString: @"<" withString: @""]
                                    stringByReplacingOccurrencesOfString: @">" withString: @""]
                                   stringByReplacingOccurrencesOfString: @" " withString: @""];
    [self.lsUniversal.agentHandler setNotificationToken:deviceTokenString];
}

- (void)pushRegistry:(PKPushRegistry *)registry didReceiveIncomingPushWithPayload:(PKPushPayload *)payload forType:(NSString *)type
{
    NSLog(@"Received VoIP push notification");
    if ([self.lsUniversal canHandleNotification:payload.dictionaryPayload]) {
        [self.lsUniversal handleNotification:payload.dictionaryPayload];
    }
}

#pragma mark UNUserNotificationCenterDelegate

//Called when a notification is delivered to a foreground app.
-(void)userNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions options))completionHandler{
    NSLog(@"User Info : %@",notification.request.content.userInfo);
    completionHandler(UNAuthorizationOptionSound | UNAuthorizationOptionAlert | UNAuthorizationOptionBadge);
}

//Called to let your app know which action was selected by the user for a given notification.
-(void)userNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void(^)())completionHandler{
    NSLog(@"User Info : %@",response.notification.request.content.userInfo);
    if ([response.notification.request.content.categoryIdentifier isEqualToString:CallLocalNotificationCategory]) {
        // Handle actions
        if ([response.actionIdentifier isEqualToString:CallLocalNotificationAcceptActionID])
        {
            [self notifyListener:CALL_ACCEPTED_EVENT_RECEIVED data:response.notification.request.content.userInfo];
        }
    }
    completionHandler();
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

