//
//  LSMAAgentIdentity.h
//  LSUniversalSDK
//

#import <Foundation/Foundation.h>
#import "LSMAUsecase.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * The Agent identity describes the agent information stored in the backend.
 */
@interface LSMAAgentIdentity: NSObject

/**
 * The login of the agent
 */
@property (nonatomic, readonly) NSString *login;

/**
 * The agent's displayname
 */
@property (nonatomic, readonly) NSString *displayName;

@property (nonatomic, readonly) NSString *email;

@property (nonatomic, readonly) NSString *phoneNumber;

@property (nonatomic, readonly) NSString *device;

@property (nonatomic, readonly) NSString *osVersion;

/**
 * The reference used to send notifications
 */
@property (nonatomic, readonly) NSString *reference;

/**
 * All the usecases that can be used to send a pincode
 */
@property (nonatomic, readonly) NSArray <NSObject <LSMAUsecase> *> *pincodeUsecases;

/**
 * Get a specific usecase using its ID
 */
- (NSObject <LSMAUsecase> *_Nullable)usecaseForID:(NSInteger)usecaseID;

/**
 * The APNs token used by the backend to send notifications
 */
@property (nonatomic, readonly) NSString *apnstoken;

/**
 * Date at which the agent was registered.
 */
@property (nonatomic, readonly) NSDate *registrationDate;

/**
 * Can agent.startACDCallWithAdditionalReference: be used ?
 */
@property (nonatomic, readonly) BOOL canCallACD;

@end

NS_ASSUME_NONNULL_END
