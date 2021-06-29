//
//  LSMaterialTextField.h
//  LSUniversalSDK
//
//  Created by Ben Maad on 07/04/2020.
//  Copyright © 2020 SightCall. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MaterialComponents/MDCTextInputControllerOutlined.h>

NS_ASSUME_NONNULL_BEGIN

@interface LSMaterialTextField : MDCTextInputControllerOutlined

+ (void)setupTheme:(LSMaterialTextField *) materialTextField;

@end

NS_ASSUME_NONNULL_END
