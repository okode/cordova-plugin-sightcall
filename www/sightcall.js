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

SightCall.isSightCallPush = function(payload) {
    return payload != null && payload.extras != null && payload.extras.guest_ready != null;
};

SightCall.startCall = function(payload) {
    if (SightCall.isSightCallPush(payload)) {
        var sightCallExtra = payload.extras.guest_ready;
        try {
            var callURL = JSON.parse(sightCallExtra).url;
            exec(null, null, "SightCall", "startCall", [callURL]);
        } catch (e) {
            console.error('Error parsing sight call payload extra');
        }
    }
};

module.exports = SightCall;