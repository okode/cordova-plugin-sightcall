package com.okode.cordova.sightcall;

import androidx.annotation.NonNull;
import android.util.Log;

import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.iid.FirebaseInstanceId;
import com.google.firebase.iid.InstanceIdResult;
import com.sightcall.universal.Universal;
import com.sightcall.universal.agent.Code;
import com.sightcall.universal.agent.CreateCode;
import com.sightcall.universal.agent.CreateCodeCallback;
import com.sightcall.universal.agent.FetchUsecasesCallback;
import com.sightcall.universal.agent.RegisterCallback;
import com.sightcall.universal.agent.UniversalAgent;
import com.sightcall.universal.agent.Usecase;
import com.sightcall.universal.agent.registration.OldRegistration;
import com.sightcall.universal.agent.registration.RegistrationRequest;
import com.sightcall.universal.api.Environment;
import com.sightcall.universal.event.CallReportEvent;
import com.sightcall.universal.fcm.messages.GuestReady;
import com.sightcall.universal.media.MediaSavedEvent;
import com.sightcall.universal.scenario.Step;
import com.sightcall.universal.scenario.steps.GuestPincodeCallStep;
import com.sightcall.universal.scenario.steps.HostPincodeCallStep;

import net.rtccloud.sdk.event.Event;
import net.rtccloud.sdk.event.call.StatusEvent;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.PluginResult;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.List;

import static com.okode.cordova.sightcall.Methods.DEMO;
import static com.okode.cordova.sightcall.Methods.ENABLE_LOGGER;
import static com.okode.cordova.sightcall.Methods.FETCH_USE_CASES;
import static com.okode.cordova.sightcall.Methods.GENERATE_URL;
import static com.okode.cordova.sightcall.Methods.IS_AGENT_AVAILABLE;
import static com.okode.cordova.sightcall.Methods.REGISTER_AGENT;
import static com.okode.cordova.sightcall.Methods.REGISTER_LISTENER;
import static com.okode.cordova.sightcall.Methods.SET_ENVIRONMENT;
import static com.okode.cordova.sightcall.Methods.START_CALL;

public class SightCall extends CordovaPlugin {

    private static final String TAG = "SightCallPlugin";
    private static final String URL_PARAM = "url";
    private static final String CALL_ID_PARAM = "callId";

    @Override
    protected void pluginInitialize() {
        Log.i(TAG, "Registering us for Sightcall's events");
        Universal.register(this);
    }

    @Override
    public void onReset() {
        super.onReset();
        EventsManager.instance().setListener(null);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        EventsManager.instance().setListener(null);
    }

    @Event
    public void onGuestReady(GuestReady event) {
        Log.i(TAG, event.toString());
        EventsManager.instance().sendGuestReadyEvent(event.pincode(), event.caseReportId());
    }

    @Event
    public void onCallStatusEvent(StatusEvent event) {
        Log.i(TAG, event.status().toString());
        EventsManager.instance().sendStatusEvent(event);
    }

    @Event
    public void onCallFinished(CallReportEvent event) {
        Log.i(TAG, event.toString());
        EventsManager.instance().sendCallReportEvent(event);
    }

    @Event
    public void onMediaSavedEvent(MediaSavedEvent event) {
        Log.i(TAG, event.toString());
        EventsManager.instance().sendMediaSavedEvent(event);
    }

    @Event
    public void onStepStateEvent(Step.StateEvent event) {
        Log.i(TAG, event.toString());
        if (event.state() == Step.State.ACTIVE) {
            final Step step = event.step();
            if (step instanceof GuestPincodeCallStep) {
                // On the Guest side
                final GuestPincodeCallStep guestStep = (GuestPincodeCallStep) step;
                final String pincode = guestStep.session().config().pin();
                final String caseReportId = guestStep.session().caseReportId();
                EventsManager.instance().sendCallStartEvent(pincode, caseReportId);
            } else if (step instanceof HostPincodeCallStep) {
                // On the agent side
                final HostPincodeCallStep hostStep = (HostPincodeCallStep) step;
                final GuestReady guestReady = hostStep.session().config().guestReady();
                if (guestReady != null) {
                    final String pincode = guestReady.pincode();
                    final String caseReportId = guestReady.caseReportId();
                    EventsManager.instance().sendCallStartEvent(pincode, caseReportId);
                }
            }
        }
    }


