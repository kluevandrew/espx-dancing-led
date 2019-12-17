/* 
 * This file is part of the espx-dancing-led distribution (https://github.com/kluevandrew/espx-dancing-led).
 * Copyright (c) 2019, Kluev Andrew <kluev.andrew@gmail.com>.
 * 
 * espx-dancing-led is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 * You are free to:
 *   Share — copy and redistribute the material in any medium or format
 *   Adapt — remix, transform, and build upon the material
 *
 * Under the following terms:
 *   Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
 *   NonCommercial — You may not use the material for commercial purposes.
 *   ShareAlike — If you remix, transform, or build upon the material, you must distribute your contributions under the same license as the original.
 *   No additional restrictions — You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.
 * 
 * For additional information, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.
 */
#include <led/animations/LedAnimation.h>
#include <Application.h>
#include <settings/Settings.h>
#include "SocketEventHandler.h"

#define HANDLER(NAME, CALLBACK) if (event == NAME) { CALLBACK(payload, response); }
#define ERROR_RESPONSE(REASON) response.success = false; \
                               response.payload["error"] = REASON;

void SocketEventHandler::handle(const String &msg, AsyncWebSocketClient *client) {
    DynamicJsonDocument message(1024);
    deserializeJson(message, msg);
    unsigned long long id = message["id"];
    String event = message["event"];
    JsonVariant payload = message["payload"];

    int capacity = 1024;
    if (event == "getOptions") {
        capacity += SETTINGS_CAPACITY;
    }
    if (event == "getWifiInfo") {
        capacity += 2048;
    }

    DynamicJsonDocument responseDocument(capacity);
    auto responsePayload = responseDocument.createNestedObject("payload");
    Response response = Response(responsePayload);

    HANDLER("setAnimation", handleSetAnimationEvent)
    else HANDLER("setOption", handleSetOptionEvent)
    else HANDLER("saveOptions", handleSaveOptionsEvent)
    else HANDLER("resetOptions", handleResetOptionsEvent)
    else HANDLER("getWifiInfo", handleGetWifiInfoEvent)
    else HANDLER("connectWifi", handleConnectWifiEvent)
    else HANDLER("resetWifi", handleResetWifiEvent)
    else {
        ERROR_RESPONSE("No such action")
    }

    String json;
    responseDocument["id"] = id;
    responseDocument["event"] = event;
    responseDocument["success"] = response.success;
    serializeJson(responseDocument, json);
    client->text(json);
}

void SocketEventHandler::handleSetAnimationEvent(JsonVariant &payload, Response &response) {
    if (!payload.containsKey("name")) {
        ERROR_RESPONSE("No animation name")
        return;
    }

    String name = payload["name"];
    auto led = Application::getInstance().getLed();
    led->setAnimation(name);

    response.payload["currentAnimation"] = led->getAnimation()->getName();
}

void SocketEventHandler::handleSetOptionEvent(JsonVariant &payload, SocketEventHandler::Response &response) {
    if (!payload.containsKey("key")) {
        ERROR_RESPONSE("No key")
        return;
    }
    if (!payload.containsKey("value")) {
        ERROR_RESPONSE("No value")
        return;
    }

    auto &settings = Settings::getInstance();

    settings.set(payload["key"], payload.getMember("value"));
    response.payload["done"] = true;
}


void SocketEventHandler::handleSaveOptionsEvent(JsonVariant &payload, SocketEventHandler::Response &response) {
    Settings::getInstance().save();
    response.payload["status"] = "done";
}


void SocketEventHandler::handleResetOptionsEvent(JsonVariant &payload, SocketEventHandler::Response &response) {
    Settings::getInstance().reset();
    response.payload["status"] = "done";
}

void SocketEventHandler::handleGetWifiInfoEvent(JsonVariant &payload, SocketEventHandler::Response &response) {
    WiFiManager::ScanResult scan = WiFiManager::scan();

    if (scan.running) {
        response.payload["running"] = true;
    } else {
        response.payload["running"] = false;
        response.payload["count"] = scan.count;
        JsonArray networks = response.payload.createNestedArray("networks");
        if (scan.count > 0) {
            for (WiFiManager::Network &network : scan.networks) {
                JsonObject jsonObject = networks.createNestedObject();
                jsonObject["ssid"] = network.ssid;
                jsonObject["rssi"] = network.rssi;
                jsonObject["bssid"] = network.bssid;
                jsonObject["encryption"] = network.encryption;
                jsonObject["channel"] = network.channel;
                jsonObject["isHidden"] = network.isHidden;
            }
        }
    }

    response.payload["ap"] = WiFiManager::isInApMode();
    response.payload["ip"] = WiFiManager::ip();
}

void SocketEventHandler::handleConnectWifiEvent(JsonVariant &payload, SocketEventHandler::Response &response) {
    if (!payload.containsKey("ssid")) {
        ERROR_RESPONSE("No ssid")
        return;
    }

    String ssid = payload["ssid"];
    if (payload.containsKey("pass")) {
        String pass = payload["pass"];
        Serial.print("WiFiManager::connect(");
        Serial.print(ssid);
        Serial.print(", ");
        Serial.print(pass);
        Serial.println(")");
        WiFiManager::connect(ssid, pass);
    } else {
        Serial.print("WiFiManager::connect(");
        Serial.print(ssid);
        Serial.println(")");
        WiFiManager::connect(ssid);
    }
}

void SocketEventHandler::handleResetWifiEvent(JsonVariant &payload, SocketEventHandler::Response &response) {
    WiFiManager::reset();
}
