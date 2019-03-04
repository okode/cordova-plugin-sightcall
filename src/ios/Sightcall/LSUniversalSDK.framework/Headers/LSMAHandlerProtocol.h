//
//  LSMAHandlerProtocol.h
//  LSUniversalSDK
//

#import <Foundation/Foundation.h>
#import "LSMAUsecase.h"
#import "LSMATypes.h"

NS_ASSUME_NONNULL_BEGIN

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
 
 */
@property (nonatomic, nullable) NSString *notificationToken;

/**
 
 */
@property (nonatomic, nullable) NSString *notificationReference;



/**
 * Call this method to test if the notification is correctly stored in the backend.
 * @param block A block fired after the call to the backend is done.
 */
- (void)sendTestNotification:(void(^_Nullable)(LSMATestNotification_t))block;

/**
 * Start a call to another agent.
 * @param reference A nullable string, sent to the backend on call start.
 */
- (void)startACDCallWithAdditionalReference:(nullable NSString *)reference;



@end

NS_ASSUME_NONNULL_END
