//
//  LSUniversalDelegate.h
//  LSUniversalSDK
//
//  Created by Charles Thierry on 21/03/17.
//  Copyright © 2017 SightCall. All rights reserved.
//


#import <LSUniversalSDK/LSUniversalSDK.h>

@protocol LSUniversalLogDelegate <NSObject>

- (void)logLevel:(NSInteger)level logModule:(NSInteger)module fromMethod:(NSString *)from message:(NSString *)message, ... NS_REQUIRES_NIL_TERMINATION;

@end



/**
 *  The protocol the LSUniversalDelegate is to follow. Make no assumption regarding the thread that is used to trigger those messages.
 */
@protocol LSUniversalDelegate <NSObject>

/**
 *  A connection event occured. You were not connected, the SDK goes to idle, etc.
 *
 *  @param status The new connection state.
 */
- (void)connectionEvent:(lsConnectionStatus_t)status;

/**
 *  An error occured. The SDK is most likely now disconnecting.
 *
 *  @param error The error that occured.
 */
- (void)connectionError:(lsConnectionError_t)error;

/**
 *  The call ended. You should probably dismiss the Call ViewController here.
 *
 *  @param callEnd The reason the call ended.
 */
- (void)callReport:(lsCallReport_s)callEnd;

@optional

/**
 *  Called with information about the ACD queue.
 *  @param update The ACD queue information
 */
- (void)acdStatusUpdate:(LSACDQueue_s)update;

/**
 *  The user was accepted by an agent using the ACD system. The call begins soon after (i.e. connectionEvent: will soon be called with lsConnectionStatus_calling). This method may not be called.
 *
 *  @param agentUID The UID of the agent that accepted the call.
 *  @sa acdProgressEvent:
 */
- (void)acdAcceptedEvent:(NSString *)agentUID;


- (void)connectionParameters:(NSDictionary *)parameters;
- (void)cameraUsedOnStart:(lsCameraUsedOnStart_t)isFront;

/**
 *  Should this method be called, it will be at the end of a call. It contains all informations needed to open the survey webpage defined in the administration portal.
 *
 *  @param infos The informations needed to open the survey page and optionaly open a popup to prompt the user before that.
 */
- (void)callSurvey:(id<LSSurveyInfos>)infos;

//MARK: Mobile to Mobile

/**
 *  The mobile2mobile object can be used to send notification to other users.
 */
- (void)notificationUsecaseAvailable;

/**
 *  The USDK is registered as an agent and sent an invite to a guest. The guest accepted. Use the URL to start the call.
 *  @param callURL	The url to use to start the call.
 */
- (void)callTheGuest:(NSString *)callURL;

/**
 *  The SDK is registered as an agent and is being asked to join a call to another agent. Use this URL to start the call.
 *  @param callURL	The URL to use to start the call.
 */
- (void)callTheAgent:(NSString *)callURL;

@end
