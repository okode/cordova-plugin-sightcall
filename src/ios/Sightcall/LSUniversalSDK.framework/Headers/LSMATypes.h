//
//  LSMATypes.h
//  LSUniversalSDK
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSUInteger, LSMAUsecaseMode_t)
{
    LSMAUsecaseMode_None =  0,
    LSMAUsecaseMode_Call = 1 << 0,
    LSMAUsecaseMode_Appointment = 1 << 1
};

typedef NS_OPTIONS(NSUInteger, LSMAUsecaseMedia_t)
{
    LSMAUsecaseMedia_None = 0,
    LSMAUsecaseMedia_SMS = 1 << 0,
    LSMAUsecaseMedia_Email = 1 << 1,
    LSMAUsecaseMedia_URL = 1 << 2
};


typedef NS_ENUM(NSUInteger, LSMATemporaryTokenStatus_t)
{
    LSMATemporaryTokenStatus_Sent,
    LSMATemporaryTokenStatus_Error
};

/**
 * Information sent through the registration block when attempting registration
 */
typedef NS_ENUM(NSUInteger, LSMARegistrationStatus_t)
{
    /**
     * Registration took place correctly
     */
    LSMARegistrationStatus_registered,
    /**
     * An unspecified error occured
     */
    LSMARegistrationStatus_notSpecified,
    /**
     * An error occured on the connection to the back while attempting registration
     */
    LSMARegistrationStatus_backError,
    /**
     * The temporary token used to attempt registration is invalid or expired
     */
    LSMARegistrationStatus_invalidTemporaryToken,
    /**
     * The notification reference is missing
     */
    LSMARegistrationStatus_missingReference,
    /**
     * The APNs token was not set before registration
     */
    LSMARegistrationStatus_missingAPNToken
} ;

typedef NS_ENUM(NSUInteger,LSMAModelStatus_t)
{
    LSMAModelStatus_notRegistered,
    LSMAModelStatus_Registering,
    LSMAModelStatus_Registered,
    LSMAModelStatus_Unregistering
} ;

typedef NS_ENUM(NSUInteger, LSMAPincodeStatus_t)
{
    LSMAPincodeStatus_unknown,
    LSMAPincodeStatus_created,
    LSMAPincodeStatus_resent,
    LSMAPincodeStatus_error,
    LSMAPincodeStatus_cancelled
} ;

typedef NS_ENUM(NSUInteger, LSMARegistrationRequestStatus_t)
{
    LSMARegistrationRequestStatus_sent,
    LSMARegistrationRequestStatus_notSpecified = 1 ,
    LSMARegistrationRequestStatus_backError = LSMARegistrationRequestStatus_notSpecified << 1,
    LSMARegistrationRequestStatus_invalidFirstName = LSMARegistrationRequestStatus_notSpecified << 2,
    LSMARegistrationRequestStatus_invalidEmail = LSMARegistrationRequestStatus_notSpecified << 3,
    LSMARegistrationRequestStatus_invalidLastName = LSMARegistrationRequestStatus_notSpecified << 4,
    LSMARegistrationRequestStatus_invalidCompanyName = LSMARegistrationRequestStatus_notSpecified << 5,
    LSMARegistrationRequestStatus_invalidWorldRegion = LSMARegistrationRequestStatus_notSpecified << 6
}  ;


typedef NS_ENUM(NSUInteger, LSMATestNotification_t)
{
    LSMATestNotification_sent,
    LSMATestNotification_error
} ;


typedef NS_ENUM(NSUInteger, LSMACapabities_t)
{
    LSMACapabities_response,
    LSMACapabities_cameraStarted,
    LSMACapabities_cameraStopped,
    LSMACapabities_arStarted,
    LSMACapabities_arStopped,
    LSMACapabities_pauseStart,
    LSMACapabities_pauseStop,
    LSMACapabities_snapshotStart,
    LSMACapabities_snapshotStop,
    LSMACapabities_galleryStart,
    LSMACapabities_galleryStop,
    LSMACapabities_cameraSwitch,
    LSMACapabities_screencastStart,
    LSMACapabities_cobrowsingStart,
    LSMACapabities_cobrowsingStop,
    LSMACapabities_colimatorStart,
    LSMACapabities_colimatorStop,
    LSMACapabities_takeocr,
    LSMACapabities_advancedAnnotationStart,
    LSMACapabities_agent_screencastStart,
    LSMACapabities_agent_screencastStop
};

/**
 *  The registration block that is fired by registerWith...:andReference:onSignIn:
 * If the registration correctly took place, tokenID is the permanent identification token and t is LSMARegistrationStatus_registered.
 * Otherwise tokenID is nil and t is a description of the error that occured.
 */
typedef void (^registrationBlock)(LSMARegistrationStatus_t t, NSString *_Nullable tokenID);

/**
 *  The block that is called once the usecases are fetched.
 *
 *  @param statusCode    The HTTP Status code for the request (2XX for success, failure otherwise)
 *  @param usecases      The Array that contains the usecases
 */
typedef void (^usecasesBlock)(NSInteger statusCode, NSArray *_Nullable usecases);

/**
 *  The block that is called once the usecases are fetched.
 *
 *  @param statusCode    The HTTP Status code for the request (2XX for success, failure otherwise)
 */
typedef void (^notificationBlock)(NSInteger statusCode);

/**
 *  The prototype of the block that is called after the invite is generated
 *  @param didSucceed    YES If the invitation could be generated
 *  @param invite            if didSucceed, the invitation link, else nil
 */
typedef void (^invitationBlock)(BOOL didSucceed, NSString *_Nullable invite);

NS_ASSUME_NONNULL_END
