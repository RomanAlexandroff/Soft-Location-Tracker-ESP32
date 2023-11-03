# The Soft Location Tracker

## About the Project
  
This firmware allows Users to track an approximate location of ESP32-based devices via Telegram chat notifications.
      
How it works:
      
ESP32 connects to already known Wi-Fi networks and informs a pre-set Telegram chat of that fact. For it to work Users must manually input the credentials of the Wi-Fi networks they wish to be notified about, as well as the Telegram chat ID they wish to be notified to. Users also should add a personalized message to every inputed Wi-Fi network to indicate the network location. The device will check its connection status at most as frequently as once per hour and notify the chat if it manages to connect. The firmware does not have access to any other means of location tracking.

Newly added functionality: Offline tracking. If ESP32 wakes up but cannot find familiar networks to connect to, it records the names of the networks it can "see" around into a well-structured list inside of its memory. When ESP32 eventually connects to a known Wi-Fi network, not only it states its location, but also prints out the list of networks the device "saw" since the previous successfull connection. Since most of Wi-Fi networks are named after the venues or places they belong to, the functionality allows Users to track the device route by simply googling the networks names even through places where no known Wi-Fi networks were available.
      
Considering that the Soft Location Tracker cannot track the exact location at the exact time, it provides a solid layer of privacy to anything or anyone tracked by such a device, hence the name. At the same time it allows Users to reliably backtrack the Tracker's movements.

What applications it can have:
      
- parents can be sure their children successfully reached school and then came back home after the lessons without invading the children's privacy,
- companies can track location of their property without compromising the privacy of the employees who are being in possesion of the property,
- senders can be notified of their valuable parcels having reached the intended destination without need of a confirmation from the receivers,
- extravert people who want all their friends to know that they are visiting some venue,
- etc.


## Features

- multiple Wi-Fi enabled — every program cycle the Tracker tries to connect to all known Wi-Fi networks instead of just one of them;
- antispam — if the Tracker finds himself connected to the same Wi-Fi network it was connected to during the previous cycle, it will not send any notifications, preventing spamming Users with repetative messages;
- Offline tracking — even when the Tracker cannot get online it keeps tracking its movements by recording all Wi-Fi networks it "sees" on its way;
- battery status notifications — low battery charge will not go unnoticed by Users, since the Tracker can ask them to charge it via Telegram chat;
- user commands — Users can control some functionality of the Tracker by sending it commands via Telegram chat; e.g. add new locations to track right from the Telegram chat and it will be stored in the microcontroller memory, your ESP32 will automatically use it next time it wakes up — no hardcoding necessary;
- OTA update — no need to take the Tracker out from wherever you put it to update it! Simply open the Telegram chat, command the Tracker to start updating and it will send you a link to open with your web-browser and to drop a binary file with new firmware there.
 
 
## Hardware

A single device requires the following components:
- ESP32-C3 Module,
- Li-Ion 230 mAh Battery with protection


## Ports

This repository contains firmware for ESP32. The ESP8266 / ESP8285 version can be found [HERE](https://github.com/RomanAlexandroff/Soft-Location-Tracker). 

 
## Contributions
 
Contributions to the Soft Location Tracker project are welcome! If you have any enhancements, bug fixes, or new features to contribute, please submit a pull request. Additionally, feel free to open issues for any questions, suggestions, or bug reports.
 
 
## Future Development

- make possible for Users to set the system messages language from Telegram chat;
- add functionality to delete individual saved Wi-Fi networks from the list from Telegram chat (right now it is only possible to delete the whole list);
- make possible for Users to assign a name or an ID to individual Trackers from Telegram chat;
- refactor unused functionality from the UniversalTelegramBot library to empty memory space
 
 
## License

The Soft Location Tracker is released under the MIT License. Please refer to the LICENSE file for more information.  


## Regards
      
The Telegram-related functionality is possible thanks to Brian Lough's [Universal Telegram Bot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot) library.

The OTA functionality added using [ElegantOTA](https://github.com/ayushsharma82/ElegantOTA) as well as [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) libraries.
 

## Example of the credentials.h File

```  
#define BOTtoken   "xxxxxxxxxx:xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" // Telegram Bot Token
#define CHAT_ID    "-xxxxxxxxxxxxx"                                 // Telegram chat ID

#define OTA_PASSWORD         "1234"

#define BACKUP_SSID1         "home_wifi_name"
#define BACKUP_PASSWORD1     "home_wifi_password"
    
#define BACKUP_SSID2         "office_wifi_name"
#define BACKUP_PASSWORD2     "office_wifi_password"
    
#define BACKUP_SSID3         "friends_wifi_name"
#define BACKUP_PASSWORD3     "friends_wifi_password"
```
 
 
## Calculating Constants for the Battery Charge Function

You will need to check the following little utility:
https://github.com/RomanAlexandroff/ESP-Battery-Charge-Utility
      
It was designed to be a universal solution for battery charge level detection in ESP-based projects and has all the instructions inside of it.