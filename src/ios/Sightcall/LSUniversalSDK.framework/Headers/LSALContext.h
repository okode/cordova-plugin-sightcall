//
//  LSActivityLog.h
//  LSUniversalSDK
//
//  Created by Charles Thierry on 09/05/2017.
//  Copyright © 2017 SightCall. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LSActivityEntry.h"

/**
 * Set an object conforming to this protocol as the LSALContext.delegate, and it will be notified when there is a change in the entry list.
 */
@protocol LSALContextDelegate <NSObject>
/**
 * A change took place. Call getAllEntries to get an updated version of the activity list.
 */
- (void)entriesUpdate;


@end

/**
 * The context is used to create a list of entries and clear the dictionary.
 */
@protocol LSALContext <NSObject>

/**
 *  The object can't be simply initialized.
 */
- (_Nullable instancetype) init NS_UNAVAILABLE;

//MARK: Get activities currently stored.
/**
 * Get a readonly list of entries.
 */
- (NSArray <id <LSActivityEntry>> * _Nullable) getAllEntries;


//MARK: Log Clean
/**
 * Cleans the activity log. Removing the entries is an async job. The delegate is triggered once the removal is complete.
 */
- (void)removeAllEntries;


/**
 * This delegate is notified every time a change takes place.
 */
@property (nonatomic, weak, nullable) id<LSALContextDelegate> delegate;

@end
