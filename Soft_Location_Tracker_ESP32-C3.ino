/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   Soft_Location_Tracker_ESP32-C3.ino                               :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/09/30 21:48:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   This is the Main file of the Soft Tracker Project. This firmware edition is adapted for      */
/*   ESP32 microcontrollers. The firmware allows User to track an approximate location of         */
/*   ESP-based devices via Telegram chat notifications. More details in the ReadMe file.          */
/*                                                                                                */
/* ********************************************************************************************** */

#include "header.h"

void  setup(void)
{
    #ifdef DEBUG
        Serial.begin(115200);
    #endif
    DEBUG_PRINTF("\n\n\nDEVICE START\n\n", "");
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_11db);
    esp_sleep_enable_timer_wakeup(g_for_this_long);
    esp_task_wdt_init(WD_TIMEOUT, true);                                      // watchdog
    ft_spiffs_init();
    WiFi.persistent(true);                                                    // Save WiFi configuration in flash - optional
    WiFi.mode(WIFI_STA);
    WiFi.hostname("SoftTraker");
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
    ft_wifi_list();
    if (wifiMulti.run(CONNECT_TIMEOUT) == WL_CONNECTED) 
    {
        ft_power_down_recovery();
        ft_send_location();
        ft_battery_notification();
        ft_check_incomming_messages(WAIT_FOR_MESSAGES_LIMIT);
    }
    else
    {
        ft_wifi_scan();
        g_last_wifi = 0;
    }
    ft_go_to_sleep();
}

void  loop(void) { /* NOTHING IS HERE */ }
 
