//
//  Model.h
//  RTCC Generic
//
//  Created by Charles Thierry on 19/09/14.
//  Copyright (c) 2014 Charles Thierry. All rights reserved.
//

#import <LSUniversalSDK/LSUniversalSDK.h>

/**
 The LSUniversal object is your point of entry to the connection and call control. The Callflow is simplified to the max:

 - Start the connection using parameters;
 - When the connection goes active, the call is created;
 - When the call ends, you are disconnected.

 Starting can be done through two methods startWithDictionary: or startWithString: . The latter makes use of the former.

 The call internals are controlled through the UI (turning off the camera, muting the mic. etc). As said before, the call is started upon connection.

 You are notified of the connection state through the delegate property. You can check the status of the connection anytime by calling currentConnectionStatus.

 You can call abort during connection or during a call. It will either directly disconnect the LSUniversalSDK or hangup the call, which will in turn disconnect the LSUniversalSDK. On your end, calls can be ended either trough this abort method or the hangup button in the call view UI. You are notified of call ends by the [LSUniversalDelegate callReport:].
 
 Upon init, the self.mobile2mobile instance is initialized and attemps to sign in if agent credentials are available.
 
 */
@interface LSUniversal : NSObject

/**
 *  A UIViewController that is used by the SDK to display the video in and out, as well as the buttons used for controlling the call. Display it when the currentConnectionStatus goes to active. Remove it on call end. Available only when the LSUniversalSDK status goes to lsConnectionStatus_callActive.
 *  @sa [LSUniversalDelegate connectionEvent:]
 *  @sa [LSUniversalDelegate callReport:]
 */
@property(nonatomic, readonly) UIViewController *callViewController;

/**
 *  The delegate that is notified of connection events and call ends.
 */
@property(nonatomic, weak) id<LSUniversalDelegate> delegate;

/**
 *  If the delegate conforms to LSUniversalLogDelegate, this property is set automatically
 */
@property(nonatomic, weak) id<LSUniversalLogDelegate> logDelegate;

/**
 *  This delegate is called upon when the call menu is resized.
 */
@property(nonatomic, weak) NSObject<LSCustomizationDelegate> *customizationDelegate;

/**
 *  The current connection status. When the connection goes from connecting to active, the call is created.
 */
@property(nonatomic, readonly) lsConnectionStatus_t status;

/**
 *  This object is responsible for sending the notifications to a remote contact (e.g. in the Agent-to-Guest case). It needs the Apple Notification token to work (see APNS).
 *  This object is available after the init is done, until destruction of the LSUniversal instance.
 */
@property(nonatomic, readonly) NSObject<LSMAHandler> *agentHandler;


@property(nonatomic, readonly) NSObject<LSALContext> *activityHandler;

/**
 *  The picture upload delegate is notified when uploading is started, stopped and when a picture is uploading, along with some information regarding the picture being uploaded.
 */
@property(nonatomic, weak) NSObject <LSPictureProtocol> *pictureDelegate;


/**
 *  Connects the LSUniversalSDK to SightCall's cloud. The dictionary is a <String *: String *> dictionary, with the key being URL Scheme parameters and the values their value.
 *
 *  @param dictionary The key/value used to connect
 *
 *  @return True if the dictionary allows for connection. If false, something is missing from the dictionary.
 *  @sa startWithString:
 *  @sa abort
 */
- (BOOL)startWithDictionary:(NSDictionary *)dictionary;

/**
 *  Connects the LSUniversalSDK to SightCall's cloud. From this string is made a dictionary used in startWithDictionary:
 *
 *  @param param The string used to create the dictionary. It can be an URL string such as the one used in the Universal App (i.e. "sightcall://?mode=value&..."), or simply "mode=value&...". `?`, `=` and `&` are used as separators.
 *
 *  @return True if the string allows for connection. If false, something is missing from the library
 *  @sa startWithDictionary:
 *  @sa abort
 */
- (BOOL)startWithString:(NSString *)param;

/**
 *  Disconnects the LSUniversalSDK from the cloud. Hangup the call if any.
 *  @sa startWithString:
 *  @sa startWithDictionary:
 */
- (void)abort;

/**
 *  Checks if a PushKit notification received by the application can be dealt with by the SDK
 *  @param notification The notification's payload dictionary.
 *  @return YES if the notification can be handled by the SDK.
 *  @sa handleNotification:
 */
- (BOOL)canHandleNotification:(NSDictionary *)notification;


/**
 *  Handles the notification if canHandleNotification: returned YES
 *  @param notification The Push notification's payload dictionary.
 *  @sa canHandleNotification:
 */
- (void)handleNotification:(NSDictionary *)notification;

@end
