package com.okode.cordova.sightcall.events;

import android.util.Log;

import com.okode.cordova.sightcall.Constants;

import org.json.JSONException;
import org.json.JSONObject;

public class CallStart implements Event {

    private static final String CALL_START_EVENT_NAME = "sightcall.callstart";
    private static final String CALL_ID_PARAM = "callId";

    private String callId;

    public CallStart(String callId) {
        this.callId = callId;
    }

    @Override
    public String getEventName() {
        return CALL_START_EVENT_NAME;
    }

    @Override
    public JSONObject getEventData() {
        JSONObject data = new JSONObject();
        try {
            data.putOpt(CALL_ID_PARAM, this.callId);
        } catch (JSONException e) {
            Log.e(Constants.TAG, "Error constructing notification object. Message: " + e);
        }
        return data;
    }
}
