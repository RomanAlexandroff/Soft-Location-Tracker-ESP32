/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   wifi_recorder.h                                                  :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/09/10 18:48:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   In case the device wakes up and cannot find any already known Wi-Fi networks, the code in    */
/*   this file searches for Wi-Fi networks at the location of the device and stores the names     */
/*   of the found networks in the RTC memory. When the device eventually connects to a known      */
/*   Wi-Fi network, not only it states its location, but also prints out the list of networks     */
/*   the device "saw" between the last two successfull connections.                               */
/*                                                                                                */
/* ********************************************************************************************** */

void  ft_clear_scan_results(void)
{
    int  i;
    int  j;

    i = 0;
    j = 0;
    while (g_scan_results[i][0] != '\0')
    {
        while (g_scan_results[i][j] != '\0')
        {
            g_scan_results[i][j] = '\0';
            j++;
        }
        j = 0;
        i++;
    }
}

String  ft_write_report_message(void)
{
    int     i;
    int     j;
    String  message;

    i = 0;
    j = 0;
    message = "Between this and the previous report I was taken to unknown locations. ";   
    message += "Until now I could not connect to a network to let you know ";
    message += "but I made you a list of Wi-Fis I saw on the way. ";
    message += "By googling their names you may discover the route I traveled:\n";
    while (g_scan_results[i][0] != '\0')
    {
        message += "\n" + String(i + 1) + ". ";
        while (g_scan_results[i][j] != '\0')
        {
            message += String(g_scan_results[i][j]);
            j++;
        }
        j = 0;
        i++;      
    }
    return (message);
}

void  ft_scan_report(void)
{
    String  message;

    message = ft_write_report_message();
    bot.sendMessage(CHAT_ID, message, "");
    ft_clear_scan_results();
}

void  IRAM_ATTR ft_wifi_scan(void)
{
    int     i;
    int     j;
    int     quantity;
    String  ssid;

    i = 0;
    j = 0;
    DEBUG_PRINTF("Starting WiFi scan.../n", "");
    quantity = WiFi.scanNetworks(false, true);
    if (quantity == 0)
    {
        DEBUG_PRINTF("No networks found\n", "");
    }
    else if (quantity > 0)
    {
        DEBUG_PRINTF("%d networks found\n", quantity);
        while (g_scan_results[i][0] != '\0' && i < (MAX_NETWORKS - 6))       // this expression allows overwriting the last 5 entries in the list
            i++;              
        while (j < quantity && i < (MAX_NETWORKS - 1))
        {
            ssid = WiFi.SSID(j);
            ssid.toCharArray(g_scan_results[i], MAX_NAME_LENGTH);
            i++;
            j++;
        }
    }
    else
        DEBUG_PRINTF("WiFi scan error %d", quantity);
}
 
