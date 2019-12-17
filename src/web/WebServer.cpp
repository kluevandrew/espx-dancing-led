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
#include "WebServer.h"
#include <web/actions/IndexAction.h>
#include <web/actions/AudioAction.h>
#include <web/actions/AdcAction.h>
#include <web/actions/SetAnimationAction.h>
#include <web/actions/PutSettingsAction.h>
#include <web/actions/WiFiSetAction.h>
#include <web/actions/WiFiResetAction.h>
#include <web/actions/WiFiScanAction.h>
#ifdef ESP32
#include <SPIFFS.h>
#endif

WebServer::WebServer() {
    configure();
}

WebServer::~WebServer() = default;

void WebServer::listen() {
    server.begin();
}

void WebServer::configure() {
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "*");

    socket.onEvent(&WebServer::onSocketEvent);
    server.addHandler(&socket);

    server.on("/api/v1/", HTTP_GET, IndexAction());
    server.on("/api/v1/audio/", HTTP_GET, AudioAction());
    server.on("/api/v1/adc/", HTTP_GET, AdcAction());

    server.on("/api/v1/animation/", HTTP_POST, emptyAction(), nullptr, SetAnimationAction());
    server.on("/api/v1/wifi/", HTTP_POST, emptyAction(), nullptr, WiFiSetAction());
    server.on("/api/v1/wifi/", HTTP_DELETE, WiFiResetAction());
    server.on("/api/v1/wifi/", HTTP_GET, WiFiScanAction());
    server.on("/api/v1/settings/", HTTP_PUT, emptyAction(), nullptr, PutSettingsAction());

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");;
    server.onNotFound(&WebServer::notFound);
}

void WebServer::notFound(AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS) {
        request->send(200);
        return;
    }
    request->send(404, "text/plain", "Not found");
}

void WebServer::onSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg,
                              byte *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
        client->ping();
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("ws[%s][%u] disconnect\n", server->url(), client->id());
    } else if (type == WS_EVT_ERROR) {
        Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t *) arg), (char *) data);
    } else if (type == WS_EVT_PONG) {
        Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char *) data : "");
    } else if (type == WS_EVT_DATA) {
        auto *info = (AwsFrameInfo *) arg;
        String msg = "";
        if (info->final && info->index == 0 && info->len == len) {
            if (info->opcode == WS_TEXT) {
                for (unsigned long long i = 0; info->len > i; i++) {
                    msg += (char) data[i];
                }
            } else {
                char buff[3];
                for (unsigned long long i = 0; info->len > i; i++) {
                    sprintf(buff, "%02x ", (byte) data[i]);
                    msg += buff;
                }
            }

            processWsMessage(msg, client);
        } else {
            client->text("Message is too large");
            client->close();
        }
    }
}

void WebServer::processWsMessage(const String& msg, AsyncWebSocketClient *client) {
    SocketEventHandler::handle(msg, client);
}

void WebServer::sendEvent(const String &name, DynamicJsonDocument &message) {
    message["id"] = false;
    message["event"] = name;

    String json;
    serializeJson(message, json);
    socket.textAll(json);
}
