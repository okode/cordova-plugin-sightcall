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
 */
typedef void (^fetchUseCasesBlock)(BOOL success, NSString *msg);

// Events
NSString *const EVENT_TYPE = @"eventType";
NSString *const EVENT_DATA = @"eventData";
NSString *const CALL_REPORT_EVENT_RECEIVED = @"sightcall.callreportevent";
NSString *const STATUS_EVENT_RECEIVED = @"sightcall.statusevent";
NSString *const MEDIA_EVENT_RECEIVED = @"sightcall.mediaevent";
NSString *const GUEST_READY_EVENT_RECEIVED = @"sightcall.guestready";
NSString *const CALL_START_EVENT_RECEIVED = @"sightcall.callstart";

// UNIVERSAL STATUS
NSString *const IDLE_STATUS = @"IDLE";
NSString *const CONNECTING_STATUS = @"CONNECTING";
NSString *const ACTIVE_STATUS = @"ACTIVE";
NSString *const DISCONNECTING_STATUS = @"DISCONNECTING";
NSString *const CONNECTION_LOST = @"CONNECTION_LOST";

// UNIVERSAL END REASON
NSString *const END_FOR_UNEXPECTED_ERROR = @"UNEXPECTED";
NSString *const END_LOCAL = @"LOCAL";
NSString *const END_REMOTE = @"REMOTE";

@implementation CDVSightCall

@synthesize ignoreUsecaseConfiguration;

static CDVSightCall *instance;
BOOL isLoggerEnabled = FALSE;

#pragma mark - Plugin Initialization

- (void)pluginInitialize
{
    [self initSightcall];
    instance = self;
    self.ignoreUsecaseConfiguration = TRUE;
}

- (void)initSightcall {
    self.lsUniversal = [[LSUniversal alloc] init];
    self.lsUniversal.delegate = self;
    [self.lsUniversal setPictureDelegate: self];
    self.lsUniversal.logDelegate = self;
}

- (void)registerListener:(CDVInvokedUrlCommand *)command {
    self.listenerCallbackID = command.callbackId;
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
        case lsConnectionStatus_networkLoss: {
            statusData = CONNECTION_LOST;
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
        [self savePictoreOnDisk:image with: metadata.caseID];
    }
}

- (void)uploadingStop {
    NSLog(@"Uploading picture stop");
}

- (void)uploadingPicture:(NSInteger)pictureID of:(NSInteger)maxID andThumbnail:(UIImage *_Nullable)image {
    NSLog(@"Uploading picture");
}

- (void)guestAcceptedCall:(nullable void(^)(BOOL))userResponse {
    NSLog(@"The guest is calling");
    userResponse(true);
}

#pragma mark - Plugin functions

- (void)demo:(CDVInvokedUrlCommand*)command
{
    NSLog(@"SightCall: demo function invoked");
}

- (void)enableLogger:(CDVInvokedUrlCommand *)command
{
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        isLoggerEnabled = [args objectAtIndex:0];
    }];
}

- (void)setEnvironment:(CDVInvokedUrlCommand *)command {
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        NSString *env = [[args objectAtIndex:0] lowercaseString];
        [[NSUserDefaults standardUserDefaults] setObject:env forKey:@"kStorePlatform"];
        [self initSightcall];
    }];
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
        NSString *token = [args objectAtIndex:0];
        NSString *apnsReference = [args objectAtIndex:1];
        if (token == NULL || apnsReference == NULL) {
            completionHandler(CDVCommandStatus_ERROR, @"Error, token or APNS reference param is NULL");
        }
        [self.lsUniversal.agentHandler registerWithCode:token andReference:apnsReference onSignIn:^(LSMARegistrationStatus_t t, NSString * _Nullable tokenID) {
            if (t == LSMARegistrationStatus_registered) {
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
        handler(false, @"Register the agent before");
        return;
    }
    [self.lsUniversal.agentHandler fetchIdentity:^(BOOL success) {
        handler(success, NULL);
    }];
}

- (void)fetchUseCases:(CDVInvokedUrlCommand*)command
{
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        [self _fetchUseCases:^(BOOL success, NSString *msg) {
            if (success) {
                completionHandler(CDVCommandStatus_OK, @"Use cases fetched successfully");
            } else {
                completionHandler(CDVCommandStatus_ERROR,  [NSString stringWithFormat:@"Error fetching use cases. Reason: %@", msg]);
            }
        }];
    }];
}

