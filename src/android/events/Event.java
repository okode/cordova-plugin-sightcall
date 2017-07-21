package com.okode.cordova.sightcall.events;

import org.json.JSONObject;

/**
 * Created by rpanadero on 21/7/17.
 */

public interface Event {

    public static final String EVENT_TYPE = "eventType";
    public static final String EVENT_DATA = "eventData";
    /**
     * The event name.
     *
     * @return The event name.
     */
    String getEventName();

    /**
     * The event data.
     *
     * @return The event data.
     */
    JSONObject getEventData();
}
