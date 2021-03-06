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
#include <Application.h>
#include <settings/Settings.h>
#include "PutSettingsAction.h"

void
PutSettingsAction::operator()(AsyncWebServerRequest *request, byte *data, size_t len, size_t index, size_t total) {
    DynamicJsonDocument requestBody(1024);
    deserializeJson(requestBody, (const char *) data);

    if (!requestBody.containsKey("key") || !requestBody.containsKey("value")) {
        jsonBadRequest(request);
        return;
    }

    Settings::getInstance().set(requestBody["key"], requestBody.getMember("value"));
    Settings::getInstance().save();

    requestBody["status"] = "ok";
    json(request, requestBody);
}
