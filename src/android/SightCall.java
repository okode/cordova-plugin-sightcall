package com.okode.cordova.sightcall;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.json.JSONArray;
import org.json.JSONException;

public class SightCall extends CordovaPlugin {

    @Override
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
        if ("demo".equals(action)) {
            System.out.println("SightCall: demo invoked");
            callbackContext.success();
            return true;
        }
        callbackContext.error(action + " is not a supported action");
        return false;
    }

}
