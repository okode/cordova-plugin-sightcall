//
//  KSMATypes.h
//  LSUniversalSDK
//
//  Created by Charles Thierry on 30/03/17.
//
//

#import <Foundation/Foundation.h>

/**
 *  The registration status of the module
 */
typedef NS_ENUM(NSInteger, RegistrationStatus_t) {
	/**
	 *  No login/password couple found in memory
	 */
	registrationStatus_notRegistered,
	/**
	 *  The registration request was sent and waiting for answer, or we are waiting for a notification token.
	 */
	registrationStatus_registering,
	/**
	 *  A login/pass is available.
	 */
	registrationStatus_registered,
	/**
	 *  The sign in request was sent
	 */
	registrationStatus_signingIn,
	/**
	 *  The sign in was done
	 */
	registrationStatus_signedIn,
	/**
	 *  Fetching usecase
	 */
	registrationStatus_fetchingUsecases,
	/**
	 *  Usecase are available
	 */
	registrationStatus_fetchedUsecases,
};

/**
 *  This error is sent in the registration block
 */
typedef NS_ENUM(NSInteger, RegistrationError_t) {
	/**
	 *  No error occured
	 */
	registrationError_none,
	/**
	 *  The apple notification token is not available. Setting a token will start the registration
	 */
	registrationError_notificationTokenUnavailable,
	/**
	 *  Either the pin code or the token provided were invalid
	 */
	registrationError_invalidTokenOrPin,
	/**
	 *  The App attempted a sign in but no credentials are available.
	 */
	registrationError_noCredentials,
	/**
	 * The App registered, attempted to sign in and failed.
	 */
	registrationError_invalidCredentials,
	/**
	 *  There is no network available
	 */
	registrationError_noNetwork,
};


