#include <Arduino.h>
#include <nvs_flash.h>
#include <persist/flash_erase.h>

void erase_flash()
{
  nvs_flash_erase(); // erase the NVS partition and...
  nvs_flash_init();  // initialize the NVS partition.
  while (true)
    ;
}
