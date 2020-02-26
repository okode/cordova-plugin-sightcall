//
//  LSCustomizationDelegate.h
//  LSUniversalSDK
//

#import <LSUniversalSDK/LSUniversalSDK.h>

NS_ASSUME_NONNULL_BEGIN

@protocol LSCustomizableContent <NSObject>

@property (nonatomic) UIColor *enabledBackgroundColor;

@property (nonatomic) UIColor *disabledBackgroundColor;

@property (nonatomic) UIColor *borderColor;

@property (nonatomic) CGFloat borderWidth;

- (void)setImage:(nullable UIImage *)image forState:(UIControlState)state;

@end

/**
 *  The customization delegate allows to change a few things in the LSUniversal callViewController's view.
 *  The button customization allows for changing the buttons look and content. Their size is computed by the SDK. The content is dependant on the button's state. See each buttons callback for details.
 */
@protocol LSLocalCustomizationDelegate <NSObject>
@optional

/**
 * @param b The LSButton describing the mute status of the app.
 */
- (void)customizeMuteToggle:(UIButton <LSCustomizableContent> *)b;

/**
 *  Tapping this button stops/starts the video out stream capture.
 *  b.state == UIControlStateNormal - The video is being sent
 *  b.state == UIControlStateSelected - The video is not being sent
 *  @param b The button to customize
 */
- (void)customizeCameraToggle:(UIButton <LSCustomizableContent> *)b;

/**
 * Tapping this button changes the video out source.
 *  b.state == UIControlStateNormal - Using the front camera
 *  b.state == UIControlStateSelected - Using another camera
 *
 *  @param b The button to customize
 */
- (void)customizeCameraSource:(UIButton <LSCustomizableContent> *)b;

/**
 *  Tapping that button changes the audio route used to play/record the audio, if possible.
 *  b.state == UIControlStateNormal - Using the loudspeaker
 *  b.state == UIControlStateSelected - Not using the loudspeaker
 *  This button changes automatically when connecting a headset with a mic.
 *
 *  @param b The button to customize
 */
- (void)customizeSpeakerRoute:(UIButton <LSCustomizableContent> *)b;

/**
 *  This button pauses/resumes the video out stream capture.
 *  b.state == UIControlStateNormal - VideoOut is playing
 *  b.state == UIControlStateSelected - VideoOut is paused
 *
 *  @param b The button to customize
 */
- (void)customizeVideoPauseToggle:(UIButton <LSCustomizableContent> *)b;

/**
 *  Tapping that button will display the media selector popup.
 *
 *  @param b The button to customize
 */
- (void)customizeShareMedia:(UIButton <LSCustomizableContent> *)b;

/**
 *  Tapping that button will stop any outgoing share.
 *
 *  @param b The button to customize
 */
- (void)customizeStopShareMedia:(UIButton <LSCustomizableContent> *)b;

/**
 *  Tapping this button starts/stops the torch, when using the back camera. It does nothing if using the front camera.
 *  b.state == UIControlStateNormal - The torch is not on
 *  b.state == UIControlStateSelected - The torch is on
 *
 *  @param b The button to customize
 */
- (void)customizeTorchToggle:(UIButton <LSCustomizableContent> *)b;

/**
 *  Tapping this button will erase any drawings (on share, video out or player)
 *  @param b The button to customize
 */
- (void)customizeEraseDrawings:(UIButton <LSCustomizableContent> *)b;
/**
 *  Tapping this button will end the ongoing call.
 *
 *  @param b The button to customize
 */
- (void)customizeHangup:(UIButton <LSCustomizableContent> *)b;

/**
 *  Tapping this button will display the chat.
 *
 *  @param b The button to customize
 */
- (void)customizeChat:(UIButton <LSCustomizableContent> *)b;

/**
 *  Tapping this button displays the Control Center's Screen Recording
 */
- (void)customizeScreencast:(UIButton <LSCustomizableContent> *)b;

@end

@protocol LSCustomizationDelegate <LSLocalCustomizationDelegate>

@end

@protocol LSRemoteCustomizationDelegate <NSObject>

@optional

- (void)customizeMuteToggle:(UIButton <LSCustomizableContent> *)b;
- (void)customizeCameraToggle:(UIButton <LSCustomizableContent> *)b;
- (void)customizeCameraSource:(UIButton <LSCustomizableContent> *)b;
- (void)customizeSpeakerSource:(UIButton <LSCustomizableContent> *)b;
- (void)customizeVideoPauseToggle:(UIButton <LSCustomizableContent> *)b;
- (void)customizeShareMediaMenu:(UIButton <LSCustomizableContent> *)b;
- (void)customizeSharePicture:(UIButton <LSCustomizableContent> *)b;
- (void)customizeSharePhoto:(UIButton <LSCustomizableContent> *)b;
- (void)customizeStopShareMedia:(UIButton <LSCustomizableContent> *)b;
- (void)customizeTorchToggle:(UIButton <LSCustomizableContent> *)b;
- (void)customizeErase:(UIButton <LSCustomizableContent> *)b;
- (void)customizeLocation:(UIButton <LSCustomizableContent> *)b;
- (void)customizeSnapshot:(UIButton <LSCustomizableContent> *)b;
- (void)customizeSavePicture:(UIButton <LSCustomizableContent> *)b;
- (void)customizeScreencast:(UIButton <LSCustomizableContent> *)b;

@end


NS_ASSUME_NONNULL_END
