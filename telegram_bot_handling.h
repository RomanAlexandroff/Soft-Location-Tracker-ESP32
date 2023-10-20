/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   telegram_bot_handling.h                                          :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/10/19 18:09:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   These functions are for checking on new Telegram messages, reading them and reacting to      */
/*   them accordingly. Waiting for new messages also serves as a loop for the OTA functions.      */
/*   WARNING! DO NOT CALL THE ft_go_to_sleep() FUNCTION FROM ANY OF THESE FUNCTIONS! THE DEVICE   */
/*   WOULD BECOME UNRESPONSIVE TO ANY MESSAGES FROM THE TELEGRAM CHAT!                            */
/*                                                                                                */
/* ********************************************************************************************** */

short  IRAM_ATTR ft_answer_engine(String chat_id, String text)
{
    short       cycles;
    String      message;
    static bool add_location_flag;

    esp_task_wdt_reset();
    #ifdef PRIVATE
    if (chat_id != CHAT_ID)
    {
        cycles = 0;
        bot.sendMessage(chat_id, "UNAUTHORIZED. ACCESS DENIED.\nThe device is unaccessable from this chat.\n\nhttps://www.youtube.com/watch?v=XV25MrouozE", "");
        return (cycles);
    }
    #endif 
    if (text == "/status")
    {
        cycles = 0;
        add_location_flag = false;
        message = "I am connected to " + String(WiFi.SSID());   
        message += ". Signal strength is " + String(WiFi.RSSI());
        message += " dBm. My battery is " + String(ft_battery_check()) + "% charged, ";
        message += "software version " + String(SOFTWARE_VERSION);
        bot.sendMessage(chat_id, message, "Markdown");
        return (cycles);
    }
    else if (text == "/location")
    {
        cycles = 0;
        add_location_flag = false;
        g_last_wifi = 0;
        ft_send_location();
        return (cycles);
    }
    else if (text == "/list locations")
    {
        cycles = 0;
        add_location_flag = false;
        if (!(ft_read_spiffs_file("/locations.txt")))
            message = "You have no saved locations just yet. Add them by using the /add location command";
        else
        {
            message = "Here are the locations I am keeping track of:\n\n";
            message += ft_read_spiffs_file("/locations.txt");
        }
        bot.sendMessage(chat_id, message, "");
        return (cycles);
    }
    else if (text == "/add location")
    {
        cycles = 0;
        add_location_flag = true;
        message = "You can add a new location for me to keep a track of and notify you about. ";
        message += "To do so simply write me the Wi-Fi name, password and the location description in your next message. ";
        message += "Also, please, start the message with the \"/\" sign and separate the pieces of information with a comma. ";
        message += "For example, your message could look like this:";
        message += "\n\n/CoffeeCafe Wi-Fi Free, LoveCoffee123, At Coffee Cafe in the city centre of Amsterdam, Netherlands";
        message += "\n\nIf you don't want to add any locations just yet, simply write me any other command.";
        bot.sendMessage(chat_id, message, "");
        return (cycles);
    }
    else if (text == "/delete locations")
    {
        cycles = -32767;
        message = "All of your saved locations are about to be errased from my memory. Before it happens, I will output them into this chat, ";
        message += "so you could restore some of them in case you needed to. Here we go:";
        bot.sendMessage(chat_id, message, "");
        bot.sendMessage(chat_id, ft_read_spiffs_file("/locations.txt"), "");
        ft_delete_spiffs_file("/locations.txt");
        message.clear();
        message = "Done! You have no saved locations!\n\nNow, before I turn off, you must add at least one location. ";
        message += "Otherwise I would not be able to connect to any Wi-Fi and you or anyone else would not be able to use me! ";
        message += "This cannot be done later.";
        bot.sendMessage(chat_id, message, "");
        message.clear();
        message = "It can be any location. Your home Wi-Fi or your office Wi-Fi would do just fine. ";
        message += "Simply write me the Wi-Fi name, password and the location description in your next message. ";
        message += "Please, start the message with the \"/\" sign and separate the pieces of information with a comma. ";
        message += "For example, your message could look like this:";
        message += "\n\n/My_Home_WiFi, HomeSweetHome123, At home in Brussels, Belgium";
        bot.sendMessage(chat_id, message, "");
        add_location_flag = true;
        return (cycles);
    }
    else if (text == "/ota")
    {
        cycles = 0;
        add_location_flag = false;
        bot.sendMessage(chat_id, "To activate OTA Firmware Update enter your Developer Password", "");
        return (cycles);
    }
    else if (text == ("/" + String(OTA_PASSWORD)) || text == ("/ota " + String(OTA_PASSWORD)))
    {
        add_location_flag = false;
        bot.sendMessage(chat_id, "Password accepted.", "");
        cycles = ft_ota_mode(chat_id);
        return (cycles);
    }
    else if (text == "/reboot")
    {
        bot.sendMessage(chat_id, "Rebooting!", "");
        g_reboot = true;
        cycles = WAIT_FOR_MESSAGES_LIMIT;
        return (cycles);
    }
    else if (text == "/off")
    {
        bot.sendMessage(chat_id, "Switching off!", "");
        cycles = WAIT_FOR_MESSAGES_LIMIT;
        return (cycles);
    }
    else
    {
        cycles = 0;
        if (add_location_flag)
        {
            add_location_flag = false;
            message = text;
            message.remove(0, 1);
            ft_write_spiffs_file("/locations.txt", message);
            bot.sendMessage(chat_id, "I've added a new location for you. You can confirm it by calling the \"list locations\" command", "");
        }
        else
        {
            bot.sendMessage(chat_id, "I'm sorry, I don't understand", "");
            bot.sendMessage(chat_id, "Try one of the following commands: status, location, list locations, add location, delete locations, ota, reboot, off. Every command should start with \"/\" sign", "");
        }
        return (cycles);
    }
    return (cycles);
}

short ft_new_messages(short numNewMessages)
{
    short   cycles;
    String  chat_id;
    String  text;
    String  from_name;

    esp_task_wdt_reset();
    DEBUG_PRINTF("\nHandling new messages\n", "");
    DEBUG_PRINTF("Number of messages to handle: %d\n", numNewMessages);
    for (short i = 0; i < numNewMessages; i++) 
    {
        DEBUG_PRINTF("Handling loop iterations: i = %d\n", i);
        chat_id = String(bot.messages[i].chat_id);
        text = bot.messages[i].text;
        from_name = bot.messages[i].from_name;
        DEBUG_PRINTF("%s says: ", from_name.c_str());
        DEBUG_PRINTF("%s\n\n", text.c_str());
        cycles = ft_answer_engine(chat_id, text);
    }
    esp_task_wdt_reset();
    return (cycles);
}

void  ft_check_incomming_messages(short cycles)
{
    short numNewMessages;

    esp_task_wdt_reset();
    while (cycles <= WAIT_FOR_MESSAGES_LIMIT)
    {
        ElegantOTA.loop();
        DEBUG_PRINTF("Waiting for incomming commands from Telegram chat. Waiting loop cycles: %d\n", cycles);       
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        while (numNewMessages)
        {
            cycles = ft_new_messages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }
        if ((cycles + 25) == WAIT_FOR_MESSAGES_LIMIT)
            bot.sendMessage(CHAT_ID, "Going to sleep in 1 minute. To keep me awake, write me anything", "");
        if ((cycles + 1) == WAIT_FOR_MESSAGES_LIMIT)
            bot.sendMessage(CHAT_ID, "Good talk! I'm off now", "");
        cycles++;
    }
}
 
