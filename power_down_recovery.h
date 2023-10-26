/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   power_down_recovery.h                                            :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/09/20 18:48:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   This function shows welcome message after the device turned on and resets the RTC            */
/*   variables values.                                                                            */
/*                                                                                                */
/* ********************************************************************************************** */

void  ft_power_down_recovery(void)
{
    RTC_DATA_ATTR static long power_loss_detector;
    esp_reset_reason_t        reason;

    if (power_loss_detector != 1110111)
    {
        g_last_wifi = 0;
        g_offline_wakeups = 0;
        g_reboot = false;
        ft_delete_spiffs_file("/offline_tracking_list.txt");
        power_loss_detector = 1110111;
    }
    reason = esp_reset_reason();
    switch (reason)
    {
        case ESP_RST_POWERON:
        case ESP_RST_BROWNOUT:
            DEBUG_PRINTF("\nReset reason: Power-on or Brown-out reset\n", "");
            bot.sendMessage(CHAT_ID, "Hello! I am the Soft Location Tracker. I am ON and ready for work!", "");
            break;
        case ESP_RST_SW:
            DEBUG_PRINTF("\nReset reason: Software reset\n", "");
            bot.sendMessage(CHAT_ID, ("I've successfully rebooted. Current software version " + String(SOFTWARE_VERSION)), "");
            delay(1000);
            bot.sendMessage(CHAT_ID, "Is there anything else I can do for you?", "");
            ft_check_incomming_messages(0);
            ft_go_to_sleep();
            break;
        case ESP_RST_PANIC:
            DEBUG_PRINTF("\nReset reason: Panic/exception reset\n", "");
            bot.sendMessage(CHAT_ID, "Don't panic, but my Core has just panicked!", "");
            delay(3000);
            bot.sendMessage(CHAT_ID, "You were trying to update me, weren't you? Well, let's try again", "");
            delay(2000);
            ft_check_incomming_messages(ft_ota_mode(CHAT_ID));
            ft_go_to_sleep();
            break;
    }
}
