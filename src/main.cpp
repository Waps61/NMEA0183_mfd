/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  URL:      https://www.hackster.io/waps61
  TARGET:   ESP32-P4-evboard integrated with in a JC1060P470 display module
  VERSION:  0.2
  Date:     v0.1 31-01-2026
  Last
  Update:   v0.2 14-02-2026
            Except from communication with hardwared NMEA0183 the program is functional.
            For testing and demo purposes it sends virtual NMEA0183 messages and these
            are processed correctly.
            Persistent storage implemented and working
            Sun-, Dawn and Night modes not functional
  Previous
  updates:
            31-01-2026 V0.1
            1st working version of the UI without NMEA0183 data input, but with a working demo

  Achieved: 31-01-2026 Succesful FAT  with Runnable version on ESP32 with JC1060P470 HMI
  Purpose:  Build a Multi Function Display for wired NMEA0183 data
            - Reading NMEA0183 v2.x data from a talker device such as a multiplexer
            - Parse the data and send data to display
            - Relay data for next device in the NMEA0183 chain
            - Inform the crew a board about sensor readings

  NOTES:
        1)  NMEA encoding conventions in short
            An NMEA sentence consists of a start delimiter, followed by a comma-separated sequence
            of fields, followed by the character '*' (ASCII 42), the checksum and an end-of-line marker.
            i.e. <start delimiter><field 0>,<field 1>,,,<field n>*<checksum><end-of-linemarker>
            The start delimiter is either $ or !. <field 0> contains the tag and the remaining fields
            the values. The tag is normaly a 5 character wide identifier where the 1st 2 characters
            identify the talker ID and the last 3 identify the sentence ID.
            Maximum sentence length, including the $ and <CR><LF> is 82 bytes.

            Source: https://gpsd.gitlab.io/gpsd/NMEA.html#_nmea_0183_physical_protocol_layer


        2)  Digital GPIO xx (and xx ) are reserved for NMEA talker via
            Hard/ SoftSerial on 4800 Bd

            The layout consists of a static mainscreen stacked with a menubar
   and 5 panels. Each panel 1-6 tiles. Buttons on the menubar toggle the panels
   on or off
    __________________________________________
    \       mainscreen                        \
     \ ____ ___________________________________\
      \\ m \\   panels                         \\
       \\ e \\ __________                       \\
        \\ n \\\ tiles   \                       \\
         \\ u \\\         \                       \\
          \\   \\\         \                       \\
           \\___\\\_________\                       \\
            \     \__________________________________\\
             \_________________________________________\

  Hardware setup:
  RS-484 (NMEA0183 protocol) are read from GPIO 32 & 33

  Wiring Diagram (for NMEA0183 to NMEA0183 device):
     ESP32      | NMEA device
     TX GPIO 33 |  RX +
     RX GPIO 32 |  TX +

  NOTE: The above settings works with the USB power to the ESP32

  ! Remember to set the pins to suit your display module! and that the EPS32 GPIO
  ! pins run on 3.3V (and not 5V as the Arduino)


  ---------------
  Terms of use:
  ---------------
  The software is provided "AS IS", without any warranty of any kind, express or implied,
  including but not limited to the warranties of mechantability, fitness for a particular
  purpose and noninfringement. In no event shall the authors or copyright holders be liable
  for any claim, damages or other liability, whether in an action of contract, tort or
  otherwise, arising from, out of or in connection with the software or the use or other
  dealings in the software.

  -----------
  Warning:
  -----------
  Do NOT use this compass in situations involving safety to life
  such as navigation at sea.

  TO DO:    - Connect HMI to 5V from the Buck converter i.s.o. 3.3V pin on ESP32
            -Implement 2-way communication so that incomming NMEA data can be relayed
            to other devices. An NMEA0183 network is typically a daisy chained network

  LIMITATIONS:


  Credits:
    - https://www.arduinoforum.de/User-andruid59 for his work on JC1060P470 display module at
      https://www.arduinoforum.de/arduino-Thread-Test-JC1060P470-CYD-mit-ESP32-P4
    - LVGL community for their great UI library
    - GFX library by Moononournation for (hw)display handling
    - Bitbank2 for the capacitive touch library
    - https://github.com/pixelEDI for pointing me to EEZ Studio; https://www.envox.eu/
  */

#include <Arduino.h>
#include <lvgl.h>
#include <mfd_conf.h>

#include <font/mfd_fonts.h>
#include <NMEA0183_conf.h>

