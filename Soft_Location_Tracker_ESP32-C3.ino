/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   Soft_Location_Tracker_ESP32-C3.ino                               :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/09/10 18:48:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   This is the Main file of the Soft Tracker Project. This firmware edition is adapted for      */
/*   ESP32 microcontrollers. The firmware allows User to track an approximate location of         */
/*   ESP-based devices via Telegram chat notifications. More details in the ReadMe file.          */
/*   Important! Firmware file is not to exeed 50% of memory. Otherwise OTA is unavailable.        */
/*                                                                                                */
/* ********************************************************************************************** */

#include "header.h"

void  setup(void)
{
    short battery_state;
    bool  play_recorded;

    play_recorded = false;
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_0db);
    esp_sleep_enable_timer_wakeup(g_for_this_long);
    esp_task_wdt_init(WD_TIMEOUT, true);                                      // watchdog
    #ifdef DEBUG
        Serial.begin(115200);
    #endif
    DEBUG_PRINTF("\n\n\nDEVICE START\n\n", "");
    WiFi.persistent(true);                                                    // Save WiFi configuration in flash - optional
    WiFi.mode(WIFI_STA);
    WiFi.hostname("SoftTraker");
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
    ft_wifi_list();
    if (wifiMulti.run(CONNECT_TIMEOUT) == WL_CONNECTED) 
    {
        if (g_last_wifi < 0 || g_last_wifi > 12)
            ft_power_down_recovery();
        if (g_last_wifi == 0)
            play_recorded = true;
        ft_send_location();
        if (play_recorded && g_scan_results[0][0] != '\0')
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
        g_last_wifi = 0;
    }
    ft_go_to_sleep();
}

void  loop(void) { /* NOTHING IS HERE */ }
 
