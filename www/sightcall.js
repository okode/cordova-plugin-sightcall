var exec = require('cordova/exec');

var SightCall = function() {};

SightCall.demo = function() {
    exec(null, null, "SightCall", "demo", []);
};

module.exports = SightCall;