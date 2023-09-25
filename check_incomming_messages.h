/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   check_incomming_messages.h                                       :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/09 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/09/18 08:48:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   These functions are for checking on new Telegram messages, reading them and reacting to      */
/*   them accordingly.                                                                            */
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
        message += " dBm. My battery is " + String(ft_battery_check()) + "% charged";
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
        message = "Here are the locations I am keeping track of for you:\n\n";
        message += ft_read_spiffs_file("/locations.txt");
        bot.sendMessage(chat_id, message, "");
        return (cycles);
    }
    else if (text == "/add location")
    {
        cycles = 0;
        add_location_flag = true;
        message = "You can add a new location for me to keep a track of and notify you about. ";
        message += "To do so simply write me the Wi-Fi name, password and the location description in your next message. ";
        message += "Also, please, separate the pieces of information with a comma. For example, your message could look like this:";
        message += "\n\nCoffeeCafe Wi-Fi Free, LoveCoffee123, At Coffee Cafe in the city centre of Amsterdam, Netherlands";
        message += "\n\nIf you don't want to add any locations just yet, simply write me any other command.";
        bot.sendMessage(chat_id, message, "");
        return (cycles);
    }
    else if (text == "/ota")
    {
        cycles = 0;
        add_location_flag = false;
        bot.sendMessage(chat_id, "The OTA mode is for wireless firmware update and accessable only by the developers. If you wish to continue, enter your Developer Password", "");
        return (cycles);
    }
    else if (text == ("/" + String(OTA_PASSWORD)) || text == ("/ota " + String(OTA_PASSWORD)))
    {
        add_location_flag = false;
        bot.sendMessage(chat_id, "Password accepted", "");
        cycles = ft_ota_mode(chat_id);
        return (cycles);
    }
    else if (text == "/reboot")
    {
        bot.sendMessage(chat_id, "Rebooting!", "");
        g_for_this_long = 10000;
        esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
        esp_sleep_enable_timer_wakeup(g_for_this_long);
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
            ft_write_spiffs_file("/locations.txt", text);
            
        }
        else
        {
            bot.sendMessage(chat_id, "I'm sorry, I don't understand", "");
            bot.sendMessage(chat_id, "Try one of the following commands: status, location, list locations, add location, ota, reboot, off. Every command should start with \"/\" sign", "");
        }
        return (cycles);
    }
    return (cycles);
}

short ft_new_messages(short numNewMessages)                                       // function to handle what happens when you receive new messages
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

    while (cycles <= WAIT_FOR_MESSAGES_LIMIT)                                       // waiting for new messages
    {
        esp_task_wdt_reset();
        DEBUG_PRINTF("Waiting for incomming commands from Telegram chat. Waiting loop cycles: %d\n", cycles);       
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);             // check how many new messages in queue
        while (numNewMessages)
        {
            cycles = ft_new_messages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }
        if ((cycles + 25) == WAIT_FOR_MESSAGES_LIMIT)
            bot.sendMessage(CHAT_ID, "It seems that I'm not currently needed. I'll wait for 1 more minute just in case and then go to sleep. To keep me awake, write me anything.", "");
        cycles++;
    }
}
 
