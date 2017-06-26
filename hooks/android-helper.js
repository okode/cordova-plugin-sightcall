var fs = require("fs");
var path = require("path");
var utilities = require("./utilities");

module.exports = {

    addGServicesBuildToolsGradle: function() {

        var buildGradle = utilities.readBuildGradle();

        buildGradle +=  [
            "",
            "// Sight Call Cordova Plugin - Start Sight Call Build Tools ",
            "buildscript {",
            "    repositories {",
            "        jcenter()",
            "    }",
            "    dependencies {",
            "        classpath 'com.google.gms:google-services:3.1.0'",
            "    }",
            "}",
            "",
            "apply plugin: 'com.google.gms.google-services'",
            "// Sight Call Cordova Plugin - End Sight Call Build Tools",
        ].join("\n");

        utilities.writeBuildGradle(buildGradle);
    },

    removeGServicesBuildToolsFromGradle: function() {

        var buildGradle = utilities.readBuildGradle();

        buildGradle = buildGradle.replace(/\n\/\/ Sight Call Cordova Plugin - Start Sight Call Build Tools[\s\S]*\/\/ Sight Call Cordova Plugin - End Sight Call Build Tools/, "");

        utilities.writeBuildGradle(buildGradle);
    }
};