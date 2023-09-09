/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   power_down_recovery.h                                            :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/06/28 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/06/29 18:48:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   This function shows welcome message after the device turned on and resets the RTC            */
/*   variables values.                                                                            */
/*                                                                                                */
/* ********************************************************************************************** */

void  ft_power_down_recovery(void)
{
    bot.sendMessage(CHAT_ID, "Hello! I am Soft Location Tracker. I am ON and ready for work!", "");
    rtcMng.last_wifi = 0;
    ft_clear_scan_results();
}
