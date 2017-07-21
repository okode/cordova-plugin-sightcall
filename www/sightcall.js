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

SightCall.setNotificationToken = function(token) {
    exec(null, null, "SightCall", "setNotificationToken", [token]);
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

SightCall.registerAgent = function(token, pin) {
    return new Promise(function(resolve, reject) {
        exec(function() {
            resolve();
        }, function(error) {
            reject(error);
        }, "SightCall", "registerAgent", [token, pin]);
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

SightCall.invite = function(phoneNumber) {
    return new Promise(function(resolve, reject) {
        exec(function() {
            resolve();
        }, function(error) {
            reject(error);
        }, "SightCall", "invite", [phoneNumber]);
    });
};

SightCall.generateURL = function() {
    return new Promise(function(resolve, reject) {
        exec(function(url) {
            resolve(url);
        }, function(error) {
            reject(error);
        }, "SightCall", "generateCallURL", []);
    });
};

SightCall.isGuestReadyPush = function(payload) {
    return payload != null && payload.extras != null && payload.extras.guest_ready != null;
};

SightCall.canHandleNotification = function(payload) {
    return new Promise(function(resolve, reject) {
        exec(function() {
            resolve();
        }, function(error) {
            reject(error);
        }, "SightCall", "canHandleNotification", [payload]);
    });
};

SightCall.handleNotification = function(payload) {
    exec(null, null, "SightCall", "handleNotification", [payload]);
};

SightCall.startCall = function(url) {
    exec(null, null, "SightCall", "startCall", [url]);
};


SightCall.bindDocumentEvent = function() {
    exec(function(e) {
      console.log("Firing document event: " + e.eventType)
      cordova.fireDocumentEvent(e.eventType, e.eventData)
    }, null, "SightCall", "registerListener", []);
};

document.addEventListener("deviceready", SightCall.bindDocumentEvent, false);

module.exports = SightCall;