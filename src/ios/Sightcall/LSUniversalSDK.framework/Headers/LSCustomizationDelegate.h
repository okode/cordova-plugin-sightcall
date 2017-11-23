//
//  LSCustomization.h
//  LSUniversalSDK
//
//  Created by Charles Thierry on 21/03/17.
//  Copyright © 2017 SightCall. All rights reserved.
//

#import <LSUniversalSDK/LSUniversalSDK.h>

/**
 *  The customization delegate allows to change a few things in the LSUniversal callViewController's view.
 *  The button customization allows for changing the buttons look and content. Their size is computed by the SDK. The content is dependant on the button's state. See each buttons callback for details.
 */
@protocol LSCustomizationDelegate <NSObject>
@optional

/**
 * @param b The LSButton describing the mute status of the app.
 */
- (void)customizeMuteToggle:(UIButton *)b;

/**
 *  Tapping this button stops/starts the video out stream capture.
 *  b.state == UIControlStateNormal - The video is being sent
 *  b.state == UIControlStateSelected - The video is not being sent
 *  @param b The button to customize
 */
- (void)customizeCameraToggle:(UIButton *)b;

/**
 * Tapping this button changes the video out source.
 *  b.state == UIControlStateNormal - Using the front camera
 *  b.state == UIControlStateSelected - Using another camera
 *
 *  @param b The button to customize
 */
- (void)customizeCameraSource:(UIButton *)b;

/**
 *  Tapping that button changes the audio route used to play/record the audio, if possible.
 *  b.state == UIControlStateNormal - Using the loudspeaker
 *  b.state == UIControlStateSelected - Not using the loudspeaker
 *  This button changes automatically when connecting a headset with a mic.
 *
 *  @param b The button to customize
 */
- (void)customizeSpeakerRoute:(UIButton *)b;

/**
 *  This button pauses/resumes the video out stream capture.
 *  b.state == UIControlStateNormal - VideoOut is playing
 *  b.state == UIControlStateSelected - VideoOut is paused
 *
 *  @param b The button to customize
 */
- (void)customizeVideoPauseToggle:(UIButton *)b;

/**
 *  Tapping that button will display the media selector popup.
 *
 *  @param b The button to customize
 */
- (void)customizeShareMedia:(UIButton *)b;

/**
 *  Tapping this button starts/stops the torch, when using the back camera. It does nothing if using the front camera.
 *  b.state == UIControlStateNormal - The torch is not on
 *  b.state == UIControlStateSelected - The torch is on
 *
 *  @param b The button to customize
 */
- (void)customizeTorchToggle:(UIButton *)b;

/**
 *  Tapping this button will erase any drawings (on share, video out or player)
 *  @param b The button to customize
 */
- (void)customizeEraseDrawings:(UIButton *)b;
/**
 *  Tapping this button will end the ongoing call.
 *
 *  @param b The button to customize
 */
- (void)customizeHangup:(UIButton *)b;
/**
 *  Tapping this button will stop an ongoing share out.
 *
 *  @param b The button to customize
 */
- (void)customizeStopShare:(UIButton *)b;

@end
