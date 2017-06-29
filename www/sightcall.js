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
    return new Promise(function(resolve, reject) {
        exec(function() {
            resolve();
        }, function(error) {
            reject(error);
        }, "SightCall", "isSightCallPush", [payload]);
    });
};

SightCall.startCallFromPush = function(payload) {
    exec(null, null, "SightCall", "startCallFromPush", [payload]);
};

module.exports = SightCall;