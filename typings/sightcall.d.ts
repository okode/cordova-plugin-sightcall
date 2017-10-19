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
         * The method checks if the displayed notification is due to an entry Sightcall call
         * Only available on iOS.
         * @param payload Push payload
         */
        isCallLocalNotification(payload: any): Promise<void>;
        /**
         * This method handles the local notification displayed due to an entry call
         * and generates an event (only generated on iOS) to notice that the call has been accepted.
         * This method should be invoked after invoking 'isCallLocalNotification'
         * Only available on iOS.
         * @param payload Push payload
         */
        handleCallLocalNotification(payload: any): void;
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
    interface MediaEvent {
        filePath: string;
        size: number;
    }
    /** iOS event */
    interface CallAcceptedEvent {
        callUrl: string;
    }
}

declare var sightcall: SightCall.SightCallPlugin;