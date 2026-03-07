/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  URL:      https://www.hackster.io/waps61
  TARGET:   ESP32-P4-evboard integrated with in a JC1060P470 display module
  VERSION:  0.8.4 07-03-2026
  Date:     v0.1 31-01-2026
  NOTE:     Version is set in major and minor tick in mfd_conf.h
  Last :    v0.8.4 07-03-2026:
  Changes:  Moved wind gauges from right to middle position on windpanel for better optical balance
            V0.8.3 06-03-2026
            Memory assertion seems to besolved by changing line 72 in l_conf.h, size of memory available
            for `lv_malloc()` in bytes (>= 2kB) #define LV_MEM_SIZE (128 * 1024U) (used to 64 *124u by default).
            Two wind gaugaes added to the wind panel; one for apparent wind angele between -135 and +135 degrees and
            and an apparent wind angle +, showing a zoomed view  between -60 and +60 degrees for higher precision
 Previous   V08.2 05-03-2026
 updates:   A gauge tile added (this was a feature on the prio list.The gauge is a graphical representation of
            the data, and can be used to display the wind angle for example. It is implemented as a separate tile type,
            since it has a different layout and functionality than the regular tiles. It is not problem free yet since
            it drains the memory.
            V0.81 04-03-2026
            MTW tag added to NMEA_TAG enum and implemented in mfd_calculation.cpp since it was forgotton to be added
            in the previous version, and is used in the test data and is a common tag for water temperature.
            Fixed unreliable serial cable leading to unreadable data from the UART leading to long debugging sessions
            Implemented filtering of non-printable characters in the serial data, which can cause problems in the processing
            of the NMEA data and can lead to crashes or incorrect data being displayed.
            V0.80 01-03-2026
            FAT passed with real NMEA0183 data from the test network, and the display is working as expected.
            2way communication crashed the program, so I have commented out the talker part for now, and will implement it in a later stage.
             I have added some comments to the code to make it more clear what is going on.
            v0.6 28-02-2026
            Implemented 2-way communication so that incomming NMEA data can be relayed
            I re-used my state based function from the Yazz_NMEAtor_ESP32 project to read the
            NMEA data from the serial port, and added a function to process the data when it is ready.
            fixed bugs, cleaned code, implemented version info on settings screen,
            fixed toggle state for menubar, added spacer for panels when not all tiles are used
            implemented Sun-, Dawn and Night modes
            V0.5  22-02-2026
            fixed bugs, cleaned code, implemented version info on settings screen,
            fixed toggle state for menubar, added spacer for panels when not all tiles are used
            implemented Sun-, Dawn and Night modes
            v0.4 17-02-2026
            Fixed toggle state. Cleaned dead code
            Version info implemented on Setting screen
            TO DO: Implement Sun-, Dawn and Night modes
            v0.3 15-02-2026
            fixed bugs, cleaned code, spacer added for panels when not all tiles are used
            CMG implemented
            TO DO: Implement Sun-, Dawn and Night modes
                   fix toggle state for menubar
            v0.2 14-02-2026
            Except from communication with hardwared NMEA0183 the program is functional.
            For testing and demo purposes it sends virtual NMEA0183 messages and these
            are processed correctly.
            Persistent storage implemented and working
            Sun-, Dawn and Night modes not functional
            v0.1 31-01-2026
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

/**
 * If you want to test sepcific undocumented LVGL functionality
 * then use this playground.
 * It excludes all of your main program, unless you include them within the
 * testboundary here
 */
// #define TEST // Exclude if not used for testing purposes
#ifdef TEST
#include "test/testlab.h"

#endif // TEST
#ifndef TEST
#include <ui/ui_screens.h>
#include <ui/screen_main.h>
#include <ui/screen_about.h>
#include <ui/mfd_themes.h>
#include <ui/mfd_bright_panel.h>
#include <persist/mfd_persistent.h>
#include <NMEA0183_data.h>
#include <comms/mfd_communication.h>

#include <persist/flash_erase.h>

#endif // TEST

// lv_style_t style_base;
lv_obj_t *main_view = NULL;
lv_theme_t *mfd_theme_day;
// static Preferences mfdsettings;
lv_subject_t mfd_subject_baudrate; // default to 38400
lv_subject_t mfd_subject_wifi;     // default to 0 = off
lv_subject_t mfd_subject_ssid;     // obviously not disclosed, but read from the EEPROM
lv_subject_t mfd_subject_pwd;      // obviously not disclosed, but read from the EEPROM
lv_subject_t mfd_subject_log;      // total mileage of your ship

#ifndef TEST
mfd_pers_t ship_config;
float boat_log = 0.01;
#endif // TEST
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
#ifndef TEST
  // Read persistent dat from NVR (non volatile ram)
  ship_config = mfd_read_persistent_data();
  nvr_millis = millis();
#endif // TEST
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
      lv_palette_lighten(LV_PALETTE_GREY, 1),
      false, /* Dark theme?  False = light theme. */
      &ui_font_lv_conthrax_16);

  lv_log("+++++ Setup done\n");
  ui_screens_init("yazz_mfd");

  // set subject data for config data from NVR
  char tmpbuf[26];
  lv_subject_init_int(&mfd_subject_baudrate, mfd_ship_config_get_baudrate());
  lv_log("mfd_subject_baudrate = %d\n", lv_subject_get_int(&mfd_subject_baudrate));
  lv_subject_init_int(&mfd_subject_wifi, mfd_ship_config_get_wifi());
  lv_log("mfd_subject_wifi_on = %d\n", lv_subject_get_int(&mfd_subject_wifi));
  (mfd_ship_config_get_ssid()).toCharArray(tmpbuf, 25, 0);
  lv_subject_init_string(&mfd_subject_ssid, mfd_ssid_curval, mfd_ssid_oldval, 25, tmpbuf);
  lv_log("mfd_subject_ssid = %s\n", lv_subject_get_string(&mfd_subject_ssid));
  (mfd_ship_config_get_pwd()).toCharArray(tmpbuf, 25, 0);
  lv_subject_init_string(&mfd_subject_pwd, mfd_pwd_curval, mfd_pwd_oldval, 25, tmpbuf);
  lv_log("mfd_subject_pwd = %s\n", lv_subject_get_string(&mfd_subject_pwd));
  lv_subject_init_float(&mfd_subject_log, mfd_ship_config_get_log());
  lv_log("mfd_subject_log = %.1f\n", lv_subject_get_float(&mfd_subject_log));

  lv_disp_load_scr(screen_main);
  lv_log("--->>EEPROM read with:\n");
  lv_log("baudrate: %d\n", mfd_ship_config_get_baudrate());
  lv_log("wifi is %d\n", mfd_ship_config_get_wifi());
  lv_log("SSID :%s\n", mfd_ship_config_get_ssid());
  lv_log("pwd :%s\n", mfd_ship_config_get_pwd());
  lv_log("log: %.1f\n", mfd_ship_config_get_log());

  set_boat_log(lv_subject_get_float(&mfd_subject_log)); // set boat_log to value from NVR
  lv_log(" boat_log initialize with value from mfd_log %.1f\n", get_boat_log());

  // for testing purposes only
  //  mfd_pers_t testconfig;
  //  testconfig.baudrate = 4800;
  //  testconfig.wifi_on = 0;
  //  testconfig.SSID = "test2_ssid";
  //  testconfig.pwd = "###2";
  //  testconfig.ship_log = 2345.6;
  //  mfd_write_persistent_data(&testconfig);
  //  delay(250);
  //  testconfig.SSID = "update3_ssid";
  //  mfd_update_persistent_key(MFD_SSID,&testconfig);
  //  testconfig.baudrate = 38400;
  //  mfd_update_persistent_key(MFD_BAUDRATE, &testconfig);

  // erase_flash();
