/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   power_down_recovery.h                                            :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/09/10 18:48:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   This function shows welcome message after the device turned on and resets the RTC            */
/*   variables values.                                                                            */
/*                                                                                                */
/* ********************************************************************************************** */

void  ft_power_down_recovery(void)
{
    RTC_DATA_ATTR static long  power_loss_detector;

    if (power_loss_detector != 1110111)
    {
        bot.sendMessage(CHAT_ID, "Hello! I am Soft Location Tracker. I am ON and ready for work!", "");
        g_last_wifi = 0;
        g_offline_wakeups = 0;
        ft_delete_spiffs_file("/offline_tracking_list.txt");
        power_loss_detector = 1110111;
    }
}
