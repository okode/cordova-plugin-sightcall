var exec = require('cordova/exec');

var SightCall = function() {};

SightCall.demo = function() {
    exec(null, null, "SightCall", "demo", []);
};

SightCall.enableLogger = function(enabled) {
    exec(null, null, "SightCall", "enableLogger", [enabled]);
};

SightCall.setEnvironment = function(environmentKey) {
    exec(null, null, "SightCall", "setEnvironment", [environmentKey]);
};

SightCall.isAgentAvailable = function() {
    return new Promise(function(resolve, reject) {
        exec(function() {
            resolve();
        }, function(error) {
            reject(error);
        }, "SightCall", "isAgentAvailable", []);
    });
};

SightCall.registerAgent = function(token, apnsReference) {
    return new Promise(function(resolve, reject) {
        exec(function() {
            resolve();
        }, function(error) {
            reject(error);
        }, "SightCall", "registerAgent", [token, apnsReference]);
    });
};

SightCall.fetchUseCases = function() {
    return new Promise(function(resolve, reject) {
        exec(function() {
            resolve();
        }, function(error) {
            reject(error);
        }, "SightCall", "fetchUseCases", []);
    });
};

SightCall.generateURL = function(referenceId) {
    return new Promise(function(resolve, reject) {
        exec(function(url) {
            resolve(url);
        }, function(error) {
            reject(error);
        }, "SightCall", "generateCallURL", [referenceId]);
    });
};

SightCall.isGuestReadyPush = function(payload) {
    return payload != null && payload.extras != null && payload.extras.guest_ready != null;
};

SightCall.isCallLocalNotification = function(payload) {
    return payload != null && payload.extras != null && payload.extras.notificationId == 'SIGHTCALL_LOCALNOTIFICATION';
};

SightCall.handleCallLocalNotification = function(payload) {
    exec(null, null, "SightCall", "handleCallLocalNotification", [payload]);
};

SightCall.startCall = function(url) {
    exec(null, null, "SightCall", "startCall", [url]);
};

SightCall.bindDocumentEvent = function() {
    exec(function(e) {
      console.log("Firing document event: " + e.eventType + " with data " + JSON.stringify(e.eventData));
      cordova.fireDocumentEvent(e.eventType, e.eventData);
    }, null, "SightCall", "registerListener", []);
};

document.addEventListener("deviceready", SightCall.bindDocumentEvent, false);

module.exports = SightCall;