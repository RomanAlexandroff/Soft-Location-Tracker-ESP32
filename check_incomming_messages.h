/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   check_incomming_messages.h                                       :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/06/28 14:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/06/29 18:48:41                                ###    ###   ###     ###         */
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
    short   cycles;
    String  message;

    ESP.wdtFeed();
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
        ESP.wdtFeed();
        cycles = 0;
        message = "I am connected to " + String(WiFi.SSID());   
        message += ". Signal strength is " + String(WiFi.RSSI());
        message += " dBm. My battery is " + String(ft_battery_check()) + "% charged";
        bot.sendMessage(chat_id, message, "Markdown");
        message.clear();
        return (cycles);
    }
    if (text == "/recorder report")
    {
        ESP.wdtFeed();
        cycles = 0;
        if (rtcMng.scan_results[0][0] != '\0')
            ft_scan_report();
        else
            bot.sendMessage(CHAT_ID, "My list of unknown Wi-Fi networks is currently empty", "");
        return (cycles);
    }
    else if (text == "/location")
    {
        cycles = 0;
        rtcMng.last_wifi = 0;
        ft_send_location();
        return (cycles);
    }
    else if (text == "/ota")
    {
        cycles = 0;
        bot.sendMessage(chat_id, "The OTA mode is for wireless firmware update and accessable only by the developers. If you wish to continue, enter your Developer Password", "");
        return (cycles);
    }
    else if (text == ("/" + String(OTA_PASSWORD)) || text == ("/ota " + String(OTA_PASSWORD)))
    {
        cycles = -32767;                                                              // keep the device working as long as possible while OTA
        bot.sendMessage(chat_id, "Password accepted", "");
        ft_ota_mode(chat_id);
        return (cycles);
    }
    else if (text == "/reboot")
    {
        bot.sendMessage(chat_id, "Rebooting!", "");
        g_for_this_long = 10;
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
        bot.sendMessage(chat_id, "I'm sorry, I don't understand", "");
        bot.sendMessage(chat_id, "Try one of the following commands: status, location, recorder report, ota, reboot, off. Every command should start with \"/\" sign", "");
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

    ESP.wdtFeed();
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
    ESP.wdtFeed();
    return (cycles);
}

void  ft_check_incomming_messages(short cycles)
{
    short numNewMessages;

    while (cycles <= WAIT_FOR_MESSAGES_LIMIT)                                       // waiting for new messages
    {
        ESP.wdtFeed();
        DEBUG_PRINTF("Waiting for incomming commands from Telegram chat\n", "");       
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);             // check how many new messages in queue
        while (numNewMessages)
        {
            DEBUG_PRINTF("Inside of the while (numNewMessages) loop, numNewMessages == %d\n", numNewMessages);
            cycles = ft_new_messages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }
        DEBUG_PRINTF("Waiting loop cycles: %d\n", cycles);
        if ((cycles + 25) == WAIT_FOR_MESSAGES_LIMIT)
            bot.sendMessage(CHAT_ID, "It seems that I'm not currently needed. I'll wait for 1 more minute just in case and then go to sleep. To keep me awake, write me anything.", "");
        cycles++;
    }
}
 
