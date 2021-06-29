//
//  LSMAAgentIdentity.h
//  LSUniversalSDK
//

#import <Foundation/Foundation.h>
#import <LSUniversalSDK/LSMAUsecase.h>
#import <LSUniversalSDK/OfflineIncludedIdentity.h>
#import <LSUniversalSDK/LiveOCR.h>

NS_ASSUME_NONNULL_BEGIN

@class LiveARCodeModel, OCRIntegrationModel;

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


@property (nonatomic, readonly) NSString *urlAnnotation;

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

/**
 live OCR teacher use case number
 */
@property (nonatomic, readonly) int liveOCRTeacher;

@property (nonatomic, readonly) NSArray<NSDictionary *> *offlineMedia;

@property (nonatomic, readonly) NSArray<NSDictionary *> *arOverlayMedia;

@property (nonatomic, readonly) NSArray<NSDictionary *> *ocrList;

@property (nonatomic) NSMutableArray<OfflineIncludedIdentity *> *offlineMediaModels;

/**
 * Get a specific offline media model using its ID
 */
- (OfflineIncludedIdentity *_Nullable)offlineModelForID:(NSString*)modelId;

@property (nonatomic) NSMutableArray<LiveARCodeModel *> *liveARModels;

@property (nonatomic) NSMutableArray<LiveOCR *> *liveOCRModels;

/**
 * Get a specific offline media model using its ID
 */
- (LiveOCR *_Nullable)liveOCRModelForID:(NSString*)modelId;

@property (nonatomic) NSMutableArray<OCRIntegrationModel *> *integrations;

@end

NS_ASSUME_NONNULL_END
