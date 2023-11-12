/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   globals.h                                                        :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/10/31 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/11/12 11:40:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   Global variables and objects declaration                                                     */
/*                                                                                                */
/* ********************************************************************************************** */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <WiFiClientSecure.h>
#include <WiFiMulti.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <UniversalTelegramBot.h>
#include "credentials.h"

#define SLEEP_DURATION               3600000000ULL   // in microseconds (60000000 == 1 minute; 3600000000 == 1 hour)

extern WiFiMulti wifiMulti;
extern WiFiClientSecure client;
extern UniversalTelegramBot bot;
extern AsyncWebServer server;

extern RTC_DATA_ATTR unsigned short  g_last_wifi;
extern RTC_DATA_ATTR unsigned short  g_offline_wakeups;
extern RTC_DATA_ATTR bool            g_reboot;
extern unsigned int                  g_for_this_long;

#endif
