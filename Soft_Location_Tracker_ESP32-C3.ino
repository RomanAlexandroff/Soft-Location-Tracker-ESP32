/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   Soft_Location_Tracker_ESP32-C3.ino                               :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/09/09 18:48:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   This is the Main file of the Soft Tracker Project. This firmware edition is adapted for      */
/*   ESP32-C3 microcontrollers. The firmware allows User to track an approximate location of      */
/*   ESP-based devices via Telegram chat notifications. More details in the ReadMe file.          */
/*   Important! Firmware file not to exeed 50% of memory. Otherwise OTA is unavailable.           */
/*                                                                                                */
/* ********************************************************************************************** */

#include "header.h"

void  setup(void)
{
    short battery_state;
    bool  play_recorded;

    play_recorded = false;
    ESP.wdtEnable(WD_TIMEOUT);                                                // watchdog
    #ifdef DEBUG
        Serial.begin(115200);
    #endif
    DEBUG_PRINTF("\n\n\nDEVICE START\n\n", "");
    system_rtc_mem_read(64, &rtcMng, sizeof(rtcMng));                         // Revive variables from RTC memory after deep sleep
    configTime(0, 0, "pool.ntp.org");
    client.setTrustAnchors(&cert);                                            // Add root certificate for api.telegram.org
    WiFi.persistent(true);                                                    // Save WiFi configuration in flash - optional
    WiFi.mode(WIFI_STA);
    WiFi.hostname("SoftTraker");
    ft_wifi_list();
    if (wifiMulti.run(CONNECT_TIMEOUT) == WL_CONNECTED) 
    {
        if (rtcMng.last_wifi < 0 || rtcMng.last_wifi > 12)
            ft_power_down_recovery();
        if (rtcMng.last_wifi == 0)
            play_recorded = true;
        ft_send_location();
        if (play_recorded && rtcMng.scan_results[0][0] != '\0')
            ft_scan_report();
        battery_state = ft_battery_check();
        DEBUG_PRINTF("Current battery state is %d%%\n", battery_state);
        if (battery_state <= 15)
        {
            bot.sendMessage(CHAT_ID, "My battery is quite low. Please, charge me when you have time!", "");
        }
        else if (battery_state >= 99)                                         // unlimited messaging unlocks only when charging
        {
            bot.sendMessage(CHAT_ID, "I'm fully charged and ready for work!", "");
            ft_check_incomming_messages(0);                                   // 0 == check new messages for WAIT_FOR_MESSAGES_LIMIT times
        }
        else
            ft_check_incomming_messages(WAIT_FOR_MESSAGES_LIMIT);             // WAIT_FOR_MESSAGES_LIMIT == check new messages only 1 time
    }
    else
    {
        ft_wifi_scan();
        rtcMng.last_wifi = 0;
    }
    ft_go_to_sleep();
}

void  loop(void) { /* NOTHING IS HERE */ }
 
