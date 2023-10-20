/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   wifi_list.h                                                      :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/10/03 18:09:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   List of known Wi-Fi networks in accordance with the ESP8266WiFiMulti library. More           */
/*   networks credentials can be added in an identical fasion ("SSID", "password"). The           */
/*   list must be called before the wifiMulti.run() function, which initiates connection.         */
/*                                                                                                */
/* ********************************************************************************************** */

void  ft_backup_connection(void)
{
    String      message;

    esp_task_wdt_reset();
    DEBUG_PRINTF("I will try to connect to the back-up networks, so you can examine the issue. ", "");
    DEBUG_PRINTS("Here is the list of my back-up networks:\n%s,\n%s,\n%s", String(BACKUP_SSID1), String(BACKUP_SSID2), String(BACKUP_SSID3), "");
    wifiMulti.addAP(BACKUP_SSID1, BACKUP_PASSWORD1);
    wifiMulti.addAP(BACKUP_SSID2, BACKUP_PASSWORD2);
    wifiMulti.addAP(BACKUP_SSID3, BACKUP_PASSWORD3);
    if (wifiMulti.run(CONNECT_TIMEOUT) == WL_CONNECTED) 
    {
        message = "My locations list is empty, so I connected to my back-up Wi-Fi: " + String(WiFi.SSID());
        bot.sendMessage(CHAT_ID, message, "");
    }
    else
        ft_go_to_sleep();
}

void  IRAM_ATTR ft_wifi_list(void)
{
    short       i;
    short       j;
    String      output;
    char        buffer[256];
    const char* data;
    char        ssid[128];
    char        password[128];

    i = 1;
    esp_task_wdt_reset();
    DEBUG_PRINTF("\n\nInitializing Wi-Fi networks credentials.\n", "");
    File file = LittleFS.open("/locations.txt", "r");
    while (!file && i <= 2)
    {
        DEBUG_PRINTF("An error occurred while opening locations.txt file for reading in LittleFS. Retry #%d.\n", i);
        File file = LittleFS.open("/locations.txt", "r");
        i++;
        delay(100);
    }
    if (!file)
    {
        DEBUG_PRINTF("Failed to open locations.txt file for reading in LittleFS even after %d retries. The file dependant function will be unavailable during this programm cycle.\n", (i - 1));
        file.close();
        ft_backup_connection();
    }  
    else
    {
        while (file.available())
        {
            DEBUG_PRINTF("\nExtracting Wi-Fi networks credentials from the locations.txt file...", "");
            output = file.readStringUntil('\n');
            output.trim();
            output.toCharArray(buffer, sizeof(buffer));
            data = buffer;
            DEBUG_PRINTF("\nreading the line, current data value: %s", data);
            i = 0;
            j = 0;
            while (data[i] != ',' && i < sizeof(ssid) - 1)
            {
                ssid[i] = data[i];
                i++;
            }
            ssid[i] = '\0';
            i += 2;
            while (data[i] != ',' && j < sizeof(password) - 1)
            {
                password[j] = data[i];
                i++;
                j++;
            }
            password[j] = '\0';
            DEBUG_PRINTF("\nSSID: %s", ssid);
            DEBUG_PRINTF("\nPassword: %s\n", password);
            wifiMulti.addAP(ssid, password);
        }
        file.close();
    }
    wifiMulti.addAP(BACKUP_SSID1, BACKUP_PASSWORD1);
    wifiMulti.addAP(BACKUP_SSID2, BACKUP_PASSWORD2);
    wifiMulti.addAP(BACKUP_SSID3, BACKUP_PASSWORD3);
    esp_task_wdt_reset();
}
 
