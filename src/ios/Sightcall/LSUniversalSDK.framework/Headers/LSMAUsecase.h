//
//  LSMAUsecase.h
//  LSUniversalSDK
//

#import <Foundation/Foundation.h>
#import "LSMATypes.h"

NS_ASSUME_NONNULL_BEGIN

@protocol LSMAUsecase <NSObject>

@property (nonatomic, readonly, nullable) NSString *name;

@end


/**
 * Describes a usecase used to generate pincode to invite a guest.
 */
@interface LSMAGuestUsecase: NSObject <LSMAUsecase>

- (nullable instancetype)init NS_UNAVAILABLE;

/** Which modes are available (at least Call) */
@property (nonatomic, readonly) LSMAUsecaseMode_t availableModes;

/** Which mode is the default */
@property (nonatomic, readonly) LSMAUsecaseMode_t defaultMode;

/** Which media are supported to send a pincode */
@property (nonatomic, readonly) LSMAUsecaseMedia_t availableMedia;

/** Which media is the default to send a pincode */
@property (nonatomic, readonly) LSMAUsecaseMedia_t defaultMedia;

/** Is the pincode auto-deleted after the call */
@property (nonatomic, readonly) BOOL autoDelete;

/** Is a reference needed to create a pincode */
@property (nonatomic, readonly) BOOL needReference;

@end

NS_ASSUME_NONNULL_END
