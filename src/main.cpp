/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  URL:      https://www.hackster.io/waps61
  TARGET:   ESP32-P4-evboard integrated with in a JC1060P470 display module
  VERSION:  0.9.1 14-03-2026
  Date:     v0.1 31-01-2026
  NOTE:     Version is set in major and minor tick in mfd_conf.h
  Change log: See CHANGELOG.MD file in the root of the project


  Achieved: 14-03-2026 Successful SAT with 2 minor improvement suggestions, see CHANGELOG.MD for details
            31-01-2026 Succesful FAT  with Runnable version on ESP32 with JC1060P470 HMI
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
     TX GPIO 33 |
     RX GPIO 32 |  NMEA OUT -
     GND        |  GND

  NOTE: The above settings works with the USB power to the ESP32

  ! Remember to set the pins to suit your display module! and that the EPS32 GPIO
  ! pins run on 3.3V (and not 5V as the Arduino)


  ---------------
  Terms of use:
  ---------------
  IT License

Copyright (c) 2026 Waps61

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

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
#include <mfd_conf.h>
#include <persist/flash_erase.h>

#endif // TEST
/**
 * Initialize global variables for the boat data, NMEA data store, LVGL subjects and styles, and UI elements.
 * These variables will be used throughout the program to store and manage the state of the boat's data,
 * the NMEA data, the UI state, and the configuration settings.
 */
float boat_sog = 0.0;
int boat_hdg = 0;
int boat_awa = 0;
int boat_cts = 0;
int boat_cog = 0;
float boat_dpt = 0.0;
float boat_vmg = 0.0;
float boat_log = 0.1; // = 0.1;  // Will be set to real value when EEPROM is read

float boat_trp = 0.0;      // = 0.0;
bool trip_started = false; // to ensure trip starts at 0
char lat_new[FIELD_BUFFER] = {0};
char lon_new[FIELD_BUFFER] = {0};
char lat_old[FIELD_BUFFER] = {0};
char lon_old[FIELD_BUFFER] = {0};

char NMEA_DATA_STORE[NR_OF_NMEA_TAGS][NMEA_BUFFER_SIZE + 1] = {0};
bool data_store_inited = false;

lv_subject_t brightness_subject;
bool brightness_changed = false;
int brightness_value = 0;
bool update_data_values = false;
lv_obj_t *screen_active = NULL;
lv_mem_monitor_t *mem_monitor = NULL;

/*Create a Tabview box object*/
lv_obj_t *SOGbox, *CTSbox, *COGbox, *DPTbox, *AWAbox, *TWAbox, *AWSbox, *TWSbox,
    *TRPbox, *VMGbox, *CMGbox, *LOGbox, *HDGbox = NULL;
// some boxes ure used twice
lv_obj_t *SOGbox2, *COGbox2, *CTSbox2, *AWSbox2 = NULL;
lv_obj_t *AWAGaugeZoom, *AWAGaugeTileZoom = NULL;
lv_obj_t *AWAGauge, *AWAGaugeTile = NULL;

// static Preferences mfdsettings;
lv_subject_t mfd_subject_baudrate;
lv_subject_t mfd_subject_wifi;
lv_subject_t mfd_subject_ssid;
lv_subject_t mfd_subject_pwd;
lv_subject_t mfd_subject_log;
// extern lv_subject_t *mfd_groupsettings_array_subject[];
bool mfd_demo_mode = false; // set to true to enable demo mode, which simulates data for the display

char mfd_ssid_curval[26] = {0};
char mfd_ssid_oldval[26] = {0};
char mfd_pwd_curval[26] = {0};
char mfd_pwd_oldval[26] = {0};

bool mfd_styles_inited = false;
int mfd_style_changed = -1;

lv_subject_t theme_subject;
lv_style_t mfd_style_day, mfd_style_night, mfd_style_sun, mfd_style_dawn, mfd_style;
lv_style_t mfd_style_tile, mfd_style_menubar, mfd_style_btn, mfd_style_btn_pressed;

// lv_style_t style_base;
lv_obj_t *main_view = NULL;
lv_theme_t *mfd_theme_day;

int nmeaStatus = INVALID;                    //*** state variable for the NMEA receiving process, initial state is INVALID
int nmeaIndex = 0;                           //*** index variable for the NMEA receiving process, initial state is 0
bool nmeaDataReady = false;                  //*** flag variable for the NMEA receiving process, initial state is false
bool recvInProgress = false;                 //*** flag variable to indicate if the receiving process is in progress, initial state is false
bool newData = false;                        // *** flag variable to indicate if new data is available, initial state is false
char nmeaBuffer[NMEA_BUFFER_SIZE + 1] = {0}; //*** buffer variable to store the incoming NMEA data, initial state is an array of 0 with size NMEA_BUFFER_SIZE + 1

#ifndef TEST
mfd_pers_t ship_config;
#endif // TEST

void set_demo_mode(bool value)
{
  mfd_demo_mode = value;
}

bool get_demo_mode()
{
  return mfd_demo_mode;
}

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
  if (!get_demo_mode())
  {
    NMEA_startListening();
  }
  else
    test_screen_data_updates();

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

  mfd_recolor(screen_main);
}
