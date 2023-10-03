/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   header.h                                                         :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/10/03 18:09:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   This firmware allows User to track an approximate location of ESP-based devices via          */
/*   Telegram chat notifications.                                                                 */
/*                                                                                                */
/* ********************************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

#include <Arduino.h>                                                  // OTA
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiMulti.h>
#include <stdio.h>
#include <AsyncTCP.h>                                                 // OTA
#include <ESPAsyncWebServer.h>                                        // OTA
//#include <AsyncElegantOTA.h>
#include <ElegantOTA.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>                                              // Telegram
#include "LittleFS.h"
#include <driver/adc.h>
#include "esp_adc_cal.h"
#include <esp_task_wdt.h>
#include "credentials.h"

#define SOFTWARE_VERSION        2.01
#define PRIVATE                                                       // comment out this line to allow bot answer in any Telegram chat
#define DEBUG                                                         // comment out this line to turn off Serial output
#ifdef DEBUG
  #define DEBUG_PRINTF(x, y) Serial.printf(x, y)
  #define DEBUG_PRINTS(q, w, e, r, t) Serial.printf(q, w, e, r, t)
#else
  #define DEBUG_PRINTF(x, y)
  #define DEBUG_PRINTS(q, w, e, r, t)
#endif
#define WD_TIMEOUT              8000                                  // watchdog, in milliseconds (5000 == 5 seconds; 8500 — system max val)
#define CONNECT_TIMEOUT         5000                                  // WiFi timeout per each AP, in milliseconds. Increase if cannot connect.
#define WAIT_FOR_OTA_LIMIT      60                                    // in seconds
#define WAIT_FOR_MESSAGES_LIMIT 80                                    // in seconds, 1 == 2 seconds (80 == 160 seconds == 2,5 minutes)
#define SLEEP_DURATION          3600000000ULL                         // in microseconds (60000000 == 1 minute; 3600000000 == 1 hour)

RTC_DATA_ATTR unsigned short  g_last_wifi;
RTC_DATA_ATTR unsigned short  g_offline_wakeups;
unsigned int                  g_for_this_long = SLEEP_DURATION;       // setting Deep Sleep default length

WiFiMulti wifiMulti;
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
AsyncWebServer server(80);

#include "other.h"
#include "spiffs_management.h"
#include "ota_mode.h"
#include "offline_tracking.h"
#include "send_location.h"
#include "check_incomming_messages.h"
#include "power_down_recovery.h"
#include "wifi_list.h"

void    ft_write_spiffs_file(const char* file_name, String input);
String  ft_read_spiffs_file(const char* file_name);
void    ft_delete_spiffs_file(const char* file_name);
void    IRAM_ATTR ft_spiffs_init(void);
String  ft_write_report_message(void);
void    ft_backup_connection(void);
void    IRAM_ATTR ft_wifi_scan(void);
void    IRAM_ATTR ft_wifi_list(void);
void    IRAM_ATTR ft_send_location(void);
void    ft_check_incomming_messages(short cycles);
short   ft_new_messages(int numNewMessages);
short   IRAM_ATTR ft_answer_engine(String chat_id, String text);
void    ft_power_down_recovery(void);
short   ft_ota_mode(String chat_id);
short   ft_battery_notification(void);
short   ft_battery_check(void);
void    ft_go_to_sleep(void);

#endif
 
