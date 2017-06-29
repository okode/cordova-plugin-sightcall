package com.okode.cordova.sightcall;

import android.support.annotation.NonNull;
import android.util.Log;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.sightcall.universal.Universal;
import com.sightcall.universal.agent.UniversalAgent;
import com.sightcall.universal.agent.model.GuestInvite;
import com.sightcall.universal.agent.model.GuestUsecase;
import com.sightcall.universal.event.UniversalCallReportEvent;
import com.sightcall.universal.event.UniversalStatusEvent;
import com.sightcall.universal.internal.api.model.SightCallCredentials;
import com.sightcall.universal.util.Environment;

import net.rtccloud.sdk.Event;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaInterface;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CordovaWebView;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.lang.reflect.Type;
import java.util.Map;

import static com.okode.cordova.sightcall.Methods.DEMO;
import static com.okode.cordova.sightcall.Methods.ENABLE_LOGGER;
import static com.okode.cordova.sightcall.Methods.FETCH_USE_CASES;
import static com.okode.cordova.sightcall.Methods.INVITE_GUEST;
import static com.okode.cordova.sightcall.Methods.IS_SIGHT_CALL_PUSH;
import static com.okode.cordova.sightcall.Methods.REGISTER_AGENT;
import static com.okode.cordova.sightcall.Methods.SET_ENVIRONMENT;
import static com.okode.cordova.sightcall.Methods.START_CALL_FROM_PUSH;

public class SightCall extends CordovaPlugin {

    private static final String TAG = "SightCallPlugin";
    private static final String SIGHT_CALL_PUSH_KEY = "guest_ready";
    private static final String SIGHT_CALL_PUSH_URL_KEY = "url";

    @Override
    public void initialize(CordovaInterface cordova, CordovaWebView webView) {
        super.initialize(cordova, webView);
        Universal.register(this);
    }

    //On first run, this lifecycle method is not fired. So, we invoke the register method in plugin initialization.
    @Override
    public void onStart() {
        super.onStart();
        Universal.register(this);
    }

    @Override
    public void onStop() {
        super.onStop();
        Universal.unregister(this);
    }

    @Event
    public void onStatusEvent(UniversalStatusEvent event) {
        Log.i(TAG, event.toString());
    }

    @Event
    public void onCallFinished(UniversalCallReportEvent event) {
        Log.i(TAG, event.toString());
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
            this.setEnvironment(args.optString(0, Environment.PROD.value()));
            callbackContext.success();
            return true;
        } else if (REGISTER_AGENT.equals(action)) {
            this.registerAgent(args.optString(0, null), args.optString(0, null), callbackContext);
            return true;
        } else if (FETCH_USE_CASES.equals(action)) {
            this.fetchUseCases(callbackContext);
            return true;
        } else if (INVITE_GUEST.equals(action)) {
            this.invite(args.optString(0, null), callbackContext);
            return true;
        } else if (IS_SIGHT_CALL_PUSH.equals(action)) {
            this.isSightCallPush(args.optJSONObject(0), callbackContext);
            return true;
        } else if (START_CALL_FROM_PUSH.equals(action)) {
            this.startCallFromPush(args.optJSONObject(0));
            return true;
        }
        callbackContext.error(action + " is not a supported action");
        return false;
    }

    private void demo() {
        Log.i(TAG, "DEMO invoked");
    }

    private void enableLogger(boolean enabled) {
        Universal.settings().rtccLogger().set(enabled);
        Universal.settings().universalLogger().set(enabled);
    }

    /** Allows you to change environment to PRE. If {@param environmentKey} is PPR, the PRE environment will be selected.
     *  Otherwise, the PRE environment will be selected
     *
     * @param environmentKey environmentKey
     */
    private void setEnvironment(String environmentKey) {
        if (Environment.PPR.value().equalsIgnoreCase(environmentKey)) {
            Log.i(TAG, "PRE environment selected");
            Universal.agent().setDefaultEnvironment(Environment.PPR);
        } else {
            Log.i(TAG, "PRO environment selected");
            Universal.agent().setDefaultEnvironment(Environment.PROD);
        }
    }

    private void registerAgent(String token, String pin, final CallbackContext callback) {
        if (Universal.agent().isAvailable()) {
            callback.success("Agent is already registered");
            return;
        }
        if (token == null || pin == null) {
            callback.error("Error, token or pin param is NULL");
            return;
        }
        Universal.agent().register(token, pin, new UniversalAgent.RegisterCallback() {
            @Override
            public void onRegisterSuccess(@NonNull SightCallCredentials sightCallCredentials) {
                callback.success("Agent registration succeeded");
            }
            @Override
            public void onRegisterFailure() {
                callback.error("Agent registration failed");
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
        agent.fetchUsecases(new UniversalAgent.FetchUsecasesCallback() {
            @Override
            public void onFetchUsecasesSuccess() {
                callback.onFetchUsecasesSuccess();
            }

            @Override
            public void onFetchUsecasesFailure() {
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

    private void invite(final String phoneNumber, final CallbackContext callback) {
        this.fetchUseCases(new FetchUseCasesCallback() {
            @Override
            public void onFetchUsecasesSuccess() {
                if (phoneNumber == null) {
                    callback.error("Error, phone number is NULL");
                    return;
                }
                UniversalAgent agent = Universal.agent();
                GuestUsecase usecase = agent.getGuestUsecase();
                GuestInvite invite = GuestInvite.sms(usecase, phoneNumber).build();
                agent.inviteGuest(invite, new UniversalAgent.InviteGuestCallback() {
                    @Override
                    public void onInviteGuestSuccess() {
                        callback.success("Invitation was sent successfully");
                    }

                    @Override
                    public void onInviteGuestFailure() {
                        callback.error("Invitation couldn't be sent");
                    }
                });
            }

            @Override
            public void onFetchUsecasesFailure(String message) {
                callback.error("Error sending the invitation. Unexpected error fetching use cases. Reason: " + message);
            }
        });
    }

    private boolean isSightCallPush(JSONObject payload) {
        if (payload == null) {
            Log.w(TAG, "Payload was NULL");
            return false;
        }
        return payload.has(SIGHT_CALL_PUSH_KEY);
    }

    private boolean isSightCallPush(JSONObject payload, final CallbackContext callback) {
        if (this.isSightCallPush(payload)) {
            callback.success("Push payload belongs to a sight call notification");
            return true;
        }
        callback.error("It is not a Sight Call push");
        return false;
    }

    private void startCallFromPush(JSONObject payload) {
        if (this.isSightCallPush(payload)) {
            try {
                String guestData = payload.getString(SIGHT_CALL_PUSH_KEY);
                if (guestData != null) {
                    Type type = new TypeToken<Map<String, String>>(){}.getType();
                    Map<String, String> guestDataMap = new Gson().fromJson(guestData, type);
                    Universal.start(guestDataMap.get(SIGHT_CALL_PUSH_URL_KEY));
                }
            } catch (JSONException e) {
                Log.w(TAG, "Error getting sight call push. Error: " + e.getMessage());
            }
        }
    }
}

final class Methods {
    final static String DEMO = "demo";
    final static String ENABLE_LOGGER = "enableLogger";
    final static String SET_ENVIRONMENT = "setEnvironment";
    final static String REGISTER_AGENT = "registerAgent";
    final static String FETCH_USE_CASES = "fetchUseCases";
    final static String INVITE_GUEST = "invite";
    final static String IS_SIGHT_CALL_PUSH = "isSightCallPush";
    final static String START_CALL_FROM_PUSH = "startCallFromPush";
}
