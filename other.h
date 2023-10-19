/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   other.h                                                          :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/10/03 18:09:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   This file contains all the little utility functions that are not too important to have       */
/*   their own personal files. They are not necesarrily connected with each other logically.      */
/*                                                                                                */
/* ********************************************************************************************** */

void  ft_go_to_sleep(void)
{
    esp_task_wdt_reset();
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

short  ft_battery_notification(void)
{
    short battery;

    battery = ft_battery_check();
    DEBUG_PRINTF("Current battery state is %d%%\n", battery);
    if (battery <= 15)
    {
        bot.sendMessage(CHAT_ID, "My battery is quite low. Please, charge me when you have time!", "");
        return (WAIT_FOR_MESSAGES_LIMIT);
    }
    else if (battery >= 100)
    {
        bot.sendMessage(CHAT_ID, "I see I'm being charged. I will stay awake until the charging is complete just in case you need something from me.", "");
        return (-32767);
    }
    else
        return (WAIT_FOR_MESSAGES_LIMIT);                                         // WAIT_FOR_MESSAGES_LIMIT == check new messages only 1 time
}
 
