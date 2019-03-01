//
//  LSMAHandlerProtocol+Register.h
//  LSUniversalSDK
//

#import "LSMAHandlerProtocol.h"
#import "LSMAAgentIdentity.h"
#import "LSConsentDescription.h"


typedef NS_ENUM(NSUInteger, LSMAWorldRegion_t) {
    LSMAWorldRegion_notspecified = 0,
    LSMAWorldRegion_apac,
    LSMAWorldRegion_emea,
    LSMAWorldRegion_america,
};

NS_ASSUME_NONNULL_BEGIN

@protocol LSMAHandlerRegister <LSMAHandler>

@property (nonatomic, nullable, readonly) LSMAAgentIdentity *identity;

@property (nonatomic, nullable, readonly) NSObject <LSConsentDescription> *consent;

- (void)clearCredentials;

/**
 *  Register with the provided data.
 *  The register needs a device/app APN token. If no notification token is set at the time of registration, the block is fired with NO.
 *
 *  While registering, the Mobile Agent Handler triggers the LSUniversal's delegate `connectionEvent:` with `lsConnectionStatus_agent...` parameters.
 *
 *  @param code                 The pin code used to register.
 *  @param referenceID          The reference used to identify the App. Set in the admin portal.
 *  @param registrationBlock    This block is fired on registration success or failure.
 */
- (void)registerWithCode:(nonnull NSString *)code
            andReference:(nonnull NSString *)referenceID
                onSignIn:(nullable registrationBlock)registrationBlock;


/**
 * Register an agent with an URL provided to the App.
 * The APN token must be set before calling this method.
 * @param url                   The URL received. The URL is formatted as ://<host>/register/<pincode>
 * @param referenceID           The reference used to identify the App. Set in the admin portal.
 * @param registrationBlock     This block is fired on registration success or failure.
 */
- (void)registerWithURL:(nonnull NSString *)url
            andReference:(nonnull NSString *)referenceID
                onSignIn:(nullable registrationBlock)registrationBlock;


/**
 * Request a registration link for an already created agent, either by mail or SMS.
 * @param recipient             The email or the phone number used to register the agent
 * @param mediaType             The media (SMS or Email) used for recipient
 * @param block                 The block is fired once the back end answers
 */

- (void)sendRegistrationCodeTo:(nonnull NSString *)recipient
                    usingMedia:(LSMAUsecaseMedia_t)mediaType
                    andExecute:(nullable void (^)(LSMATemporaryTokenStatus_t))block;

/**
 * Request the same registration link to be resent. This method can only be called once.
 * @param block                 The block is fired once the back end answers
 */
- (void)resendRegistrationCodeAndExecute:(nullable void (^)(LSMATemporaryTokenStatus_t))block;

/**
 * This method refreshes the content of self.identity. Call it once registered.
 * @param block                 In case of success, the block is fired after self.identity is updated.
 */
- (void)fetchIdentity:(void(^_Nullable)(BOOL success))block;


- (void)requestRegistrationForCompany:(NSString *_Nonnull)companyName
                        userFirstname:(NSString *_Nonnull)firstname
                         userLastname:(NSString *_Nonnull)lastname
                            userEmail:(NSString *_Nonnull)email
                       andWorldRegion:(LSMAWorldRegion_t)worldRegion
                           andExecute:(void(^_Nullable)(LSMARegistrationRequestStatus_t))block;

@end

NS_ASSUME_NONNULL_END
