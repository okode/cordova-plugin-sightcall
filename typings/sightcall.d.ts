declare module SightCall {
    interface SightCallPlugin {
        demo(): void;
        enableLogger(enabled: boolean): void;
        setEnvironment(environmentKey: string);
        registerAgent(token: string, pin: string): Promise<void>;
        fetchUseCases(): Promise<void>;
        invite(phoneNumber: string): Promise<void>;
        isSightCallPush(payload: any): Promise<void>;
        startCallFromPush(payload: any): void;
    }
}

declare var sightcall: SightCall.SightCallPlugin;
