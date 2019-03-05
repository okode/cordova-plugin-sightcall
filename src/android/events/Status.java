package com.okode.cordova.sightcall.events;

import android.util.Log;

import com.okode.cordova.sightcall.Constants;

import net.rtccloud.sdk.event.call.StatusEvent;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by rpanadero on 21/7/17.
 */

public class Status implements Event {

    private static final String STATUS_EVENT_RECEIVED = "sightcall.statusevent";
    private static final String STATUS_PARAM = "status";

    private StatusEvent event;

    public Status(StatusEvent event) {
        this.event = event;
    }

    @Override
    public String getEventName() {
        return STATUS_EVENT_RECEIVED;
    }

    @Override
    public JSONObject getEventData() {
        JSONObject data = new JSONObject();
        try {
            data.putOpt(STATUS_PARAM, this.event.status().toString());
        } catch (JSONException e) {
            Log.e(Constants.TAG, "Error constructing notification object. Message: " + e);
        }
        return data;
    }
}
