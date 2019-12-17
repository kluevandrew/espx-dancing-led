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
#ifdef ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <config/config.h>
#include <Application.h>
#include "WiFiManager.h"

bool WiFiManager::autoConnect() {
    #ifdef ESP32

    #else
        WiFi.setSleepMode(WIFI_NONE_SLEEP);
    #endif

    connectSTA();
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Client Failed! Fallback to AP mode.\n");
        connectAP();
        if (WiFi.waitForConnectResult() != WL_CONNECTED) {
            Serial.printf("WiFi AP Failed!\n");
        }
        Serial.print("IP Address: ");
        Serial.println(WiFi.softAPIP());
        WiFi.scanNetworks(true, true);
        return true;
    } else {
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    }
    WiFi.scanNetworks(true, true);
    return false;
}

void WiFiManager::reset() {
    WiFi.disconnect(true);
    WiFi.softAPdisconnect(true);
    Application::hardwareReset();
}

void WiFiManager::connect(const String &ssid, const String &pass) {
    WiFi.begin(ssid.c_str(), pass.c_str(), 0, nullptr, false);
    Application::reboot();
}

void WiFiManager::connect(const String &ssid) {
    WiFi.begin(ssid.c_str(), nullptr, 0, nullptr, false);
    Application::reboot();
}

void WiFiManager::connectSTA() {
    WiFi.mode(WIFI_STA);
#ifdef WIFI_CLIENT_SSID
#ifdef WIFI_CLIENT_PASS
    WiFi.begin(WIFI_CLIENT_SSID, WIFI_CLIENT_PASS);
#else
    WiFi.begin(WIFI_CLIENT_SSID);
#endif
#endif
    WiFi.begin();
}

void WiFiManager::connectAP() {
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
#if defined(WIFI_AP_PASS)
    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASS);
#else
    WiFi.softAP(WIFI_AP_SSID);
#endif
}

WiFiManager::ScanResult WiFiManager::scan() {
    WiFiManager::ScanResult result;

    int count = WiFi.scanComplete();
    if (count == WIFI_SCAN_FAILED) {
        WiFi.scanNetworks(true, true);
    }

    if (count < 0) {
        result.running = true;
        result.count = 0;
        return result;
    }

    result.running = false;
    result.count = count;
    if (count > 0) {
        for (int i = 0; i < count; ++i) {
            WiFiManager::Network network;
            network.ssid = WiFi.SSID(i);
            network.rssi = WiFi.RSSI(i);
            network.bssid = WiFi.BSSIDstr(i);
            #ifdef ESP32

            #else
                network.encryption = WiFi.encryptionType(i) != ENC_TYPE_NONE;
                network.isHidden = WiFi.isHidden(i);
            #endif
            network.channel = WiFi.channel(i);
            result.networks.push_back(network);
        }
        WiFi.scanDelete();
        if (WiFi.scanComplete() == WIFI_SCAN_FAILED) {
            WiFi.scanNetworks(true, true);
        }
    }

    return result;
}
