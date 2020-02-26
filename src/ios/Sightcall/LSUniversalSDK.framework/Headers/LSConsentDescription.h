//
//  LSConsentDescription.h
//  LSUniversalSDK
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 *  This object describe the content of the consent message to be displayed. The consent message
 *  for guest is to be displayed either before starting a guest call or while doing a `fetchIdentity:` if agent.
 *
 *  This object is provided by the delegate's `displayConsentWithDescription:` callback.
 */
@protocol LSConsentDescription <NSObject>

/**
 *  Title of the consent popup
 */
@property (readonly, nonatomic, nullable) NSString *title;

/**
 *  Consent message
 */
@property (readonly, nonatomic, nullable) NSString *message;

/**
 *  Label of the `Agree` button
 */
@property (readonly, nonatomic, nullable) NSString *agreeLabel;

/**
 *  Label of the `Disagree` button
 */
@property (readonly, nonatomic, nullable) NSString *cancelLabel;

/**
 *  Label of the `Open EULA` button
 */
@property (readonly, nonatomic, nullable) NSString *eulaLabel;

/**
 *  URL to the EULA
 */
@property (readonly, nonatomic, nullable) NSString *eulaURL;

/**
 *  Block to trigger when the user agrees or disagrees with the consent form.
 */
@property (readonly, nonatomic, nullable) void (^consent)(BOOL);

@end


NS_ASSUME_NONNULL_END
