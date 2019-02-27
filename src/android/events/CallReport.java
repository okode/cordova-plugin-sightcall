package com.okode.cordova.sightcall.events;

import android.util.Log;

import com.okode.cordova.sightcall.Constants;
import com.sightcall.universal.event.CallReportEvent;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by rpanadero on 21/7/17.
 */

public class CallReport implements Event {

    private static final String CALL_REPORT_EVENT_RECEIVED = "sightcall.callreportevent";
    private static final String END_REASON = "endReason";
    private static final String DURATION = "duration";
    private static final String ACTIVE_DURATION = "activeDuration";
    private static final String CASE_REPORT_ID = "caseReportId";

    private CallReportEvent event;

    public CallReport(CallReportEvent event) {
        this.event = event;
    }

    @Override
    public String getEventName() {
        return CALL_REPORT_EVENT_RECEIVED;
    }

    @Override
    public JSONObject getEventData() {
        JSONObject data = new JSONObject();
        try {
            data.putOpt(DURATION, this.event.duration());
            data.putOpt(ACTIVE_DURATION, this.event.activeDuration());
            data.putOpt(END_REASON, this.event.result().toString());
            data.putOpt(CASE_REPORT_ID, this.event.session().caseReportId());
        } catch (JSONException e) {
            Log.e(Constants.TAG, "Error constructing notification object. Message: " + e);
        }
        return data;
    }
}
