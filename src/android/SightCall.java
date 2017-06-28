package com.okode.cordova.sightcall;

import android.util.Log;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.sightcall.universal.Universal;
import com.sightcall.universal.agent.UniversalAgent;
import com.sightcall.universal.agent.model.GuestInvite;
import com.sightcall.universal.agent.model.GuestUsecase;
import com.sightcall.universal.internal.api.model.SightCallCredentials;
import com.sightcall.universal.util.Environment;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
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

public class SightCall extends CordovaPlugin {

    private static final String TAG = "SightCallPlugin";
    private static final String SIGHT_CALL_PUSH_KEY = "guest_ready";
    private static final String SIGHT_CALL_PUSH_URL_KEY = "url";

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
        } else if (INVITE_GUEST.equals(action)) {
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
     * @param environmentKey
     */
    private void setEnvironment(String environmentKey) {
        if(Environment.PPR.value().equalsIgnoreCase(environmentKey)) {
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
            callback.error("Token or pin param is NULL");
            return;
        }
        Universal.agent().register(token, pin, new UniversalAgent.RegisterCallback() {
            @Override
            public void onRegisterSuccess(SightCallCredentials sightCallCredentials) {
                callback.success("Agent registration succeeded");
            }
            @Override
            public void onRegisterFailure() {
                callback.error("Agent registration failed");
            }
        });
    }

    private void fetchUseCases(final CallbackContext callback) {
        if (!Universal.agent().isAvailable()) {
            callback.error("Register the agent before");
            return;
        }
        UniversalAgent agent = Universal.agent();
        agent.fetchUsecases(new UniversalAgent.FetchUsecasesCallback() {
            @Override
            public void onFetchUsecasesSuccess() {
                callback.success("Agent use cases retrieved");
            }

            @Override
            public void onFetchUsecasesFailure() {
                callback.error("Error retrieving agent use cases");
            }
        });
    }

    private void invite(String phoneNumber, final CallbackContext callback) {
        if (!Universal.agent().isAvailable()) {
            callback.error("Register the agent before");
            return;
        }
        if (phoneNumber == null) {
            callback.error("Phone number cannot be NULL");
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
    public final static String DEMO = "demo";
    public final static String ENABLE_LOGGER = "enableLogger";
    public final static String SET_ENVIRONMENT = "setEnvironment";
    public final static String REGISTER_AGENT = "registerAgent";
    public final static String FETCH_USE_CASES = "fetchUseCases";
    public final static String INVITE_GUEST = "invite";
    public final static String IS_SIGHT_CALL_PUSH = "isSightCallPush";
    public final static String START_CALL_FROM_PUSH = "startCallFromPush";
}
