package com.okode.cordova.sightcall.events;

import android.util.Log;

import com.okode.cordova.sightcall.Constants;

import org.json.JSONException;
import org.json.JSONObject;

public class CallStart implements Event {

    private static final String CALL_START_EVENT_NAME = "sightcall.callstart";
    private static final String CALL_ID_PARAM = "callId";
    private static final String CASE_REPORT_ID = "caseReportId";

    private String callId;
    private String caseReportId;

    public CallStart(String callId, String caseReportId) {
        this.callId = callId;
        this.caseReportId = caseReportId;
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
            data.putOpt(CASE_REPORT_ID, this.caseReportId);
        } catch (JSONException e) {
            Log.e(Constants.TAG, "Error constructing notification object. Message: " + e);
        }
        return data;
    }
}