#ifndef TEST
#include <ui/ui_screens.h>
#include <ui/screen_main.h>
#include <ui/screen_about.h>
#include <ui/mfd_themes.h>
#include <ui/mfd_bright_panel.h>
#include <persist/mfd_persistent.h>
#include <NMEA0183_data.h>

#include <persist/flash_erase.h>

#endif // TEST

#ifdef TEST
#include "test/testlab.h"

#endif // TEST
// lv_style_t style_base;
lv_obj_t *main_view = NULL;
lv_theme_t *mfd_theme_day;

/**
 * Set the backlight of the JC1060P470 display with integrated ESP32-P4-C6
 */
void set_backlight(int value)
{
  if (value > 4 && value < 256)
  {
    analogWrite(GFX_BL, value);
  }
}

void setup()
{
  ship_config = mfd_read_persistent_data();

  lv_init();
  Serial.begin(115200);

  delay(1000);
  lv_log("\n\n+++++ NMEA0183 simple MFD \n");
  lv_log("+++++ using lvgl V%d.%d.%d \n", lv_version_major(), lv_version_minor(), lv_version_patch());

  touchscreen_setup();
#ifndef TEST
  mfd_theme_day = lv_theme_default_init(
      disp,                      /* Use DPI, size, etc. from this display */
      lv_color_hex(DAY_PRIMARY), /* Primary and secondary palette */
      lv_color_hex(DAY_SECONDARY),
      false, /* Dark theme?  False = light theme. */
      &ui_font_lv_conthrax_16);

  // Read the mfdconfig settting from EEPROM
  // by opening the app in w-r mode
  // TO DO: fix compiling error: unkown type 'class'
  // mfdsettings.begin("nmea-mfd", false);
  // mfd_baudrate = mfdsettings.getInt("baudrate", 38400);
  // mfd_wifi = mfdsettings.getBool("wifi", false);
  // mfd_ssid = mfdsettings.getString("SSID", "your_ssid");
  // mfd_pwd = mfdsettings.getString("PWD", "your_pwd");
  // mfd_log = mfdsettings.getLong("log", 0);

  lv_log("+++++ Setup done\n");
  ui_screens_init("yazz_mfd");

  lv_disp_load_scr(screen_main);
  ("--->>EEPROM read with:\n");
  lv_log("baudrate: %d\n", ship_config.baudrate);
  lv_log("wifi is %d\n", ship_config.wifi_on);
  lv_log("SSID :%s\n", ship_config.SSID);
  lv_log("pwd :%s\n", ship_config.pwd);
  lv_log("log: %.1f\n", ship_config.ship_log);
  boat_log = ship_config.ship_log;
  lv_subject_init_int(&mfd_baudrate, ship_config.baudrate);
  lv_subject_init_int(&mfd_wifi, ship_config.wifi_on);
  char tmpbuf[26];
  ship_config.SSID.toCharArray(tmpbuf, 25, 0);
  lv_subject_init_string(&mfd_ssid, mfd_ssid_curval, mfd_ssid_oldval, 24, tmpbuf);
  ship_config.pwd.toCharArray(tmpbuf, 25, 0);
  lv_subject_init_string(&mfd_ssid, mfd_pwd_curval, mfd_pwd_oldval, 24, tmpbuf);
  lv_subject_init_float(&mfd_log, ship_config.ship_log);

  // char *rslt;
  // for testing purposes only
  // mfd_pers_t testconfig;
  // testconfig.baudrate = 4800;
  // testconfig.wifi_on = 0;
  // testconfig.SSID = "test2_ssid";
  // testconfig.pwd = "###2";
  // testconfig.ship_log = 2345.6;
  // mfd_write_persistent_data(&testconfig);
  // delay(250);
  // testconfig.SSID = "update3_ssid";
  // mfd_update_persistent_key(MFD_SSID,&testconfig);
  // testconfig.baudrate = 38400;
  // mfd_update_persistent_key(MFD_BAUDRATE, &testconfig);
  // erase_flash();
  // lv_log("result %s",rslt);
#endif // TEST
#ifdef TEST
  testlab_init();
#endif // TEST
}

void loop()
{
  lv_task_handler(); /* let the GUI do its work */
  delay(5);
#ifndef TEST
#ifdef DEMO
  test_screen_data_updates();
#endif

  mfd_update_tile_data();

// if (mfd_style_changed)
// {
//   mfd_style_changed = false;
//   lv_obj_report_style_change(&mfd_style_night);
// }
#endif // TEST
}
