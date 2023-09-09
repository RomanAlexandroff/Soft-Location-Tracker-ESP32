/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   other.h                                                          :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/06/28 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/06/29 18:48:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   This file contains all the little utility functions that are not too important to have       */
/*   their own personal files. They are not necesarrily connected with each other logically.      */
/*                                                                                                */
/* ********************************************************************************************** */

void  ft_go_to_sleep(void)
{
    ESP.wdtFeed();
    system_rtc_mem_write(64, &rtcMng, sizeof(rtcMng));
    DEBUG_PRINTF("\nGoing to sleep for %d minute(s)\n", (g_for_this_long / 60000000));
    DEBUG_PRINTF("The device was running for %d second(s) this time\n", (millis() / 1000));
    DEBUG_PRINTF("\nDEVICE STOP\n\n\n", "");
    ESP.deepSleep(g_for_this_long, WAKE_NO_RFCAL);
}

short  ft_battery_check(void)
{
    short  battery;
      
    battery = ceil((ESP.getVcc() - 3040) / 12.22);             // see ReadMe regarding the constants
    if (battery <= 0)
        battery = 0;
    if (battery >= 100)
        battery = 100;
    return (battery);
}
 
