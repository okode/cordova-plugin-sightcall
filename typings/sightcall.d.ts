declare module SightCall {
    interface SightCallPlugin {
        demo(): void;
        enableLogger(enabled: boolean): void;
        setEnvironment(environmentKey: string): void;
        isAgentAvailable(): Promise<void>;
        registerAgent(token: string, pin: string): Promise<void>;
        fetchUseCases(): Promise<void>;
        invite(phoneNumber: string): Promise<void>;
        generateURL(): Promise<string>;
        isGuestReadyPush(payload: any): boolean;
        startCall(payload: any): void;
    }
}

declare var sightcall: SightCall.SightCallPlugin;