#endif // TEST
#ifdef TEST
  testlab_init();
#endif // TEST
#ifndef DEMO
  mfd_setup_communication();
#endif // DEMO
  mem_monitor = (lv_mem_monitor_t *)malloc(sizeof(lv_mem_monitor_t));
  lv_mem_monitor(mem_monitor);
  lv_log("Memory monitor after init: total size: %d, free size: %d, used size: %d\n", mem_monitor->total_size, mem_monitor->free_size, mem_monitor->total_size - mem_monitor->free_size);
  lv_free(mem_monitor);
}

void loop()
{
  lv_task_handler(); /* let the GUI do its work */
  delay(5);
#ifndef TEST
#ifdef DEMO
  test_screen_data_updates();
#endif
#ifndef DEMO
  NMEA_startListening();

#endif // DEMO

  mfd_update_tile_data();

  if ((millis() - nvr_millis) > NVR_UPDATE_INTERVAL) // every 5 minutes the ships_log is store to NVR
  {
    nvr_millis = millis();
    // synchronize the subject and the persistent ship log objects
    lv_subject_set_float(&mfd_subject_log, get_boat_log());
    mfd_ship_config_set_log(get_boat_log());
    // write last vale of ship log to NVR
    mfd_update_persistent_key(MFD_SHIPLOG, &ship_config);
  }
#endif // TEST

  mfd_recolor(screen_main);
}
