declare module SightCall {
    interface SightCallPlugin {
        demo(): void;
        /**
         * It enables SDK logger for debug purpose.
         * Only available on Android.
         * @param enabled
         */
        enableLogger(enabled: boolean): void;
        /**
         * It allows you to change the environment to PPR(pre-production) if you want.
         * Only available on Android.
         * @param environmentKey
         */
        setEnvironment(environmentKey: string): void;
        /**
         * The method configures the Apple Push Notification token in Sight Call's SDK.
         * Only available on iOS.
         * @param token
         */
        setNotificationToken(token: string): void;
        isAgentAvailable(): Promise<void>;
        registerAgent(token: string, pin: string): Promise<void>;
        fetchUseCases(): Promise<void>;
        invite(phoneNumber: string): Promise<void>;
        generateURL(): Promise<string>;
        /**
         * The method checks if the payload belongs to Sight Call and
         * if it has been received because the guest is ready to start the call.
         * Only available on Android.
         * @param payload Push payload
         */
        isGuestReadyPush(payload: any): boolean;
        /**
         * The method checks if the payload belongs to Sight Call and
         * if it has been received because the guest is ready to start the call.
         * Only available on iOS.
         * @param payload Push payload
         */
        canHandleNotification(payload: any): Promise<void>;
        /**
         * The method provides the push payload received as parameter to SDK so that
         * the SDK can perform some action if needed.
         * Only available on iOS.
         * @param payload Push payload
         */
        handleNotification(payload: any): void;
        startCall(url: string): void;
    }
    interface StatusEvent {
        status: string;
    }
    interface CallEndEvent {
        endReason: string;
        duration: number;
        activeDuration: number;
    }
}

declare var sightcall: SightCall.SightCallPlugin;
