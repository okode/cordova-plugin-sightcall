package com.okode.cordova.sightcall.events;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.util.Log;

import com.okode.cordova.sightcall.Constants;
import com.sightcall.universal.Universal;
import com.sightcall.universal.event.UniversalStatusEvent;
import com.sightcall.universal.model.Configuration;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by rpanadero on 21/7/17.
 */

public class Status extends UniversalStatusEvent implements Event {

    private static final String STATUS_EVENT_RECEIVED = "sightcall.statusevent";
    private static final String STATUS_PARAM = "status";

    public Status(UniversalStatusEvent event) {
        super(event.status(), event.configuration());
    }

    @Override
    public String getEventName() {
        return STATUS_EVENT_RECEIVED;
    }

    @Override
    public JSONObject getEventData() {
        JSONObject data = new JSONObject();
        try {
            data.putOpt(STATUS_PARAM, this.status().toString());
        } catch (JSONException e) {
            Log.e(Constants.TAG, "Error constructing notification object. Message: " + e);
        }
        return data;
    }
}
