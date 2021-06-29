//
//  LSQoSMetrics.h
//  LSUniversalSDK
//

#import <LSUniversalSDK/LSUniversalSDK.h>

NS_ASSUME_NONNULL_BEGIN

/**
 Various status of the ongoing test.
 */
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


/**
 * Possible values for the evaluation
 *
 */
typedef NS_ENUM(NSInteger, LSQoSEvaluationScore_t)
{
	LSQoSEvaluationScore_notApplicable,
	LSQoSEvaluationScore_bad,
	LSQoSEvaluationScore_medium,
	LSQoSEvaluationScore_good
};


@class LSQoSMetrics;


/**
 * Structure containing the evaluation and values for a metric
 */
typedef struct {
	NSInteger metricID;
	struct{
		/**
		 * Between 0 and 10, 10 being best
		 */
		NSInteger metric;
		LSQoSEvaluationScore_t score;
	} uplink;
	struct{
		/**
		 * Between 0 and 10, 10 being best
		 */
		NSInteger metric;
		LSQoSEvaluationScore_t score;
	} downlink;
} LSQoSCallMetrics_s;

/**
 * Conform to this protocol to be notified of various event, including the connection status change, the call start and end, the evaluation start and end..
 */
@protocol LSQoSDelegate <NSObject>

/**
 * Metric of the ongoing test
 */
@property (nonatomic) LSQoSMetrics *callMetric;

/**
 * Status change
 * @param newStatus	 The new state assumed by the SDK
 * @sa LSQoSMetrics testStatus
 */
- (void)lsqosStatusChange:(LSQoSMetricStatus_t)newStatus;

/**
 * Fired upon test start, when the SDK has identified which network is being used.
 * @param networkName	The network type (can be WiFi, 2G, 3G, etc)
 * @param score			The score associated to that metric
 */
- (void)lsqosNetworkType:(NSString *)networkName andScore:(LSQoSEvaluationScore_t)score;

/**
 * Fired when the SDK is connecting to the platform.
 * @param pfmName			The name of the platform we are connecting to.
 * @param ms				RTT between the device and the platform
 * @param score			The score associated to that metric
 */
- (void)lsqosPlatformSelected:(NSString *)pfmName withRTT:(float)ms andScore:(LSQoSEvaluationScore_t)score;

/**
 *	Quality of the call over a short period of time.
 * @param metrics			The measurement of the uplink and downlink quality
 * @param max				The total number of measurement that will be taken
 *
 */
- (void)lsqosCallMetric:(LSQoSCallMetrics_s)metrics of:(NSInteger)max;

/**
 * Fired when the video size changes
 * @param newVideoSize	The new size of the video layer
 * @sa LSQoSManager 		renderView
 */
- (void)lsqosVideoSizeChange:(CGSize)newVideoSize;

/**
 *	Fired at the end of the call
 * @param score			Global call evaluation
 */
- (void)lsqosGlobalMetric:(LSQoSEvaluationScore_t)score;

/**
 * At the end of the call, the stats are uploaded.
 * @param success			YES if the stats where uploaded.
 * @sa LSQoSMetrics shareURL
 */
- (void)lsqosStatsSent:(BOOL)success;

@end


@interface LSQoSMetrics: NSObject

/**
 * Current status of the ongoing test.
 */
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

/**
 * Number of time measurement have been taken.
 */
@property (nonatomic, readonly) NSInteger currentCount;

/**
 * Max number of time measurement will be taken.
 */
@property (nonatomic, readonly) NSInteger maxCount;

/**
 * During the call part of the test, you can set the renderView (which will display the incoming stream, that is a mirror what you are sending).
 * This size allow you to correctly resize this view.
 */
@property (nonatomic, readonly) CGSize videoSize;

/**
 * When the test is finished, the shareURL is set.
 */
@property (nonatomic, readonly) NSString *shareURL;

@end


NS_ASSUME_NONNULL_END
