/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   header.h                                                         :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/09/10 18:48:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   This firmware allows User to track an approximate location of ESP-based devices via          */
/*   Telegram chat notifications.                                                                 */
/*   Important! Firmware file not to exeed 50% of memory. Otherwise OTA unavailable.              */
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
#include <AsyncElegantOTA.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>                                              // Telegram
#include <driver/adc.h>
#include "esp_adc_cal.h"
#include <esp_task_wdt.h>
#include "credentials.h"

#define PRIVATE                                                       // comment out this line to allow bot answer in any Telegram chat
#define DEBUG                                                         // comment out this line to turn off Serial output
#ifdef DEBUG
  #define DEBUG_PRINTF(x, y) Serial.printf(x, y)
  #define DEBUG_PRINTS(q, w, e, r, t) Serial.printf(q, w, e, r, t)
#else
  #define DEBUG_PRINTF(x, y)
  #define DEBUG_PRINTS(q, w, e, r, t)
#endif
#define WD_TIMEOUT              5000                                  // watchdog, in milliseconds (5000 == 5 seconds; 8500 — system max val)
#define CONNECT_TIMEOUT         5000                                  // WiFi timeout per each AP, in milliseconds. Increase if cannot connect.
#define WAIT_FOR_OTA_LIMIT      30000                                 // in milliseconds (30000 == 30 seconds)
#define WAIT_FOR_MESSAGES_LIMIT 80                                    // in seconds, 1 == 2 seconds (80 == 160 seconds == 2,5 minutes)
#define SLEEP_DURATION          3600000000                            // in microseconds (60000000 == 1 minute; 3600000000 == 1 hour)
#define MAX_NETWORKS            16                                    // maximum number of Wi-Fi networks names to store in RTC memory
#define MAX_NAME_LENGTH         18                                    // maximum number of caracters in Wi-Fi networks names to store in RTC memory

RTC_DATA_ATTR unsigned short  g_last_wifi;
RTC_DATA_ATTR char            g_scan_results[MAX_NETWORKS][MAX_NAME_LENGTH];
unsigned int                  g_for_this_long = SLEEP_DURATION;                     // setting Deep Sleep default length

WiFiMulti wifiMulti;
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
AsyncWebServer server(80);

#include "other.h"
#include "ota_mode.h"
#include "wifi_recorder.h"
#include "send_location.h"
#include "check_incomming_messages.h"
#include "power_down_recovery.h"
#include "wifi_list.h"

void   ft_clear_scan_results(void);
String ft_write_report_message(void);
void   ft_scan_report(void);
void   IRAM_ATTR ft_wifi_scan(void);
void   IRAM_ATTR ft_wifi_list(void);
void   IRAM_ATTR ft_send_location(void);
void   ft_check_incomming_messages(short cycles);
short  ft_new_messages(int numNewMessages);
short  IRAM_ATTR ft_answer_engine(String chat_id, String text);
void   ft_power_down_recovery(void);
void   ft_ota_mode(String chat_id);
short  ft_battery_check(void);
void   ft_go_to_sleep(void);

#endif
 
