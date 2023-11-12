/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   globals.cpp                                                      :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/10/31 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/11/12 11:40:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   Global variables and objects initialization                                                  */
/*                                                                                                */
/* ********************************************************************************************** */

#include "globals.h"

WiFiMulti wifiMulti;
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
AsyncWebServer server(80);

RTC_DATA_ATTR unsigned short  g_last_wifi;
RTC_DATA_ATTR unsigned short  g_offline_wakeups;
RTC_DATA_ATTR bool            g_reboot;
unsigned int                  g_for_this_long = SLEEP_DURATION;       // setting Deep Sleep default length
