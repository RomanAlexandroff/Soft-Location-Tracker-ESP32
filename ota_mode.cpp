/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   ota_mode.cpp                                                     :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/11/12 11:40:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   This file allowes to update the microntroller wirelessly, simply by sending a compiled       */
/*   binary file into its file system. The microcontroller opens a gate, accessable through       */
/*   any web-browser via a link, that is provided by the microcontroller through both Serial      */
/*   port and Telegram chat.                                                                      */
/*   NOTE! When updating, the new software gets downloaded into the OTA memory partition first.   */
/*   So, in Arduino IDE, choosing any "No OTA" partition scheme will block OTA functionality.     */
/*                                                                                                */
/* ********************************************************************************************** */

#include "header.h"

short ft_ota_mode(String chat_id);
void  ft_check_incomming_messages(short cycles);

inline void ft_compose_message(String ssid, IPAddress ip, String chat_id)
{
    String  message;

    message = "OTA mode activated. \n\nConnected to\n" + String(ssid);
    message += "\n\nConnect to this Wi-Fi and go to the link\nhttp://";
    message += String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]) + "/update";
    message += "\n\nTo cancel the OTA update write any other command.";
    bot.sendMessage(chat_id, message, "");
}

inline void ft_ElegantOTA_callbacks(String chat_id)
{
    ElegantOTA.onStart([chat_id]() {
        DEBUG_PRINTF("\nOTA update process started.\n", "");
        bot.sendMessage(chat_id, "Receiving new firmware...", "");
    });
    ElegantOTA.onEnd([chat_id](bool success) {
        if (success)
        {
            DEBUG_PRINTF("OTA update completed successfully.", "");
            bot.sendMessage(chat_id, "Success! OTA update is complete", "");
        }
        else
        {
            DEBUG_PRINTF("OTA update failed.", "");
            bot.sendMessage(chat_id, "Oups! Something went wrong and OTA update was not completed", "");
            delay(1000);
            bot.sendMessage(chat_id, "Let's try again", "");
            ft_check_incomming_messages(ft_ota_mode(CHAT_ID));
        }
    });
}

short  ft_ota_mode(String chat_id) 
{
    String      ssid;
    IPAddress   ip;

    esp_task_wdt_reset();
    ssid = WiFi.SSID();
    ip = WiFi.localIP();
    DEBUG_PRINTF("\n\nSOFT TRACKER\nOTA update mode initialized.\n\n", "");
    DEBUG_PRINTF("Wi-Fi network: %s\n", ssid.c_str());
    DEBUG_PRINTS("IP address: %d.%d.%d.%d\n\n", ip[0], ip[1], ip[2], ip[3]);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/plain", "Soft Tracker OTA update."); });        //Starting the OTA server
    ElegantOTA.begin(&server);
    ft_ElegantOTA_callbacks(chat_id);
    server.begin();
    DEBUG_PRINTF("HTTP server started.\nConnect to the stated Wi-Fi network and proceed to the link\n\n", "");
    DEBUG_PRINTS("http://%d.%d.%d.%d/update\n\n\n", ip[0], ip[1], ip[2], ip[3]);
    ft_compose_message(ssid, ip, chat_id);
    return (-32767);
}
 
