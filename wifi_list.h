/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   wifi_list.h                                                      :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/06/28 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/06/29 18:48:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   List of known Wi-Fi networks in accordance with the ESP8266WiFiMulti library. More           */
/*   networks credentials can be added in an identical fasion ("SSID", "password"). The           */
/*   list must be called before the wifiMulti.run() function, which initiates connection.         */
/*                                                                                                */
/* ********************************************************************************************** */

void  IRAM_ATTR ft_wifi_list(void)
{
    wifiMulti.addAP(SSID1, PASSWORD1);
    wifiMulti.addAP(SSID2, PASSWORD2);
    wifiMulti.addAP(SSID3, PASSWORD3);
    wifiMulti.addAP(SSID4, PASSWORD4);
    wifiMulti.addAP(SSID5, PASSWORD5);
    wifiMulti.addAP(SSID6, PASSWORD6);
    wifiMulti.addAP(SSID7, PASSWORD7);
    wifiMulti.addAP(SSID8, PASSWORD8);
    wifiMulti.addAP(SSID9, PASSWORD9);
    wifiMulti.addAP(SSID10, PASSWORD10);
    wifiMulti.addAP(SSID11, PASSWORD11);
    wifiMulti.addAP(SSID12, "");
    ESP.wdtFeed();
}
 
