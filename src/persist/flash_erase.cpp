/**
 * @file flash_erase.c
 */
/*                                                                                                    \
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili                              \
  Contact:  waps61 ,gmail.com                                                                         \
  Flash erase implementation. It is used to format the NVR when needed
    - due to corruption in the development phase
    - due to NVR free space limitations                                                                            \
*/                                                                                                    \
#include<Arduino.h>
#include <nvs_flash.h>
#include <persist/flash_erase.h>

void erase_flash()
{
  nvs_flash_erase(); // erase the NVS partition and...
  nvs_flash_init();  // initialize the NVS partition.
  while (true)
    ;
}
