//
//  LSPictureUpload.h
//  LSUniversalSDK
//
//  Created by Charles Thierry on 23/01/17.
//  Copyright © 2017 SightCall. All rights reserved.
//

#import <LSUniversalSDK/LSUniversalSDK.h>

/**
 Implement this delegate to be notified of picture uploads. Pictures are uploaded sequentially and stored using an internal Core Data scheme.
 Uploading takes place at the end of the call if any pictures were saved during the call and only in Agent mode. Uploading may take place upon
 SDK initialization if any picture were left from a previous call.
 
 No assumption to be made on the threads used to call any of the delegate's method.
 */
@protocol LSPictureUpload <NSObject>

/**
 This method is called when all uploading are stopped. Attempting to start a call will see this method triggered before the call connection starts.
 */
- (void)uploadingStop;

/**
 
 This method informs you that a picture is being uploaded, how many pictures were to be uploaded and how many of them are left. It also provides the app with 
 a thumbnail of the picture. uploadingStart is called before this method is
 called, and this method is called whenever a new picture is being uploaded.
 
 @param pictureID The ID of the picture. Pictures may be uploaded in a different order than they are taken.
 @param maxID The number of pictures to be uploaded.
 @param image A thumbnail of the picture.
 
 */
- (void)uploadingPicture:(NSInteger)pictureID of:(NSInteger)maxID andThumbnail:(UIImage *)image;

@end
