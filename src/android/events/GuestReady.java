package com.okode.cordova.sightcall.events;

import android.util.Log;

import com.okode.cordova.sightcall.Constants;

import org.json.JSONException;
import org.json.JSONObject;

public class GuestReady implements Event {

    private static final String GUEST_READY_EVENT_NAME = "sightcall.guestready";
    private static final String CALL_ID_PARAM = "callId";
    private static final String CASE_REPORT_ID_PARAM = "caseReportId";

    private String callId;
    private String caseReportId;

    public GuestReady(String callId, String caseReportId) {
        this.callId = callId;
        this.caseReportId = caseReportId;
    }

    @Override
    public String getEventName() {
        return GUEST_READY_EVENT_NAME;
    }

    @Override
    public JSONObject getEventData() {
        JSONObject data = new JSONObject();
        try {
            data.putOpt(CALL_ID_PARAM, callId);
            data.putOpt(CASE_REPORT_ID_PARAM, caseReportId);
        } catch (JSONException e) {
            Log.e(Constants.TAG, "Error constructing guest ready object. Message: " + e);
        }
        return data;
    }
}
