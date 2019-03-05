//
//  AppDelegate+Sightcall.h
//  APPCOL
//
//  Created by Rubén Panadero Navarrete on 28/02/2019.
//

#import "AppDelegate.h"
#import "CDVSightcall.h"

@interface AppDelegate (Sightcall)
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken;
@end
