//
//  LSPictureProtocol.h
//  LSUniversalSDK
//

#import <CoreLocation/CoreLocation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * The metadata of the picture
 */
@interface LSPictureMetadata : NSObject

/**
 * If yes, then the GPS location offered by gpsLatitude and gpsLongitude is valid. Otherwise, their value is sensless.
 */
@property (nonatomic) CLLocationCoordinate2D gpsLocation;

/**
 *
 */
@property (nullable, nonatomic, copy) NSDate *captureDate;

/**
 *
 */
@property (nullable, nonatomic, copy) NSString *usecaseID;
/**
 *
 */
@property (nullable, nonatomic, copy) NSString *pictureOrigin;

/**
 *
 */
@property (nonatomic) int64_t callTimeCode;

/**
 *
 */
@property (nonatomic) int32_t caseID;

@end


/**
 Implement this delegate to be notified of picture saves and upload. 
 
 If the usecase to upload the picture Pictures are uploaded sequentially and stored using an internal Core Data scheme.
 Uploading takes place at the end of the call if any pictures were saved during the call and only in Agent mode. Uploading may take place upon
 SDK initialization if any picture were left from a previous call.
 
 No assumption to be made on the threads used to call any of the delegate's method.
 */
@protocol LSPictureProtocol <NSObject>

//MARK: Uploading

/**
 This method is called when all uploading are stopped. Attempting to start a call will see this method triggered before the call connection starts.
 */
- (void)uploadingStop;

/**
 
 This method informs you that a picture is being uploaded, how many pictures were to be uploaded and how many of them are left. It also provides the app with 
 a thumbnail of the picture. uploadingStart is called before this method is called, and this method is called whenever a new picture is being uploaded.
 
 @param pictureID The ID of the picture. Pictures may be uploaded in a different order than they are taken.
 @param maxID The number of pictures to be uploaded.
 @param image A thumbnail of the picture.
 
 */
- (void)uploadingPicture:(NSInteger)pictureID of:(NSInteger)maxID andThumbnail:(UIImage *_Nullable)image;

//MARK: External handling

/**
 * When the usecase is configured to store the picture as local, the picture is handled to the App.
 * @param image		The image that was captured, full resolution
 * @param metadata	The metadata linked to the image.
 * @sa ignoreUsecaseConfiguration
 */
- (void)savedPicture:(UIImage *_Nullable)image andMetadata:(LSPictureMetadata *_Nullable)metadata;

/**
 * If YES, the image is passed to the App to handle the local storage. Otherwise the picture is stored according to the Save picture configuration of the usecase.
 */
@property (nonatomic) BOOL ignoreUsecaseConfiguration;

@end



NS_ASSUME_NONNULL_END
