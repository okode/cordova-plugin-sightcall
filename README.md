Cordova Plugin SightCall
========================

Cordova Plugin for SightCall.

**Important**: This plugin only works on Cordova projects that are using Android with AndroidX. So, if you don't want to have dependency with AndroidX in your Android project for some reason, please use a plugin version earlier than 4.0.

##	 Installation

- Add this to config.xml

```
	<plugin name="cordova-plugin-sightcall" spec="https://github.com/okode/cordova-plugin-sightcall.git">
	    <variable name="MICROPHONE_USAGE_DESCRIPTION" value="Used for videocalls" />
	    <variable name="UNIVERSAL_FEATURE_DEEPLINK_URI" value="@string/universal_feature_deeplink_uri" />
	</plugin>
```

- Add this to package.json below your dependencies section

```
	"cordova-plugin-sightcall": "git+https://github.com/okode/cordova-plugin-sightcall.git"
```

- Add this to package.json below your cordova/plugins section

```
	"cordova-plugin-sightcall": {
		"MICROPHONE_USAGE_DESCRIPTION": "Used for videocalls",
		"UNIVERSAL_FEATURE_DEEPLINK_URI": "@string/universal_feature_deeplink_uri"
	}
```

- Register a custom application Firebase Messaging service (only on Android if you have a notification broker that uses Firebase Cloud Messaging).

    If you are using a push notification broker which uses Firebase Messaging service, you will must have a custom application one that broadcasts Firebase events to the differents Messaging services.

    For example, consider an application that uses both Urban Airship's SDK and Sightcall's SDK. Both libraries register their own Firebase Messaging services so that the registered one with the highest priority at manifest will work but the another one won't. As a result, some push notification library could stop working on Android if you install this plugin. To fix this unexpected behaviour, you must register a custom application Firebase Messagin service like that:

	- Add this to config.xml in order to add you custom service to your Android project and register it in AndroidManifest.xml with high priority.

	```
        <resource-file src="resources/android/urban_sightcall_integration/CustomFcmListenerService.java" target="src/com/mapfre/reparadores/CustomFcmListenerService.java" />
        <custom-config-file target="AndroidManifest.xml" parent="./application">
            <service android:enabled="@bool/universal_feature_fcm" android:exported="false" android:name="com.mapfre.reparadores.CustomFcmListenerService">
                <intent-filter android:priority="9999">
                    <action android:name="com.google.firebase.MESSAGING_EVENT" />
                </intent-filter>
            </service>
        </custom-config-file>
    ```

## Troubleshooting guide

Q: I get compilation errors on Android

A: First, check if your project compiles without this plugin. In that case, verify the plugin version you are using on your project because since 4.0 version we just support Cordova projects that are using Android with AndroidX. Below 4.0 version, take into account that this plugin is not compatible with AndroidX as it uses old Android Support Libraries. In that case, to migrate the plugin code to AndroidX, you can do the following:

- Add plugin to enable AndroidX in the project (`cordova plugin add cordova-plugin-androidx`) and add plugin to patch existing plugin source that uses the Android Support Library to use AndroidX (`cordova plugin add cordova-plugin-androidx-adapter`)

- Or using Jetifier if you are using this plugin on a Capacitor project: https://github.com/mikehardy/jetifier