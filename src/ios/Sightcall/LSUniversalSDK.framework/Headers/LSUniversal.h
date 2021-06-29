//
//  LSUniversal.h
//  LSUniversalSDK
//

#import <LSUniversalSDK/LSUniversalSDK.h>
#import <LSUniversalSDK/OfflineIncludedIdentity.h>
#import <LSUniversalSDK/LiveOCR.h>
#import <LSUniversalSDK/LSCustomizationDelegate.h>
#import <LSUniversalSDK/LSPictureProtocol.h>

NS_ASSUME_NONNULL_BEGIN

/**
 The LSUniversal object is your point of entry to the connection and call control. The Callflow is simplified to the max:

 - Start the connection using parameters;
 - When the connection goes active, the call is created;
 - When the call ends, you are disconnected.

 Starting can be done through two methods startWithDictionary: or startWithString: . The latter makes use of the former.

 The call internals are controlled through the UI (turning off the camera, muting the mic. etc). As said before, the call is started upon connection.

 You are notified of the connection state through the delegate property. You can check the status of the connection anytime by calling currentConnectionStatus.

 You can call abort during connection or during a call. It will either directly disconnect the LSUniversalSDK or hangup the call, which will in turn disconnect the LSUniversalSDK. On your end, calls can be ended either trough this abort method or the hangup button in the call view UI. You are notified of call ends by the [LSUniversalDelegate callReport:].
 
 Upon init, the agent Handler instance is initialized and attemps to sign in if agent credentials are available.
 
 */
@interface LSUniversal : NSObject

/**
 *  A UIViewController that is used by the SDK to display the video in and out, as well as the buttons used for controlling the call. Display it when the currentConnectionStatus goes to active. Remove it on call end. Available only when the LSUniversalSDK status goes to lsConnectionStatus_callActive.
 *  @sa [LSUniversalDelegate connectionEvent:]
 *  @sa [LSUniversalDelegate callReport:]
 */
@property(nonatomic, readonly, nullable) UIViewController *callViewController;

/**
 *  The delegate that is notified of connection events and call ends.
 */
@property(nonatomic, weak, nullable) id<LSUniversalDelegate> delegate;

/**
 *  If the delegate conforms to LSUniversalLogDelegate, this property is set automatically
 */
@property(nonatomic, weak, nullable) id<LSUniversalLogDelegate> logDelegate;

/**
 *  This delegate is called upon when the call menu is resized.
 */
@property(nonatomic, weak, nullable) NSObject<LSCustomizationDelegate> *customizationDelegate __attribute__((deprecated("Please use the localBarCustomizationDelegate.")));

/**
 * The customization delegate for the local bar.
 * The local bar controls the call from the user's end.
 */
@property(nonatomic, weak, nullable) NSObject<LSLocalCustomizationDelegate> *localBarCustomizationDelegate;

/**
 * The customization delegate for the remote bar.
 * The remote bar is only available when the user is an agent.
 */
@property(nonatomic, weak, nullable) NSObject<LSRemoteCustomizationDelegate> *remoteBarCustomizationDelegate;


/**
 *  The current connection status. When the connection goes from connecting to active, the call is created.
 */
@property(nonatomic, readonly) lsConnectionStatus_t status;

/**
 *  This object is responsible for sending the notifications to a remote contact (e.g. in the Agent-to-Guest case). It needs the Apple Notification token to work (see APNS).
 *  This object is available after the init is done, until destruction of the LSUniversal instance.
 */
@property(nonatomic, readonly, nullable) NSObject<LSMAHandler, LSMAHandlerPincode, LSMAHandlerRegister> *agentHandler;


/**
 *  The picture upload delegate is notified when uploading is started, stopped and when a picture is uploading, along with some information regarding the picture being uploaded.
 */
@property(nonatomic, weak, nullable) NSObject <LSPictureProtocol> *pictureDelegate;

@end

@interface LSUniversal (Start)
/**
 *  Connects the LSUniversalSDK to SightCall's cloud. The dictionary is a <String *: String *> dictionary, with the key being URL Scheme parameters and the values their value.
 *
 *  @param dictionary The key/value used to connect
 *
 *  @sa startWithString:
 *  @sa abort
 */
- (void)startWithDictionary:(NSDictionary *)dictionary;

/**
 *  Connects the LSUniversalSDK to SightCall's cloud. From this string is made a dictionary used in startWithDictionary:
 *
 *  @param param The string used to create the dictionary. It can be an URL string such as the one used in the Universal App (i.e. "sightcall://?mode=value&..."), or simply "mode=value&...". `?`, `=` and `&` are used as separators.
 *
 *  @sa startWithDictionary:
 *  @sa abort
 */
- (void)startWithString:(NSString *)param;

/**
 *  Connects the LSUniversalSDK to SightCall's cloud.
 *
 *  @param param The string used to create the dictionary. It can be an URL string such as the one used in the Universal App (i.e. "sightcall://?mode=value&..."), or simply "mode=value&...". `?`, `=` and `&` are used as separators.
 *  @param wss YES if the sdk should connect with websocket secure. Websocket secure is much slower but works with AppClips
 *
 *  @sa startWithString:
 */
- (void)startWithString:(NSString *)param useWebSocketSecure:(BOOL)wss;

/**
 *  Disconnects the LSUniversalSDK from the cloud. Hangup the call if any.
 *  @sa startWithString:
 *  @sa startWithDictionary:
 */
- (void)abort;

-(void) uploadUHDInBackground;

@end

@interface LSUniversal (Notifications)

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


@interface LSUniversal (OCR)

/**
 */
- (UIViewController*)getLiveOCRVC;


/**
 */
- (UIViewController*)getOCRTeacherVC;


/**
 */
- (UIViewController *)getOfflineUseCaseVC;

/**
 */
- (UIViewController *)getARLiveCodeVC;

- (UIViewController *) getSingleUploadOfflineVC:(OfflineIncludedIdentity *) offlineMedia;

- (UIViewController *) getSingleUploadOfflineVC:(OfflineIncludedIdentity *) offlineMedia withReference:(nullable NSString*)reference objectId:(nullable NSString*)objectId;

-(void) goToLiveOCR:(LiveOCR *) liveOCRModel completion:(void(^)(UIViewController *viewController))block;

@end

NS_ASSUME_NONNULL_END

