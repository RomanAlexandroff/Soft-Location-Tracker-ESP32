/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   spiffs_management.cpp                                            :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/17 08:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/11/12 11:40:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   The file contains functions to work with LittleFS file system as well as files inside of it. */
/*                                                                                                */
/* ********************************************************************************************** */

#include "header.h"

short  ft_write_spiffs_file(const char* file_name, String input)
{
    short i;

    i = 1;
    File file = LittleFS.open(file_name, "a+");
    while (!file && i <= 5)
    {
        DEBUG_PRINTS("An error occurred while opening %s file for writing in LittleFS. Retry #%d.\n", file_name, i, "", "");
        File file = LittleFS.open(file_name, "a+");
        i++;
        delay(100);
    }
    if (!file)
    {
        DEBUG_PRINTS("Failed to open %s file for writing in LittleFS even after %d retries. The file dependant function will be unavailable during this programm cycle.\n", file_name, (i - 1), "", "");
        return (0);
    }
    else
    {
        file.println(input);
        file.close();
    }
    return (1);
}

String  ft_read_spiffs_file(const char* file_name)
{
    short   i;
    String  output;

    i = 1;
    File file = LittleFS.open(file_name, "r");
    while (!file && i <= 5)
    {
        DEBUG_PRINTS("An error occurred while opening %s file for reading in LittleFS. Retry #%d.\n", file_name, i, "", "");
        File file = LittleFS.open(file_name, "r");
        i++;
        delay(100);
    }
    if (!file)
    {
        DEBUG_PRINTS("Failed to open %s file for reading in LittleFS even after %d retries. The file dependant function will be unavailable during this programm cycle.\n", file_name, (i - 1), "", "");
        file.close();
    }  
    else
    {
        while (file.available())
            output += file.readStringUntil('\n') + "\n";
        file.close(); 
    }
    return (output);
}

void  ft_delete_spiffs_file(const char* file_name)
{
    short i;

    i = 1;
    if (LittleFS.remove(file_name))
        DEBUG_PRINTF("File %s was successfully deleted.\n", file_name);
    else
    {
        while (LittleFS.exists(file_name) && i <= 5)
        {
            DEBUG_PRINTS("Failed to delete file %s. Retry #%d.\n", file_name, i, "", "");
            LittleFS.remove(file_name);
            i++;
        }
        DEBUG_PRINTS("Failed to delete %s file in LittleFS even after %d retries. The file dependant functions may not work as intended.\n", file_name, (i - 1), "", "");
    }
}

void  IRAM_ATTR ft_spiffs_init(void)
{
    short i;

    i = 1;
    while (!LittleFS.begin(true) && i <= 5)
    {
        DEBUG_PRINTF("An error occurred while mounting LittleFS\n", "");
        i++;
        delay(100);
    }
    if (!LittleFS.begin(true))
        DEBUG_PRINTF("\nFailed to mount LittleFS even after %d retries. Dependant functions will be unavailable during this programm cycle.\n", (i - 1));
}
 
