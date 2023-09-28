/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   ReadMe.h                                                         :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/09/15 08:09:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   This file contains only notes.                                                               */
/*                                                                                                */
/* ************************************************************************************************
  
          ABOUT THE PROJECT
  
      This firmware allows Users to track an approximate location of ESP32-based devices via
      Telegram chat notifications.

      This particular repository contains firmware for the ESP32 microprocessor/devboards.
      How is it different from the ESP8266 / ESP8285 firmware?
        - different set of libraries,
        - different way to handle security certificate,
        - different way to enable deep sleep,
        - different way to store variables in RTC memory,
        - different way to measure battery charge,
        - different way to use watchdog.
      
      How it works:
      
      ESP32 connects to already known Wi-Fi networks and informs a pre-set Telegram chat of
      that fact. For it to work Users must manually input the credentials of the Wi-Fi networks
      they wish to be notified about, as well as the Telegram chat ID they wish to be notified
      to. Users also should add a personalized message to every inputed Wi-Fi network to indicate
      the network location. The device will check its connection status at most as frequently as
      once per hour and notify the chat if it manages to connect. The firmware does not have
      access to any other means of location tracking.

      Newly added functionality: Offline tracking. If ESP32 wakes up but cannot find
      familiar networks to connect to, it records the names of the networks it can "see" around
      into a well-structured list inside of its memory. When ESP32 eventually connects to a known 
      Wi-Fi network, not only it states its location, but also prints out the list of networks
      the device "saw" since the previous successfull connection. Since most of Wi-Fi networks are 
      named after the venues or places they belong to, the functionality allows Users to track
      the device route by simply googling the networks names even through places where no known
      Wi-Fi networks were available.
      
      Considering that the Soft Location Tracker cannot track the exact location at the exact time,
      it provides a solid layer of privacy to anything or anyone tracked by such a device, hence the
      name. At the same time it allows Users to reliably backtrack the Tracker's movements.

      Features:

          - multiple Wi-Fi enabled — every program cycle the Tracker tries to connect to all known
            Wi-Fi networks instead of just one of them;
          - antispam — if the Tracker finds himself connected to the same Wi-Fi network it was
            connected to during the previous cycle, it will not send any notifications, preventing
            spamming Users with repetative messages;
          - Offline tracking — even when the Tracker cannot get online it keeps tracking its
            movements by recording all Wi-Fi networks it "sees" on its way;
          - battery status notifications — low battery charge will not go unnoticed by Users, since
            the Tracker can ask them to charge it via Telegram chat;
          - user commands — Users can control some functionality of the Tracker by sending it commands
            via Telegram chat; e.g. add new locations to track right from the Telegram chat and it
            will be stored in the microcontroller memory, your ESP32 will automatically use it next
            time it wakes up — no hardcoding necessary;
          - OTA update — no need to take the Tracker out from wherever you put it to update it! Simply
            open the Telegram chat, command the Tracker to start updating and it will send you a link 
            to open with your web-browser and to drop a binary file with new firmware there.
      
      What applications it can have:
      
          - parents can be sure their children successfully reached school and then         
            came back home after the lessons without invading the children's privacy,
          - companies can track location of their property without compromising the
            privacy of the employees who are being in possesion of the property,
          - senders can be notified of their valuable parcels having reached the intended
            destination without need of a confirmation from the receivers,
          - extravert people who want all their friends to know that they are visiting
            some venue,
          - etc.

      The Soft Location Tracker hardware:
      
      - ESP32 module;
      - LiPol Battery, *230mAh, 3.7V   
      * charge-measuring function in other.h uses capacity-specific constants, hence the 230mAh capacity
        is important. To use batteries with different capacities, the constants need to be recalculated,
        otherwise the readings will not be precise. See the instructions at the bottom of the page.

      Future development:

      - make possible for Users to set the system messages language from Telegram chat;
      - make possible for Users to add new Wi-Fi networks and custom messages for them from Telegram chat
        and save them into EEPROM;
      - add functionality to output the list of saved Wi-Fi networks when requested from Telegram chat;
      - add functionality to delete individual saved Wi-Fi networks from the list from Telegram chat;
      - make possible for Users to assign a name or an ID to individual Trackers from Telegram chat;
      - refactor unused functionality from the UniversalTelegramBot library to empty memory space.
      

      This sketch was firstly written as a "toy" for my Embedded Development group chat in
      Telegram. It would allow the group chat members to track where I was without discovering
      my precise location.
      
      The project is based on Brian Lough's Universal Telegram Bot Library:
      https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot

      The OTA functionality added using AsyncElegantOTA as well as ESPAsyncWebServer libraries:
      https://github.com/ayushsharma82/AsyncElegantOTA
      https://github.com/me-no-dev/ESPAsyncWebServer


  ***********************************************************************************************


          NOTES:

      Do not call ft_go_to_sleep() function from ft_check_incomming_messages(), 
      ft_new_messages() and ft_ota_mode() functions! It causes the Telegram bot messages queue
      to get stuck on the same last message!

      When updating via OTA, the new software gets downloaded into the OTA memory partition
      first. So, in Arduino IDE, choosing any of "No OTA" partition schemes will block OTA functionality. 

      If your ESP reliably fails to connect to certain Wi-Fi hotspots, increase the CPU frequency
      to 160 Mhz. If that does not solve the issue, make sure the network runs on 2.4Ghz. ESPs do not
      support 5Ghz networks.
      
    
  ***********************************************************************************************    


      EXAMPLE OF THE credentials.h FILE
  
      #define BOTtoken      "xxxxxxxxxx:xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"     // Telegram Bot Token
      #define CHAT_ID       "-xxxxxxxxxxxxx"                                     // Telegram chat ID

      #define OTA_PASSWORD  1234

      #define BACKUP_SSID1         "home_wifi_name"
      #define BACKUP_PASSWORD1     "home_wifi_password"
    
      #define BACKUP_SSID2         "office_wifi_name"
      #define BACKUP_PASSWORD2     "office_wifi_password"
    
      #define BACKUP_SSID3         "friends_wifi_name"
      #define BACKUP_PASSWORD3     "friends_wifi_password"
     
   
  ***********************************************************************************************


      CALCULATING CONSTANTS FOR BATTERY CHARGE FUNCTION

      First, you need to find out so called Battery_Max_Reading. Fully charge your battery,
      connect it to your ESP, measure the battery state with "adc1_get_raw(ADC1_CHANNEL_0)" command
      and output the measurements into Serial monitor. Battery_Max_Reading is the first number
      outputed to Serial monitor. You will need it later.

      Second, you need to find out so called Battery_Min_Reading. Let ESP connected to Serial 
      monitor until the battery drowns down and ESP browns-out. Battery_Min_Reading is the last 
      number outputed to Serial monitor.

      The 1st constant equals to Battery_Min_Reading. You already have that value. In this sketch
      it equals to 3040 for a 230mAh battery. For other batteries the value may be different.

      The 2nd constant is so called Battery_Coefficient. In this sketch Battery_Coefficient equals
      to 12.22 for a 230mAh battery. For other batteries the number may be different.
      Battery_Coefficient needs to be calculated by the following formula:

      Battery_Coefficient == (Battery_Max_Reading - Battery_Min_Reading) / 100


      Put the obtained constants into the sketch accordingly:

      battery = ceil((adc1_get_raw(ADC1_CHANNEL_0) - Battery_Min_Reading) / Battery_Coefficient);
    

  *********************************************************************************************** */
