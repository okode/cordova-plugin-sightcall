#!/usr/bin/env node
'use strict';

var fs = require('fs');

var getValue = function(config, name) {
    var value = config.match(new RegExp('<' + name + '>(.*?)</' + name + '>', "i"))
    if(value && value[1]) {
        return value[1]
    } else {
        return null
    }
}

function fileExists(path) {
  try  {
    return fs.statSync(path).isFile();
  }
  catch (e) {
    return false;
  }
}

function directoryExists(path) {
  try  {
    return fs.statSync(path).isDirectory();
  }
  catch (e) {
    return false;
  }
}

var stripRegex = "([^\@<]+?)";
function stripStringNonDefaultValue(strings, stringKey) {
  if (strings == null) return;
  return strings.replace(new RegExp('<string name="' + stringKey + '">' + stripRegex + '<\/string>', "i"), '');
}

function setValueToString(strings, stringKey, newValue) {
  if (strings == null) return;
  return strings.replace(new RegExp('<string name="' + stringKey + '">' + stripRegex + '<\/string>', "i"), '<string name="' + stringKey + '">' + newValue + '</string>')
}

var config = fs.readFileSync("config.xml").toString()
var name = getValue(config, "name")

if (directoryExists("platforms/ios")) {
  var paths = ["GoogleService-Info.plist", "platforms/ios/www/GoogleService-Info.plist"];

  for (var i = 0; i < paths.length; i++) {
    if (fileExists(paths[i])) {
      try {
        var contents = fs.readFileSync(paths[i]).toString();
        fs.writeFileSync("platforms/ios/" + name + "/Resources/GoogleService-Info.plist", contents)
      } catch(err) {
        process.stdout.write(err);
      }

      break;
    }
  }
}

if (directoryExists("platforms/android")) {
  var paths = ["google-services.json", "platforms/android/assets/www/google-services.json"];

  for (var i = 0; i < paths.length; i++) {
    if (fileExists(paths[i])) {
      try {
        var contents = fs.readFileSync(paths[i]).toString();
        fs.writeFileSync("platforms/android/google-services.json", contents);

        var json = JSON.parse(contents);
        var strings = fs.readFileSync("platforms/android/res/values/strings.xml").toString();

        // strip non-default values
        strings = stripStringNonDefaultValue(strings, "google_app_id");
        strings = stripStringNonDefaultValue(strings, "google_api_key");
        strings = stripStringNonDefaultValue(strings, "gcm_defaultSenderId");
        strings = stripStringNonDefaultValue(strings, "default_web_client_id");
        strings = stripStringNonDefaultValue(strings, "ga_trackingId");
        strings = stripStringNonDefaultValue(strings, "firebase_database_url");
        strings = stripStringNonDefaultValue(strings, "google_crash_reporting_api_key");

        // strip empty lines
        strings = strings.replace(new RegExp('(\r\n|\n|\r)[ \t]*(\r\n|\n|\r)', "gm"), '$1')

        // replace the default values
        strings = setValueToString(strings, "google_app_id", json.client[0].client_info.mobilesdk_app_id);
        strings = setValueToString(strings, "google_api_key", json.client[0].api_key[0].current_key);
        strings = setValueToString(strings, "gcm_defaultSenderId", json.project_info.project_number);
        strings = setValueToString(strings, "default_web_client_id", json.client[0].oauth_client[0].client_id);
        if (json.client[0].services.analytics_service.analytics_property != null) {
          strings = setValueToString(strings, "ga_trackingId", json.client[0].services.analytics_service.analytics_property.tracking_id);
        }
        strings = setValueToString(strings, "firebase_database_url", json.project_info.firebase_url);
        strings = setValueToString(strings, "google_crash_reporting_api_key", json.client[0].api_key[0].current_key);

        fs.writeFileSync("platforms/android/res/values/strings.xml", strings);
      } catch(err) {
        process.stdout.write(err);
      }

      break;
    }
  }
}