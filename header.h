/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   header.h                                                         :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/11/12 11:40:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   This firmware allows User to track an approximate location of ESP-based devices via          */
/*   Telegram chat notifications.                                                                 */
/*                                                                                                */
/* ********************************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

#include <Arduino.h>
#include <WiFi.h>
#include <stdio.h>
#include <ElegantOTA.h>
#include <ArduinoJson.h>
#include "LittleFS.h"
#include <esp_system.h>
#include <driver/adc.h>
#include <esp_task_wdt.h>
#include "globals.h"

#define SOFTWARE_VERSION        2.14
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
#define CONNECT_TIMEOUT         3000                                  // WiFi timeout per each AP, in milliseconds. Increase if cannot connect.
#define WAIT_FOR_MESSAGES_LIMIT 80                                    // in seconds, 1 == approx. 2 seconds (80 == 160 seconds == 2,5 minutes)

short       ft_write_spiffs_file(const char* file_name, String input);
String      ft_read_spiffs_file(const char* file_name);
void        ft_delete_spiffs_file(const char* file_name);
void        IRAM_ATTR ft_spiffs_init(void);
String      ft_write_report_message(void);
void        ft_backup_connection(void);
void        IRAM_ATTR ft_wifi_scan(void);
void        IRAM_ATTR ft_wifi_list(void);
void        IRAM_ATTR ft_send_location(void);
void        ft_check_incomming_messages(short cycles);
short       ft_new_messages(int numNewMessages);
short       IRAM_ATTR ft_answer_engine(String chat_id, String text);
void        ft_power_down_recovery(void);
inline void ft_compose_message(String ssid, IPAddress ip, String chat_id);
inline void ft_ElegantOTA_callbacks(String chat_id);
short       ft_confirm_battery_charge(String chat_id);
short       ft_ota_mode(String chat_id);
void        ft_battery_notification(void);
short       ft_battery_check(void);
void        ft_go_to_sleep(void);

#endif
 
