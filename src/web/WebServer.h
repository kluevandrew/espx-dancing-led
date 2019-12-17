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
#ifndef ESPX_DANCING_MUSIC_WEBSERVER_H
#define ESPX_DANCING_MUSIC_WEBSERVER_H


#include <ESPAsyncWebServer.h>
#include <config/config.h>
#include <web/events/SocketEventHandler.h>

class WebServer {
public:
    explicit WebServer();

    ~WebServer();

    void listen();

    void sendEvent(const String &name) {
        DynamicJsonDocument event(128);
        sendEvent(name, event);
    };

    void sendEvent(const String &name, DynamicJsonDocument &message);

private:
    AsyncWebServer server = AsyncWebServer(WEB_SERVER_PORT);
    AsyncWebSocket socket = AsyncWebSocket("/ws");

    void configure();

    static void notFound(AsyncWebServerRequest *request);

    static ArRequestHandlerFunction emptyAction() {
        return [](AsyncWebServerRequest *request) {};
    };

    static void
    onSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, byte *data,
                  size_t len);

    static void processWsMessage(const String &msg, AsyncWebSocketClient *client);
};


#endif //ESPX_DANCING_MUSIC_WEBSERVER_H
