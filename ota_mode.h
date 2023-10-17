/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   ota_mode.h                                                       :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/10/03 18:09:41                                ###    ###   ###     ###         */
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

short  ft_ota_mode(String chat_id) 
{
    String      ssid;
    IPAddress   ip;
    String      message;
    short       battery;
    int         i;

    esp_task_wdt_reset();
    ssid = WiFi.SSID();
    ip = WiFi.localIP();
    i = WAIT_FOR_OTA_LIMIT;
    battery = ft_battery_check();
    if (battery < 9)
    {
        DEBUG_PRINTF("\nThe battery is too low to perform an OTA update safely. Connect a charging cable to proceed.\n", "");
        message = "The battery is too low to perform an OTA update safely. ";
        message += "Connect a charging cable to proceed. I will wait as long as I can.";
        bot.sendMessage(chat_id, message, "");
        while (battery > 3 && battery < 9 && i)
        {
            battery = ft_battery_check();
            esp_task_wdt_reset();
            delay (999);
            i--;
        }
        if (battery < 3)
        {
            bot.sendMessage(chat_id, "Unfortunately, my battery is way too low to wait any longer. I'm turning off.", "");
            return (WAIT_FOR_MESSAGES_LIMIT);
        }
        message.clear();
    }
    DEBUG_PRINTF("\n\nSOFT TRACKER\nOTA update mode initialized.\n\n", "");
    DEBUG_PRINTF("Wi-Fi network: %s\n", ssid.c_str());
    DEBUG_PRINTS("IP address: %d.%d.%d.%d\n\n", ip[0], ip[1], ip[2], ip[3]);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/plain", "Soft Tracker OTA update."); });        //Starting the OTA server
    ElegantOTA.begin(&server);
    server.begin();
    DEBUG_PRINTF("HTTP server started.\nConnect to the stated Wi-Fi network and proceed to the link\n\n", "");
    DEBUG_PRINTS("http://%d.%d.%d.%d/update\n\n\n", ip[0], ip[1], ip[2], ip[3]);
    message = "OTA mode activated. \n\nConnected to\n" + String(ssid);
    message += "\nAssigned IP\n" + String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
    message += "\n\nConnect to the stated Wi-Fi network and go to the link\nhttp://";
    message += String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]) + "/update";
    message += "\n\nRemember that in OTA mode I will not go to sleep automatically.";
    message += " To cancel the OTA mode without firmware update use \"off\" or \"reboot\" commands";
    bot.sendMessage(chat_id, message, "");
    esp_task_wdt_reset();
    return (-32767);
}
 
