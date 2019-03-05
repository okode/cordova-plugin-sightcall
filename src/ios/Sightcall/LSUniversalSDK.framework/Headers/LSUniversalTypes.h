//
//  LSUniversalType.h
//  LSUniversalSDK
//

#import <LSUniversalSDK/LSUniversalSDK.h>

NS_ASSUME_NONNULL_BEGIN

/** @name Connection parameters ID keys.*/
extern NSString *const kParametersMode;
extern NSString *const kParametersHash;
extern NSString *const kParametersAppID;
extern NSString *const kParametersACDProduct;
extern NSString *const kParametersACDLanguage;
extern NSString *const kParametersACDLocation;

extern NSString *const kParametersACDInfo;
extern NSString *const kParametersCalleeID;
extern NSString *const kParametersButtons;
extern NSString *const kParametersSharedURL;
extern NSString *const kParametersVideoOut;
extern NSString *const kParametersUsingVideoOut;
extern NSString *const kParametersVideoFull;
extern NSString *const kParametersVideoSmall;
extern NSString *const kParametersVideoPtrOut;
extern NSString *const kParametersAudioMute;
extern NSString *const kParametersPinCode;
extern NSString *const kParametersInternalToken;
extern NSString *const kParametersTokenLivesight;
extern NSString *const kParametersExternalToken;
extern NSString *const kParametersVideoProfile;
extern NSString *const kParametersUID;
extern NSString *const kParametersSuffix;
extern NSString *const kParametersDisplayName;
extern NSString *const kParametersSentText;
extern NSString *const kParametersMPID;
extern NSString *const kParametersMPHash;
extern NSString *const kParametersTimeout;


typedef NS_ENUM(NSInteger, lsConnectionStatus_t) {
    lsConnectionStatus_idle,
    /**
     *  The LSUniversalSDK is connected as agent.
     */
    lsConnectionStatus_agentConnected,
    /**
     *  The LSUniversalSDK is registering an agent.
     */
    lsConnectionStatus_agentRegistering,
    /**
     *  The LSUniversalSDK is unregistering as agent.
     */
    lsConnectionStatus_agentUnregistering,
    /**
     *  The LSUniversalSDK is connecting.
     */
    lsConnectionStatus_connecting,
    /**
     *  The LSUniversalSDK is connected. Depending on the authentication process, a call is being created.
     */
    lsConnectionStatus_active,
    /**
     *  A call was started.
     */
    lsConnectionStatus_calling,
    /**
     *  A call is active.
     */
    lsConnectionStatus_callActive,
    /**
     *  The LSUniversalSDK is disconnecting.
     */
    lsConnectionStatus_disconnecting,
    /*
     *  The connection was lost.
     */
    lsConnectionStatus_networkLoss
};

/**
 *  When a connection error occurs, you are notified of it through connectionError:
 */
typedef NS_ENUM(NSInteger, lsConnectionError_t) {
	/**
	 * The error was a network one
	 */
	lsConnectionError_networkError,
	/**
	 *  The credentials given in the dictionary (or string) were incorrect.
	 */
	lsConnectionError_badCredentials,
	/**
	 *  An unknown error occured.
	 */
	lsConnectionError_unknown
} ;

/**
 *  The call ended, the LSUniversalSDK is disconnecting.
 */
typedef NS_ENUM(NSInteger, lsCallEnd_t) {
    /**
	 *  The call ended prematuraly
	 */
	lsCallEnd_unexpected,
	/**
	 *  The call was ended by the remote client
	 */
	lsCallEnd_remote,
	/**
	 *  the call was ended on your end.
	 */
	lsCallEnd_local,
    /**
     *
     */
    lsCallEnd_eulaRefused
} ;

/**
 * This structure describes the condition of the call end
 */
typedef struct {
    /**
     * Reason the call ended
     */
	lsCallEnd_t callEnd;
    /**
     * Duration of the call
     */
	NSTimeInterval callLength;
} lsCallReport_s;


/**
 * ACD Queue status.
 */
typedef struct {
	/**
	 * Position in the queue.
	 */
	NSInteger position;
	/**
	 * Total length of the queue.
	 */
	NSInteger length;
} LSACDProgress_s;


/**
 * ACD Status.
 */
typedef NS_ENUM(NSInteger, LSACDStatus_t) {
	/**
	 */
	acdStatus_invalid = -1,
	/**
	 * Waiting for an agent to pick up.
	 */
	acdStatus_ongoing,
	/**
	 * The service is closed.
	 */
	acdStatus_serviceClosed,
	/**
	 * The service is unavailable.
	 */
	acdStatus_serviceUnavailable,
	/**
	 * There is no agent available.
	 */
	acdStatus_agentUnavailable
} ;


/**
 * The ACD status structure. `status` and `progress` are independant: there can be an ETA without any information about the queue, and you can be in a queue without any information about an ETA.
 */
typedef struct {
	/**
	 * Current status of your ACD request.
	 */
	LSACDStatus_t status;
	/**
	 * Current info about your position in the queue.
	 */
	LSACDProgress_s progress;
	/**
	 * ETA for an agent to accept your call.
	 */
	NSInteger waitingTime;
} LSACDQueue_s;


NS_ASSUME_NONNULL_END
