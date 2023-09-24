/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   offline_tracking.h                                               :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/17 08:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/09/17 12:48:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   In case the device wakes up and cannot find any already known Wi-Fi networks, the code in    */
/*   this file searches for Wi-Fi networks at the location of the device and stores the names     */
/*   of the found networks in the RTC memory. When the device eventually connects to a known      */
/*   Wi-Fi network, not only it states its location, but also prints out the list of networks     */
/*   the device "saw" between the last two successfull connections.                               */
/*                                                                                                */
/* ********************************************************************************************** */

String  ft_write_report_message(void)
{
    String  message;

    if (!SPIFFS.exists("/offline_tracking_list.txt"))
        message = " ";
    else
    {
        message = "\n\nBetween this and the previous report I was taken to " + String(g_offline_wakeups);   
        message += " unknown locations. Until now I could not connect to a network to let you know ";
        message += "but I made you a list of Wi-Fi neworks I saw on the way. ";
        message += "By googling their names you may discover the route I traveled.\n";
        message += ft_read_spiffs_file("/offline_tracking_list.txt");
    }
    return (message);
}

void  IRAM_ATTR ft_wifi_scan(void)
{
    int     i;
    int     quantity;
    String  input;

    i = 0;
    DEBUG_PRINTF("Starting WiFi scan.../n", "");
    quantity = WiFi.scanNetworks(false, true);
    if (quantity == 0)
    {
        DEBUG_PRINTF("No networks found\n", "");
    }
    else if (quantity > 0)
    {
        DEBUG_PRINTF("%d networks found\n", quantity);
        g_offline_wakeups++;
        input = "\n   Location number " + String(g_offline_wakeups) + ":\n";
        while (i < quantity)
        {
            input += WiFi.SSID(i) + "\n";
            i++;
        }
        ft_write_spiffs_file("/offline_tracking_list.txt", input);
    }
    else
        DEBUG_PRINTF("WiFi scan error %d", quantity);
}
 
