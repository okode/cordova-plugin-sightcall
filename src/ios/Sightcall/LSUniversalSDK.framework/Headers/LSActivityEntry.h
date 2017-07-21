//
//  LSActivity.m
//  LSUniversalSDK
//
//  Created by Charles Thierry on 09/05/2017.
//  Copyright © 2017 SightCall. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 * The Activity type. Allows to determine which protocols the activity follows without using `conformsToProtocol:` .
 */
typedef NS_ENUM(int16_t, LSActivity_t)
{
	/**
	 * A SMS sent from the agent to a guest. This activity will conforms to the LSActivitySMSInvite protocol.
	 */
	LSActivity_smsInvite,
	/**
	 * An email sent from the agent to a guest. This activity will conforms to the LSActivityEmailInvite protocol.
	 */
	LSActivity_emailInvite,
	
	LSActivity_urlInvite,
	/**
	 * This activity describes the upload of a picture taken during a call by the agent. It conforms to the LSActivityPictureUpload protocol
	 */
	LSActivity_uploadPicture,
	/**
	 * A call to a guest. It usually follows an Invite activity. This activity conforms to the LSActivityGuestCall protocol.
	 */
	LSActivity_guestCall,
	/**
	 * A call to another agent. This activity conforms to the LSActivityACDCall protocol.
	 */
	LSActivity_acdCall,
};

/**
 *  The basic activity protocol. No entry is *just* an id<LSActivityEntry>, but rather conforms to one of its subprotocols.
 *  Entries are added when the activity actually took place, i.e. calls activity are set when the call ends, invites are set when the invite was sent or failed, etc.
 */
@protocol LSActivityEntry <NSObject>

/**
 *  This is redundant with the protocol that is actually implemented, but allows for verification and easier writing (using a switch instead of checking protocol conformance).
 */
@property (nonatomic, readonly) LSActivity_t type;

/**
 *  The date at which the activity was created.
 */
@property (nonatomic, readonly, copy) NSDate *eventStart;

/**
 * If yes, the activity was successful. The significance is dependant on the actual protocol the object is conforming to.
 */
@property (nonatomic, readonly) BOOL success;

@end

/**
 *  This describes a SMS invite.
 *  Success means the invite was sent. Failure means the invite could not be sent.
 */
@protocol LSActivitySMSInvite <NSObject, LSActivityEntry>

/**
 * The phone number the SMS is being sent to.
 */
@property (nonatomic, readonly, copy) NSString *phoneNumber;

/**
 * The displayname (if any) linked to the number
 */
@property (nonatomic, readonly, copy) NSString *displayName;

/**
 * The suffix sent to the guest.
 */
@property (nonatomic, readonly, copy) NSString *suffix;

@end

/**
 *  This describes an email invite.
 *  Success means the invite was sent. Failure means the invite could not be sent.
 */
@protocol LSActivityEmailInvite <NSObject, LSActivityEntry>

/**
 * The email address used to send the mail to.
 */
@property (nonatomic, readonly, copy) NSString *email;
/**
 * The displayName (if any) linked to the email
 */
@property (nonatomic, readonly, copy) NSString *displayName;
/**
 * The suffix sent to the guest.
 */
@property (nonatomic, readonly, copy) NSString *suffix;

@end

/**
 * This describes an ACD Call.
 * Success means the call ended up properly. Failure means the call ended with an error.
 */
@protocol LSActivityACDCall <NSObject, LSActivityEntry>

/**
 * The ACD reference.
 */
@property (nonatomic, readonly, copy) NSString *reference;

/**
 * The calls duration.
 */
@property (nonatomic, readonly) NSTimeInterval duration;

@end

/**
 * This describes a Guest Call.
 * Success means the call ended up properly. Failure means the call ended with an error.
 */
@protocol LSActivityGuestCall <NSObject, LSActivityEntry>

/**
 * The invitee UID.
 */
@property (nonatomic, readonly, copy) NSString *invitee;
/**
 * The invitee suffix
 */
@property (nonatomic, readonly, copy) NSString *suffix;
/**
 * The call duration
 */
@property (nonatomic, readonly) NSTimeInterval duration;

@end

@protocol LSActivityPictureUpload <NSObject, LSActivityEntry>

@end


/**
 * This describes an invite shared as an URL
 */
@protocol LSActivityURLInvite <NSObject, LSActivityEntry>

/**
 * The URL that the guest must open to start a call to the Agent.
 */
@property (nonatomic, readonly, copy) NSString *urlString;

/**
 * The suffix sent to the guest.
 */
@property (nonatomic, readonly, copy) NSString *suffix;

@end
