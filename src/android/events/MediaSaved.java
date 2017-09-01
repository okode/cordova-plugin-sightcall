package com.okode.cordova.sightcall.events;

import android.util.Log;

import com.okode.cordova.sightcall.Constants;
import com.sightcall.universal.media.MediaSavedEvent;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by rpanadero on 21/7/17.
 */

public class MediaSaved implements Event {

    private static final String MEDIA_EVENT_RECEIVED = "sightcall.mediaevent";
    private static final String FILE_PATH_PARAM = "filePath";
    private static final String FILE_SIZE_PARAM = "size";


    private MediaSavedEvent event;

    public MediaSaved(MediaSavedEvent event) {
        this.event = event;
    }

    @Override
    public String getEventName() {
        return MEDIA_EVENT_RECEIVED;
    }

    @Override
    public JSONObject getEventData() {
        JSONObject data = new JSONObject();
        try {
            data.putOpt(FILE_PATH_PARAM, this.event.media().image().getPath());
            data.putOpt(FILE_SIZE_PARAM, this.event.media().lengthInBytes());
        } catch (JSONException e) {
            Log.e(Constants.TAG, "Error constructing notification object. Message: " + e);
        }
        return data;
    }
}
