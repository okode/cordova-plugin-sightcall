//
//  LSSurveyInfosProtocol.h
//  LSUniversalSDK
//
//  Created by Charles Thierry on 21/03/17.
//  Copyright © 2017 SightCall. All rights reserved.
//


#import <LSUniversalSDK/LSUniversalSDK.h>

/**
 *  An object of this type describes the data needed to display the survey at call's end.
 *  If displayPopup, ask the user if she wants to participate in a survey. Otherwise, just use url to open a webbrowser
 */
@protocol LSSurveyInfos <NSObject>
/**
 *  The URL of the survey. If displayPopup, ask the user if she accepts to participate in a survey. Otherwise, just open it in
 *  a webbrowser (e.g. using UIApplication openURL: )
 */
@property(nonatomic, readonly) NSString *url;
/**
 *  If YES, the user is to be presented with an alert view to confirm wether or not they want to go to the survey.
 *  If NO, just open url
 */
@property(nonatomic, readonly) BOOL displayPopup;
/**
 *  If displayPopup, this is the popup title.
 */
@property(nonatomic, readonly) NSString *popupLabel;
/**
 *  If displayPopup, this is the popup text.
 */
@property(nonatomic, readonly) NSString *buttonLabel;

@end