- (void)generateCallURL:(CDVInvokedUrlCommand*)command
{
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        NSString *reference = [args objectAtIndex:0];
        [self _fetchUseCases:^(BOOL success, NSString *msg) {
            if (success) {
                LSMAGuestUsecase *usecase = [self getFirstUsecase:[self.lsUniversal.agentHandler.identity pincodeUsecases]];
                if (usecase == NULL) {
                    completionHandler(CDVCommandStatus_ERROR, @"No use cases configured for the agent");
                    return;
                }
                [self.lsUniversal.agentHandler createInvitationForUsecase: usecase withReference:reference andNotify:^(LSMAPincodeStatus_t status, NSString * _Nullable inviteURL) {
                    if (status == LSMAPincodeStatus_created) {
                        NSString* callId = [self getCallId:inviteURL];
                        completionHandler(CDVCommandStatus_OK, @{ @"url": inviteURL, @"callId": callId });
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

#pragma mark - Plugin instance functions

+ (void)setNotificationDeviceToken:(NSString *)token {
    CDVSightCall *plugin = instance;
    if (plugin == NULL) { return; }
    [plugin.lsUniversal.agentHandler setNotificationToken: token];
}

+ (void)handleSightcallPush:(NSDictionary *)userInfo {
    CDVSightCall *plugin = instance;
    if (plugin == NULL) { return; }
    NSDictionary* guestReadyPayload = userInfo != NULL ? userInfo[@"guest-ready"] : NULL;
    if ([plugin.lsUniversal canHandleNotification:userInfo] && guestReadyPayload != NULL) {
        NSObject *pinCode = [NSNull null];
        NSObject *caseReportId = [NSNull null];
        if (guestReadyPayload != NULL) {
            pinCode = guestReadyPayload[@"pincode"];
            caseReportId = guestReadyPayload[@"case-report-id"];
        }
        [plugin notifyListener:GUEST_READY_EVENT_RECEIVED data:@{ @"callId": pinCode, @"caseReportId": caseReportId }];
        [plugin showLocalCallNotification:userInfo];
    }
}

/** On Android, it works in this way, so we have implemented the solution equally.
 **/
- (LSMAGuestUsecase*)getFirstUsecase:(NSArray<NSObject<LSMAUsecase> *>*)usecaseList {
    if (usecaseList == NULL || [usecaseList count] == 0) { return NULL; }
    return (LSMAGuestUsecase*)[usecaseList objectAtIndex:0];
}

- (void)startCall:(CDVInvokedUrlCommand*)command
{
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        NSString *url = [args objectAtIndex:0];
        [self.lsUniversal startWithString:url];
        NSString *callId = [self getCallId:url];
        if (callId != NULL) {
            [self notifyListener:CALL_START_EVENT_RECEIVED data:@{ @"callId":  callId}];
        }
    }];
}

- (NSString*) getCallId:(NSString*) url {
    NSURLComponents *parsedUrl = [NSURLComponents componentsWithString:url];
    NSArray *queryItems = parsedUrl.queryItems;
    if (queryItems == NULL) { return NULL; }
    for (NSURLQueryItem *queryItem in queryItems) {
        if ([queryItem.name isEqualToString:@"pin"]) {
            return queryItem.value;
        }
    }
    return NULL;
}

- (void)handleCallLocalNotification:(CDVInvokedUrlCommand*)command {
    [self performCallbackWithCommand:command withBlock:^(NSArray *args, CordovaCompletionHandler completionHandler) {
        [self handleCallLocalNotificationWithSightcallPayload: [args objectAtIndex:0]];
    }];
}

/** Other utility methods
 **/
- (void)savePictoreOnDisk:(UIImage *_Nullable)image with:(int32_t)caseId {
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
        NSString *caseReportId = [NSString stringWithFormat:@"%d", caseId];
        [self notifyListener:MEDIA_EVENT_RECEIVED data:@{ @"filePath": imagePath, @"size": fileSize, @"caseReportId": caseReportId }];
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

- (void)showLocalCallNotification:(NSDictionary*)sightcallPushPayload  {
    [self registerCallNotificationCategory];
    if (SYSTEM_VERSION_GRATERTHAN_OR_EQUALTO(@"10.0")) {
        UNUserNotificationCenter *center = [UNUserNotificationCenter currentNotificationCenter];
        UNMutableNotificationContent* content = [CallLocalNotification buildCallNotificationContent: sightcallPushPayload];
        
        UNTimeIntervalNotificationTrigger *trigger = [UNTimeIntervalNotificationTrigger triggerWithTimeInterval:1 repeats:false];
        UNNotificationRequest* request = [UNNotificationRequest
                                          requestWithIdentifier:@"SIGHTCALL_CALL_ALARM" content:content trigger:trigger];
        [center addNotificationRequest:request withCompletionHandler:^(NSError * _Nullable error) {
            if (error != nil) {
                NSLog(@"%@", error.localizedDescription);
            }
            [self removeLocalCallNotification];
        }];
    } else {
        NSLog(@"iOS version, lower than iOS 10, not supported for this action %@", @"showLocalCallNotification");
    }
}

/** Removes the local call notification if the call has not been taken after 20 seconds
 **/
- (void) removeLocalCallNotification {
    // Delay execution of my block for 20 seconds.
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 40 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
        UNUserNotificationCenter *center = [UNUserNotificationCenter currentNotificationCenter];
        [center removeDeliveredNotificationsWithIdentifiers:@[@"SIGHTCALL_CALL_ALARM"]];
    });
}

- (void) handleCallLocalNotificationWithSightcallPayload:(NSDictionary *)userInfo {
    if (userInfo == NULL) {
        NSLog(@"Tried to handle an empty call local notification");
        return;
    }
    NSDictionary *sightcallPushPayload = userInfo[@"sightcallPushPayload"];
    if (sightcallPushPayload != NULL) {
        NSString *callId = [sightcallPushPayload valueForKeyPath:@"guest-ready.pincode"];
        [self notifyListener:CALL_START_EVENT_RECEIVED data:@{ @"callId":  callId}];
        [self.lsUniversal handleNotification:sightcallPushPayload];
    }
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
            [self handleCallLocalNotificationWithSightcallPayload: response.notification.request.content.userInfo];
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

#pragma mark Logger

//this delegate method is called when a log line is emitted by the SDK
- (void)logLevel:(NSInteger)level logModule:(NSInteger)module fromMethod:(NSString *)originalSel message:(NSString *)message, ...;
{
    if (isLoggerEnabled) {
        va_list pe;
        va_start(pe, message);
        NSString *sMessage = [[NSString alloc] initWithFormat:message arguments:pe];
        va_end(pe);
        NSLog(@"%@ %@", originalSel, sMessage);
    }
}

@end
#endif
