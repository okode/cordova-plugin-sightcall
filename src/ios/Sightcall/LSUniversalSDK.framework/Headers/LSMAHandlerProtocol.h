//
//  LSMAHandlerProtocol.h
//  LSUniversalSDK
//
//  Created by Charles Thierry on 30/03/17.
//
//

#import <Foundation/Foundation.h>
#import "LSMAUsecase.h"
#import "LSMATypes.h"


/**
 *  The registration block that is fired in registerWithPin:andToken:onRegistration:
 *
 *  @param success		YES if the registration did succeed
 *  @param statusCode	The HTTP Status code for the request
 *  @param status			If a problem occured, set to explain the error
 */
typedef void (^registrationBlock)(BOOL success, NSInteger statusCode, RegistrationError_t status);

/**
 *  The block that is called once the usecases are fetched.
 *
 *  @param statusCode	The HTTP Status code for the request
 *  @param usecases		The Array that contains the usecases
 */
typedef void (^usecasesBlock)(NSInteger statusCode, NSArray *_Nullable usecases);

/**
 *  The block that is called once the usecases are fetched.
 *
 *  @param statusCode	The HTTP Status code for the request
 */
typedef void (^notificationBlock)(NSInteger statusCode);

/**
 *  The prototype of the block that is called after the invite is generated
 *  @param didSucceed	YES If the invitation could be generated
 *  @param invite			if didSucceed, the invitation link, else nil
 */
typedef void (^invitationBlock)(BOOL didSucceed, NSString *_Nullable invite);

/**
 * @brief The Mobile Agent handler is instanciated as LSUniversal's `agentHandler`. If credentials are available, the LSMAHandler's status is moved to registered.
 * @ discussion Upon first fetch of the usecases, the first connection is attempted. It may fail if there are no network, the servers are otherwise non responsive, or if the credentials are invalid.
 * In the last case, the currently stored credentials are removed and the SDK moves to unregistered. Any other failure type leaves the SDK in registered.
 *
 * To invite a Guest, an Agent can either create an Invitation using the createInvitationForUsecase:usingSuffix:andNotify: method or directly send an invitation by using the sendNotifcationForUsecase:... methods.
 * The first method will rely on the App to distribute the invitation (you will receive a NSString to share with the Guest). The second method uses our backend to send either an email (sendNotifcationForUsecase:toEmail:...) or a SMS (sendNotifcationForUsecase:ToPhone:...) to the Guest.
 * The string received by the Guest can be provided to the Universal SDK (startWithString:) to start the call. The
 */
@protocol LSMAHandler <NSObject>

/**
 * If YES, the agent is registered and signed in.
 */
@property (nonatomic, readonly, getter=isAvailable) BOOL available;

/**
 * If YES, Agent credentials are available (regardless of isAvailable).
 */
@property (nonatomic, readonly) BOOL hasCredentials;

/**
 *  Register with the provided data. 
 *  The register needs a device/app APN token. If no notification token is set at the time of registration, the block is fired with NO. Setting the notificationToken after calling this method 
 *  automatically triggers a new attempt.
 *
 *  While registering, the Mobile Agent Handler triggers the LSUniversal's delegate `connectionEvent:` with `lsConnectionStatus_agent...` parameters.
 *
 *  @param pin		The pin code used to register.
 *  @param token	The token needed to register (it is not the notificationToken).
 *  @param block	This block is fired on registration success or failure.
 */
- (void)registerWithPin:(nonnull NSString *)pin andToken:(nonnull NSString *)token onSignIn:(nonnull registrationBlock)block;

/**
 * Either call immediately the block if the token is valid, or fetches a new one from the backend if the one set is invalidated.
 * @param block	This block is fired when the connection token is updated.
 */
- (void)fetchConnectionToken:(nonnull void(^)(BOOL, NSString * _Nullable ))block;

/**
 *	 The application notification token. It must be set for the registration to work.
 */
@property (nonatomic, nullable) NSString *notificationToken;

/**
 *  The list of usecase. This list is only populated after a call to refreshUsecase:
 */
@property (nonatomic, readonly, nullable)NSArray<NSObject<LSMAUsecase> *> *usecases;

/**
 *  Fetches the list of usecase from the backend.
 */
- (void)fetchUsecases:(nonnull void(^)(BOOL, NSArray<NSObject<LSMAUsecase> *> * _Nullable))usecaseList;

/**
 *  Removes the credentials from the memory
 */
- (void)clearCredentials;

/**
 *  Sends an invite to a Guest. This invite will be sent as a sms. 
 *  @discussion To accept this invite, the Guest needs to call the sendGuestNotificatioForHash: method.
 *
 *  @param usecase		The usecase
 *  @param phoneNumber	The phone number. Either in international format (ex +33601234567) or national format and we'll try to guess the country code.
 *  @param contactName	The name of the contact
 *  @param block			The notification block
 */
- (void)sendNotificationForUsecase:(nullable LSMAGuestUsecase *)usecase toPhone:(nonnull NSString *)phoneNumber andDisplayName:(nullable NSString *)contactName andNotify:(nullable notificationBlock)block;

/**
 *  Sends an invite to a Guest. This invite will be sent as an email.
 *  @discussion To accept this invite, the Guest needs to call the sendGuestNotificatioForHash: method.
 *
 *  @param usecase		The usecase
 *  @param email			The email to send the notification to
 *  @param contactName	The name of the contact
 *  @param block			The notification block
 */
- (void)sendNotificationForUsecase:(nullable LSMAGuestUsecase *)usecase toEmail:(nonnull NSString *)email andDisplayName:(nullable NSString *)contactName andNotify:(nullable notificationBlock)block;


/**
 *  Sends an invite to a registered Agent. This invite will be sent as an APN/FCM notification.
 *  @discussion After sending the notification, the USDK status will change to calling. The call will start when the invitee Agent connects. The SDK will wait 120s for this to happen. 
 *  The agent receiving the invite starts the call by starting the USDK using the URL string in the invite. 
 *
 *  @param usecase		The usecase used to invite the agent
 *  @param agentID		The login used by the agent to invite
 *  @param block			This block will be fired once the invite is sent.
 */
- (void)sendNotificationForUsecase:(nullable LSMAAgentUsecase *)usecase toAgent:(nonnull NSString *)agentID andNotify:(nullable notificationBlock)block;

/**
 * Request an invitation URL. This string is then sent by the Agent to the Guest. This URL is returned in the block. Use this method to invite Guest without using the sendNotificationForUsecase:... methods.
 * @param usecase	The usecase object containing the information to be used
 * @param suffix	The suffix to use in the invitation
 * @param block	The block to be executed after the URL is received or in case of error.
 */
- (void)createInvitationForUsecase:(nullable id<LSMAUsecase>)usecase usingSuffix:(nonnull NSString *)suffix andNotify:(nullable invitationBlock)block; 

/**
 * Cancel an invitation based on the suffix. An invitation created through createInvitationForUsecase:usingSuffix:andNotify: should be cancelled.
 *	@param suffix	The suffix used to create the invitation.
 */
- (void)cancelInvitationOfSuffix:(nonnull NSString *)suffix;

@end