    @Override
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
        if (DEMO.equals(action)) {
            this.demo();
            callbackContext.success();
            return true;
        } else if (ENABLE_LOGGER.equals(action)) {
            this.enableLogger(args.optBoolean(0, false));
            callbackContext.success();
            return true;
        } else if (SET_ENVIRONMENT.equals(action)) {
            this.setEnvironment(args.optString(0, Environment.PROD.name()));
            callbackContext.success();
            return true;
        } else if (IS_AGENT_AVAILABLE.equals(action)) {
            this.isAgentAvailable(callbackContext);
            return true;
        } else if (REGISTER_AGENT.equals(action)) {
            this.registerAgent(args.optString(0, null), callbackContext);
            return true;
        } else if (FETCH_USE_CASES.equals(action)) {
            this.fetchUseCases(callbackContext);
            return true;
        } else if (START_CALL.equals(action)) {
            this.startCall(args.optString(0));
            return true;
        } else if (GENERATE_URL.equals(action)) {
            this.generateURL(args.optString(0), callbackContext);
            return true;
        } else if (REGISTER_LISTENER.equals(action)) {
            this.registerListener(callbackContext);
            return true;
        }
        callbackContext.error(action + " is not a supported action");
        return false;
    }

    private void registerListener(final CallbackContext callbackContext) {
        if (callbackContext == null) {
            EventsManager.instance().setListener(null);
            return;
        }
        EventsManager.instance().setListener(new EventListener() {
            @Override
            public void onEvent(com.okode.cordova.sightcall.events.Event event) {
                JSONObject eventData = new JSONObject();

                try {
                    eventData.putOpt(com.okode.cordova.sightcall.events.Event.EVENT_TYPE, event.getEventName());
                    eventData.putOpt(com.okode.cordova.sightcall.events.Event.EVENT_DATA, event.getEventData());
                } catch (JSONException e) {
                    Log.e(Constants.TAG, "Failed to create event: " + event);
                    return;
                }

                PluginResult result = new PluginResult(PluginResult.Status.OK, eventData);
                result.setKeepCallback(true);
                callbackContext.sendPluginResult(result);
            }
        });
    }

    private void demo() {
        Log.i(TAG, "DEMO invoked");
    }

    private void enableLogger(boolean enabled) {
        Universal.settings().loggerVerbosity().set(Log.VERBOSE); // SDK logs will be visible in Logcat
        Universal.settings().apiInterceptor().set(true); // API calls will be visible in Logcat
    }

    /** Allows you to change environment to PRE. If {@param environmentKey} is PPR, the PRE environment will be selected.
     *  Otherwise, the PRE environment will be selected
     *
     * @param environmentKey environmentKey
     */
    private void setEnvironment(String environmentKey) {
        if ( Environment.PPR.name().equalsIgnoreCase(environmentKey)) {
            Log.i(TAG, "PRE environment selected");
            Universal.settings().defaultEnvironment().set(Environment.PPR);
        } else {
            Log.i(TAG, "PRO environment selected");
            Universal.settings().defaultEnvironment().set(Environment.PROD);
        }
    }

    private void isAgentAvailable(final CallbackContext callback) {
        if (Universal.agent().isAvailable()) {
            callback.success("Agent is available");
        } else {
            callback.error("Agent not available");
        }
    }

    private void registerAgent(final String token, final CallbackContext callback) {
        if (token == null) {
            callback.error("Error, temporary token is NULL");
            return;
        }
        getFirebaseToken(callback, new OnSuccessListener<InstanceIdResult>() {
            @Override
            public void onSuccess(final InstanceIdResult result) {
              RegistrationRequest registration = new OldRegistration.Builder(SightCall.this.cordova.getActivity())
                        .temporaryToken(token)
                        .fcmToken(result.getToken())
                        .build();
                Universal.agent().register(registration, new RegisterCallback() {
                    @Override
                    public void onRegisterSuccess(@NonNull RegisterCallback.Success success) {
                        callback.success("Agent registration succeeded");
                    }

                    @Override
                    public void onRegisterError(@NonNull Error error) {
                        callback.error("Agent registration failed");
                    }
                }, true);
            }
        });
    }

    private void getFirebaseToken(final CallbackContext callback,
                                  OnSuccessListener<InstanceIdResult> successListener) {
        FirebaseInstanceId.getInstance().getInstanceId()
                .addOnSuccessListener(successListener)
                .addOnFailureListener(new OnFailureListener() {
                    @Override public void onFailure(@NonNull Exception e) {
                        callback.error("Error getting Firebase token");
                    }
                });
    }

    private void fetchUseCases(final FetchUseCasesCallback callback) {
        if (!Universal.agent().isAvailable()) {
            Log.e(TAG, "Register the agent before");
            callback.onFetchUsecasesFailure("Register the agent before");
            return;
        }
        UniversalAgent agent = Universal.agent();
        agent.fetchUsecases(new FetchUsecasesCallback() {
            @Override public void onFetchUsecasesSuccess(@NonNull FetchUsecasesCallback.Success success) {
                callback.onFetchUsecasesSuccess();
            }
            @Override public void onFetchUsecasesError(@NonNull Error error) {
                callback.onFetchUsecasesFailure("Error fetching use cases");
            }
        });
    }

    private void fetchUseCases(final CallbackContext callback) {
        this.fetchUseCases(new FetchUseCasesCallback() {
            @Override
            public void onFetchUsecasesSuccess() {
                callback.success("Use cases fetched successfully");
            }

            @Override
            public void onFetchUsecasesFailure(String message) {
                callback.error("Error fetching use cases. Reason: " + message);
            }
        });
    }

    private void generateURL(final String referenceId, final CallbackContext callback) {
        this.fetchUseCases(new FetchUseCasesCallback() {
            @Override
            public void onFetchUsecasesSuccess() {
                UniversalAgent agent = Universal.agent();

                List<Usecase> cases = agent.usecases();
                if (cases == null || cases.size() == 0) {
                    callback.error("Error generating URL. No usecases set up for this agent");
                    return;
                }

                Usecase usecase = agent.usecases().get(0);
                CreateCode code = CreateCode.url(usecase).reference(referenceId).build();
                Universal.agent().createCode(code, new CreateCodeCallback() {
                    @Override public void onCreateCodeSuccess(@NonNull Success success) {
                        Code code = success.code();
                        JSONObject res = new JSONObject();
                        try {
                            res.put(URL_PARAM, code.url());
                            res.put(CALL_ID_PARAM, code.value());
                            callback.success(res);
                        } catch (JSONException e) {
                            callback.error("Error building URL invitation response");
                        }
                    }
                    @Override public void onCreateCodeError(@NonNull Error error) {
                        callback.error("Error generating the call URL");
                    }
                });
            }

            @Override
            public void onFetchUsecasesFailure(String message) {
                callback.error("Error generating the call URL. Unexpected error fetching use cases. Reason: " + message);
            }
        });
    }

    private void startCall(String url) {
        Universal.start(url);
    }

}

final class Methods {
    final static String DEMO = "demo";
    final static String ENABLE_LOGGER = "enableLogger";
    final static String SET_ENVIRONMENT = "setEnvironment";
    final static String IS_AGENT_AVAILABLE = "isAgentAvailable";
    final static String REGISTER_AGENT = "registerAgent";
    final static String FETCH_USE_CASES = "fetchUseCases";
    final static String GENERATE_URL = "generateCallURL";
    final static String START_CALL = "startCall";
    final static String REGISTER_LISTENER = "registerListener";
}
