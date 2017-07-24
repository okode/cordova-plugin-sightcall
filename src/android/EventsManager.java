package com.okode.cordova.sightcall;

import com.okode.cordova.sightcall.events.CallReport;
import com.okode.cordova.sightcall.events.Event;
import com.okode.cordova.sightcall.events.Status;
import com.sightcall.universal.event.UniversalCallReportEvent;
import com.sightcall.universal.event.UniversalStatusEvent;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by rpanadero on 21/7/17.
 */

public class EventsManager {


    private static final EventsManager manager = new EventsManager();
    private EventListener listener = null;

    private final List<Event> pendingEvents = new ArrayList<Event>();

    private EventsManager() {}

    public static EventsManager instance() {
        return manager;
    }

    public void sendStatusEvent(UniversalStatusEvent event) {
        this.notifyListener(new Status(event));
    }

    public void sendCallReportEvent(UniversalCallReportEvent event) {
        this.notifyListener(new CallReport(event));
    }

    public void setListener(EventListener listener) {
        synchronized (manager) {
            this.listener = listener;
            this.processPendingEvents();
        }
    }

    public void addPendingEvent(Event event) {
        if (event != null) {
            this.pendingEvents.add(event);
        }
    }

    /** Notifies of all pending events if there is a listener set
     *
     */
    private void processPendingEvents() {
        if (this.listener != null && !this.pendingEvents.isEmpty()) {
            for (Event event: pendingEvents) {
                this.notifyListener(event);
            }
            this.pendingEvents.clear();
        }
    }

    /**
     * Helper method to notify the listener of the event.
     *
     * @param event The event.
     * @return {@code true} if the listener was notified, otherwise {@code false}.
     */
    private boolean notifyListener(Event event) {
        synchronized (manager) {
            if (listener != null) {
                listener.onEvent(event);
                return true;
            }
            return false;
        }
    }

}
