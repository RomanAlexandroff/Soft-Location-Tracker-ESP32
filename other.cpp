/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   other.cpp                                                        :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/11/12 11:40:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   This file contains all the little utility functions that are not too important to have       */
/*   their own personal files. They are not necesarrily connected with each other logically.      */
/*                                                                                                */
/* ********************************************************************************************** */

#include "header.h"

void  ft_go_to_sleep(void)
{
    esp_task_wdt_reset();
    if (g_reboot)
        esp_restart();
    DEBUG_PRINTF("\nGoing to sleep for %d minute(s)\n", (g_for_this_long / 60000000));
    DEBUG_PRINTF("The device was running for %d second(s) this time\n", (millis() / 1000));
    DEBUG_PRINTF("\nDEVICE STOP\n\n\n", "");
    esp_deep_sleep_start();
}

short  ft_battery_check(void)
{
    short i;
    short battery;

    i = 4;
    while (i)
    {
        battery += ceil((adc1_get_raw(ADC1_CHANNEL_0) - 650) / 12.62);            // see ReadMe regarding these constants
        i--;
    }
    battery = battery / 4;                                                        // counting average of 4 samples
    if (battery <= 0)
        battery = 0;
    if (battery >= 100)
        battery = 100;
    return (battery);
}

void  ft_battery_notification(void)
{
    short   battery;
    String  charge;

    esp_task_wdt_reset();
    battery = ft_battery_check();
    charge = String(battery);
    DEBUG_PRINTF("Current battery state is %d%%\n", battery);
    if (battery <= 10)
        bot.sendMessage(CHAT_ID, String("My battery is low — " + charge + "%. Please, charge me when you have time!"), "");
    if (battery == 100 && esp_reset_reason() == ESP_RST_POWERON)
        bot.sendMessage(CHAT_ID, "My battery is 100% charged", "");
}
 
