/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   send_location.cpp                                                :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/11/12 11:40:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   This function checks what Wi-Fi network the device is being connected to and sends an        */
/*   according message into a Telegram chat.                                                      */
/*                                                                                                */
/* ********************************************************************************************** */

#include "header.h"

void  IRAM_ATTR ft_send_location(void)
{
    bool    send_tracking_list;
    String  ssid;
    String  message;

    esp_task_wdt_reset();
    send_tracking_list = false;
    if ((g_last_wifi == 0) && LittleFS.exists("/offline_tracking_list.txt"))
        send_tracking_list = true;
    ssid = WiFi.SSID();
    DEBUG_PRINTF("WiFi connected to %s\n", ssid.c_str());
    if ((ssid == SSID1) && (g_last_wifi != 1)) 
    {
        g_last_wifi = 1;
        message = MESSAGE1;
    }
    if ((ssid == SSID2) && (g_last_wifi != 2)) 
    {
        g_last_wifi = 2;
        message = MESSAGE2;
    }
    if ((ssid == SSID3) && (g_last_wifi != 3)) 
    {
        g_last_wifi = 3;
        message = MESSAGE3;
    }
    if ((ssid == SSID4) && (g_last_wifi != 4)) 
    {
        g_last_wifi = 4;
        message = MESSAGE4;
    }
    if ((ssid == SSID5) && (g_last_wifi != 5)) 
    {
        g_last_wifi = 5;
        message = MESSAGE5;
    }
    if ((ssid == SSID6) && (g_last_wifi != 6)) 
    {
        g_last_wifi = 6;
        message = MESSAGE6;
    }
    if ((ssid == SSID7) && (g_last_wifi != 7)) 
    {
        g_last_wifi = 7;
        message = MESSAGE7;
    }
    if ((ssid == SSID8) && (g_last_wifi != 8)) 
    {
        g_last_wifi = 8;
        message = MESSAGE8;
    }
    if ((ssid == SSID9) && (g_last_wifi != 9)) 
    {
        g_last_wifi = 9;
        message = MESSAGE9;
    }
    if ((ssid == SSID10) && (g_last_wifi != 10)) 
    {
        g_last_wifi = 10;
        message = MESSAGE10;
    }
    if ((ssid == SSID11) && (g_last_wifi != 11)) 
    {
        g_last_wifi = 11;
        message = MESSAGE11;
    }
    if ((ssid == SSID12) && (g_last_wifi != 12)) 
    {
        g_last_wifi = 12;
        message = MESSAGE12;
    }
    if ((ssid == SSID13) && (g_last_wifi != 13)) 
    {
        g_last_wifi = 13;
        message = MESSAGE13;
    }
    if (message)
        bot.sendMessage(CHAT_ID, message, "");
    message.clear();
    if (send_tracking_list)
    {
        message = ft_write_report_message();
        bot.sendMessage(CHAT_ID, message, "");
        ft_delete_spiffs_file("/offline_tracking_list.txt");
    }
    esp_task_wdt_reset();
}
 
