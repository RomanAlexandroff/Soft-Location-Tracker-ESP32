/* ********************************************************************************************** */
/*                                                                                                */
/*   Soft  Tracker  Project  ESP32-C3                                  :::::::::        :::       */
/*   bluetooth_handling.h                                             :+:    :+:     :+: :+:      */
/*                                                                   +:+    +:+    +:+   +:+      */
/*   By: Roman Alexandrov <r.aleksandroff@gmail.com>                +#++:++#:    +#++:++#++:      */
/*                                                                 +#+    +#+   +#+     +#+       */
/*   Created: 2023/10/10 13:42:16                                 #+#    #+#   #+#     #+#        */
/*   Updated: 2023/10/10 13:42:16                                ###    ###   ###     ###         */
/*                                                                                                */
/*                                                                                                */
/*   Everything around the Bluetooth Low Energy (BLE) functionality. All the functions and        */
/*   the supporting libraries target the BLE protocol and may not work with the classic           */
/*   Bluetooth protocol!                                                                          */
/*                                                                                                */
/* ********************************************************************************************** */

inline void ft_bluetooth_serial_loop(void)
{
    char dat;

    if (ble.available())
    {
        dat = ble.read();
        DEBUG_PRINTF("%s", dat);
    }
    if (Serial.available())
    {
        dat = Serial.read();
        ble.write(dat);
    }
}

inline void ft_bluetooth_serial_init(void)
{
    ble.begin();
    ble.setName("SoftTraker");
    ble.getName();
    ble.setBaud(BAUD_115200);
}
