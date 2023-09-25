/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   spiffs_management.h                                              :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/09/17 08:49:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/09/17 12:48:41                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   The file contains functions to work with SPIFFS file system as well as files inside of it.   */
/*                                                                                                */
/* ********************************************************************************************** */

void  ft_write_spiffs_file(const char* file_name, String input)
{
    short i;

    i = 1;
    File file = SPIFFS.open(file_name, "a+");
    while (!file && i <= 5)
    {
        DEBUG_PRINTS("An error occurred while opening %s file for writing in SPIFFS. Retry #%d.\n", file_name, i, "", "");
        File file = SPIFFS.open(file_name, "a+");
        i++;
        delay(100);
    }
    if (!file)
        DEBUG_PRINTS("Failed to open %s file for writing in SPIFFS even after %d retries. The file dependant function will be unavailable during this programm cycle.\n", file_name, i, "", "");
    else
    {
        file.println(input);
        file.close();
    }
}

String  ft_read_spiffs_file(const char* file_name)
{
    short   i;
    String  output;

    i = 1;
    File file = SPIFFS.open(file_name, "r");
    while (!file && i <= 5)
    {
        DEBUG_PRINTS("An error occurred while opening %s file for reading in SPIFFS. Retry #%d.\n", file_name, i, "", "");
        File file = SPIFFS.open(file_name, "r");
        i++;
        delay(100);
    }
    if (!file)
    {
        DEBUG_PRINTS("Failed to open %s file for reading in SPIFFS even after %d retries. The file dependant function will be unavailable during this programm cycle.\n", file_name, i, "", "");
        output = "Unfortunatelly, the " + String(file_name) + " file was not found.";
        file.close();
    }  
    else
    {
        while (file.available())
            output += file.readStringUntil('\n');
        file.close(); 
    }
    return (output);
}

void  ft_delete_spiffs_file(const char* file_name)
{
    short i;

    i = 1;
    if (SPIFFS.remove(file_name))
        DEBUG_PRINTF("File %s was successfully deleted.\n", file_name);
    else
    {
        while (SPIFFS.exists(file_name) && i <= 5)
        {
            DEBUG_PRINTS("Failed to delete file %s. Retry #%d.\n", file_name, i, "", "");
            SPIFFS.remove(file_name);
            i++;
        }
        DEBUG_PRINTS("Failed to delete %s file in SPIFFS even after %d retries. The file dependant functions may not work as intended.\n", file_name, i, "", "");
    }
}

void  IRAM_ATTR ft_spiffs_init(void)
{
    short i;

    i = 1;
    while (!SPIFFS.begin(true) && i <= 5)
    {
        DEBUG_PRINTF("An error occurred while mounting SPIFFS\n", "");
        i++;
        delay(100);
    }
    if (!SPIFFS.begin(true))
        DEBUG_PRINTF("\nFailed to mount SPIFFS even after %d retries. Dependant functions will be unavailable during this programm cycle.\n", i);
}
 
