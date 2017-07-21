//
//  LSUniversal+Mirror.h
//  LSUniversalSDK
//
//  Created by Charles Thierry on 29/05/2017.
//  Copyright © 2017 SightCall. All rights reserved.
//

#import <LSUniversalSDK/LSUniversalSDK.h>

/**
 * Describes how the test ended.
 */
typedef NS_ENUM(NSInteger, LSQoSStop_t)
{
	LSQoSStop_finished,
	LSQoSStop_forced,
	LSQoSStop_error,
};


/**
 * The LSUniversalQoS object is instantiated by the application to test the Quality of the Service. 
 * This test can not take place at the same time as a call.
 */
@interface LSQoSManager: NSObject

/**
 * This method starts a mirror call, used to evaluate the quality of service.
 * This mode uses the back camera as a capture device, and receives the same stream from the cloud.
 * The call stops after a set time (about 30s).
 * @param delegate	The object that is notified of the call progression and QoS evaluation. It is assign to this test session Metrics object.
 * @return YES if the test started, NO if the test could not start (e.g. we are already in call, or the platform could not be contacted.)
 */
- (BOOL)startMirrorCallUsingDelegate:(id<LSQoSDelegate>)delegate;

- (BOOL)startMirrorCallUsingDelegate:(id<LSQoSDelegate>)delegate onPlatform:(NSDictionary *)pfm;

@property (nonatomic, weak) UIView *renderView;

/**
 * This object is instantiated upon starting the test. Its delegate property is set to the delegate parameter of startMirrorCall:usingDelegate:.
 */
@property (nonatomic) LSQoSMetrics *currentSession;

/**
 * Stop the QoS test. The delegate is notified when the actual stop took place.
 */
- (void)stopMirrorCall;

@end
