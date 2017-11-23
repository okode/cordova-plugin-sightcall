//
//  LSUniversalQoSMetrics.h
//  LSUniversalSDK
//
//  Created by Charles Thierry on 05/06/2017.
//  Copyright © 2017 SightCall. All rights reserved.
//

#import <LSUniversalSDK/LSUniversalSDK.h>

typedef NS_ENUM(NSInteger, LSQoSMetricStatus_t)
{
	LSQoSMetricStatus_none, //not doing anything
	LSQoSMetricStatus_testStarted, //the test was started
	LSQoSMetricStatus_networkTesting, //the platform test is starting. the network type is determined
	LSQoSMetricStatus_networkTested, //the platform is selected and the RTT is computed
	LSQoSMetricStatus_callCreating, //the call is being created
	LSQoSMetricStatus_callCreated, //the call was created and will start soon
	LSQoSMetricStatus_callStarted, //the call is starting
	LSQoSMetricStatus_callEnded, //the call is ended
	LSQoSMetricStatus_testCompleted, //the test is complete, no error
	LSQoSMetricStatus_testCompletedWithError,  //the test is complete, an error occured
	LSQoSMetricStatus_testCancelling, //the test was cancelled by the user
	LSQoSMetricStatus_statsUploadDone,
	LSQoSMetricStatus_statsUploadError,
};

typedef NS_ENUM(NSInteger, LSQoSEvaluationScore_t)
{
	LSQoSEvaluationScore_notApplicable,
	LSQoSEvaluationScore_bad,
	LSQoSEvaluationScore_medium,
	LSQoSEvaluationScore_good,
};


@class LSQoSMetrics;


typedef struct {
	NSInteger metricID;
	struct{
		NSInteger metric;
		LSQoSEvaluationScore_t score;
	} uplink;
	struct{
		NSInteger metric;
		LSQoSEvaluationScore_t score;
	} downlink;
} LSQoSCallMetrics_s;

@protocol LSQoSDelegate <NSObject>

@property (nonatomic) LSQoSMetrics *callMetric;

- (void)lsqosStatusChange:(LSQoSMetricStatus_t)newStatus;

- (void)lsqosNetworkType:(NSString *)networkName andScore:(LSQoSEvaluationScore_t)score;

- (void)lsqosPlatformSelected:(NSString *)pfmName withRTT:(float)ms andScore:(LSQoSEvaluationScore_t)score;

- (void)lsqosCallMetric:(LSQoSCallMetrics_s)metrics of:(NSInteger)max;

- (void)lsqosVideoSizeChange:(CGSize)newVideoSize;

- (void)lsqosGlobalMetric:(LSQoSEvaluationScore_t)score;

- (void)lsqosStatsSent:(BOOL)success;

@end


@interface LSQoSMetrics: NSObject

@property (nonatomic) LSQoSMetricStatus_t testStatus;

/**
 * Wifi, 4G, 3G, 2G, No Network
 */
@property (nonatomic, readonly) NSString *networkDescription;
/**
 * Wifi and 4G is Good, 3G is Medium, 2G and No Network is bad
 */
@property (nonatomic, readonly) LSQoSEvaluationScore_t networkScore;

/**
 * Name of the selected platform if any
 */
@property (nonatomic, readonly) NSString *platformName;
/**
 * RTT to the selected platform. Lower is better.
 */
@property (nonatomic, readonly) NSInteger platformRTT;
/**
 * Evaluation of the device-to-platform RTT.
 */
@property (nonatomic, readonly) LSQoSEvaluationScore_t platformRTTScore;
/**
 * Evaluation of the device-to-platform link while sending and receiving video. Higher is better.
 */
@property (nonatomic, readonly) NSInteger uplinkValue;
/**
 * A score based on the uplinkValue.
 */
@property (nonatomic, readonly) LSQoSEvaluationScore_t uplinkScore;
/**
 * Evaluation of the platform-to-device link while sending and receiving video. Higher is better.
 */
@property (nonatomic, readonly) NSInteger downlinkValue;

/**
 * A score based on the downlink value.
 */
@property (nonatomic, readonly) LSQoSEvaluationScore_t downlinkScore;

/**
 * A global score based on the network type, RTT, uplink and downlink.
 */
@property (nonatomic, readonly) LSQoSEvaluationScore_t globalScore;

@property (nonatomic, readonly) NSInteger currentCount;

@property (nonatomic, readonly) NSInteger maxCount;

/**
 * During the call part of the test, you can set the renderView (which will display the incoming stream, that is a mirror what you are sending).
 * This size allow you to correctly resize this view.
 */
@property (nonatomic, readonly) CGSize videoSize;

@property (nonatomic, readonly) NSString *shareURL;

@end



