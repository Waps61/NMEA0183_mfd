# NMEA0183-MFD: A marine multifunction display
#### V1.3 released 29-06-2026
Solved the bug with the background color of buttons. The flag LV_OBJ_FLAG_CHECKABLE was causing the issue.
#### V1.2 relased 27-06-2026
One feature request was still on the hot list. Previous version ommitted MOB functionality.
This has been implemented in V1.2 as an MOB panel. When activated, the current boat position is stored as the MOB position and is shown on a radar plot. With North up your boat is in the middle and the MOB position and distance is plotted on the radar plot. Your boat point is the actual COG so it gives you an idea where to go.
### the MOB panel
![screenshot of the MOB panel](/screenshots/mob_screen.jpeg)
#### V1.1 released 19-06-2026
After using the device for a couple of months on board,it did not fail me once. Some improvements or enhancements were noted. Like\
* The latitude and longitude was not implemented, but it became clear that this is a must. So this is implemented now as minitiles on the TRP panel
* For my use a depth sensor offset was not taken into account. But for a more generic use this has now been implemented in the Settings panel
* When booting the device brightness was set to 25%. For outdoors use this was not sufficient, so this has been increasded to 75%
* TWA and TWs were not calculated, in the assumption that these values would come from my sensors, which they didn't. So formulas are now imnplemted to calculate the values.Calculations are based on document from Joseph George Caldwwell,"Calculating True Wind Speed and Direction from Apparent Wind Speed and Direction see: [here](https://www.starpath.com/freeware/truewind.pdf)
* Water temperature seemed to be missing. Now implemented as a minitile on TRIP panel
### the trip panel
![screenshot of the trip panel](/screenshots/trip_panel.jpeg)
### the wind panel
![screenshot of the wind panel](/screenshots/wind_panel.jpeg)
### the course panel
![screenshot of the course panel](/screenshots/course_panel.jpeg)
### the brightness panel
![screenshot of the brightness panel](/screenshots/brightness_panel.jpeg)
### the config panel
![screenshot of the config panel](/screenshots/config_panel.jpeg)
#### V1.0 released 17-03-2026
Package available. See [changelog.md](https://github.com/Waps61/NMEA0183_mfd/edit/main/changelog.md)

#### Update 15-03-2026
To see what has changed , read the [changelog.md](https://github.com/Waps61/NMEA0183_mfd/edit/main/changelog.md)
On March 14, 2026 a succesfull SAT has been executed. Some improvements have been noted, see changelog.md
#### Update 06-03-2026
The program suffered from memory assertions due to lack of free memory. Altought I assumed this was caused by memory leaks, it seems to be a settings issue with LV_MEM_SIZE in lv_conf.h (line 72) wich is default set to 64 * 1024 bytes. Increasing to 128 *1024 bytes seems to solve the issue
#### Update: 02-03-2026
Per version 0.8 the FAT has been successfull performed, now waiting the SAT on board if succesfull, version will be released as V1.0.
To assure the GPIO's of ESP32-P4 are working on the right Voltage level of 3.3V I had to shift down from 5V (measured with a multimeter with a scope funtion). Hence I've soldered a voltage divider with R2=10k and R1= 5k Ohm, so Vout = Vin*(R2/(R1+R2)) = 5V*(10/(5+10)) = 3,3V\
![screenshot of GPIO's](/screenshots/voltage_divider.jpg)
#### Update:28-02-2026
Per version 0.6 it is a functional version as an MVP. Meaning it communicates NMEA-0183 over serial data, settings can be stored to flash persistently,
Day-, Sun-, Dawn- and Night modes are functional. The following data is presented on screen:
Trip panel: CTS, COG, SOG(including history graph), DPT(including history graph), AWS, and TRP(trip distance in nautical miles)
Wind panel: AWA, TWA, AWS and TWS and two blanc panels
Course panel: CTS, HDG, COG, SOG, VMG and CMG
Data is interpreted from the following NMEA-0183 sentences:
* $--MVW
* $--RMC
* $--CTS
* $--TOB (not an official NMEA message but comming from my old network as a special for battery voltages)
* $--VWR
* $--XDR (general purpose field specified by NMEA used for battery voltage)
* $--HDG
* $--MTW
* $--DPT
* $--DBK
* $--DBT
* $--BWC
* $--VHW\
RX on GPIO 1, TX on GPIO2\
![screenshot of GPIO's](/screenshots/JC1060p470gpio.jpeg)

### Initiation
An attempt to get NMEA1083 data displayed on a MFD JC1060P470 with an ESP32-P4-C6 on board with LVGL
Because I could not get the provided examples to run. Most examples have faulty references to libraries.
Nor does it support the graphics driver in the GFX library for Arduino.
Second LVGL is a new library(to me), which has a steep learning curve (despite of 40 yrs experience) with partial documentation.\
In the lvgl.h files you also need to activate several options to make use of specific functionality 

If you (like me) assume that, with all your experience, this is a walk in the park, then this is bad news.
1) You need to RTFM to get started and to get your installation right for LVGL
2) Start small with a simple exercise to get an understanding of the working
3) make a plan

So the 1st thing to do is to create the lvgl-conf.h (make a copy of lv_conf_template.h  in the lvfgl folder),
and rename to lvgl-conf.h 1 level up in the tree, so it risides in the same directory as the lvgl folder.
2nd open the lvgl-conf.h file and at line 15 (as instructed in the file) below\
>`/* clang-format off */\
#if 0 /* Set this to "1" to enable content */`\
set this to 1.

There many more of these settings that are set to off by default!!
Next you have to make create the setting for the displaydriver jd9165 like below:

Internet research got me this. BIG Thanks for whoever made this
>`Arduino_ESP32DSIPanel *dsipanel = new Arduino_ESP32DSIPanel(
    40 /* hsync_pulse_width */, 160 /* hsync_back_porch */, 160 /* hsync_front_porch */,
    10 /* vsync_pulse_width */, 23 /*vsync_back_porch  */, 12 /* vsync_front_porch */,
    48000000 /* prefer_speed */);`\
>`Arduino_DSI_Display *gfx = new Arduino_DSI_Display(
    GFX_H_RES /* width */, GFX_V_RES /* height */, dsipanel, GFX_ROTATION /* rotation */, true /* auto_flush */,
    27 /* RST */, jd9165_init_operations, sizeof(jd9165_init_operations) / sizeof(lcd_init_cmd_t));`\
See the NMEA0183_conf.h file for more details to get this display up and running\
Next thing is to get the touchscreen up and running. Thanks goes to Andruid59 (AndrunoForum.de) who pointed me to the BBCaptouch extensions.\
>`// LVGL calls it when a rendered image needs to copied to the display
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
  //lv_log("+++++ In my_disp_flush() ");
#ifndef DIRECT_RENDER_MODE
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);
#endif                       // #ifndef DIRECT_RENDER_MODE
  lv_disp_flush_ready(disp); /*Call it to tell LVGL you are ready*/
}`

>`void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
  if (bbct.getSamples(&ti))
  {
    ti.x[0] = map(ti.x[0], 0, 1023, 0, w - 1); // X-Achse auf 0 - 1023 abgebildet
    ti.y[0] = map(ti.y[0], 0, 599, 0, h - 1);  // Y-Achse auf 0 - 599 abgebildet
    lv_log("TI Anzahl: %d X: %d Y: %d Area: %d Press: %d\n", ti.count, ti.x[0], ti.y[0], ti.area[0], ti.pressure[0]);
    data->point.x = ti.x[0];
    data->point.y = ti.y[0];
    data->state = LV_INDEV_STATE_PRESSED;
  }
  else
  {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}`\
Then with the ESP32-P4 is supported by Espressif, but only the `ESP32-P4 Funtion EV board(pre-rev.3.00)` is available.\
It has limited ROM and RAM availble (about 1Mb flash) when used without extra settings.\
\
>Therefor the additional settings as below are used in the `platformio.ini` file.\
`board_build.flash_size = 16MB`\
`board_build.partitions = custom_partitions.csv`\
The file `custom_partitons.csv` file wil maked use of the full memory potential.\
`# Name,   Type, SubType, Offset,  Size, Flags`\
`nvs,      data, nvs,     0x9000,  0x5000,`\
`otadata,  data, ota,     0xe000,  0x2000,`\
`app0,     app,  ota_0,   0x10000, 0x300000,`\
`app1,     app,  ota_1,   0x310000,0x300000,`\
`ffat,     data, fat,     0x610000,0x9E0000,`\
`coredump, data, coredump,0xFF0000,0x10000,`\
## Conceptual view of the panels
![screenshot of the mainscreen](/nmea0183_diagram.png)
## Use cases:
![screenshot of the mainscreen](/NMEA0183-mfd-Use-cases.png)
## UML diagram
![screenshot of the mainscreen](/NMEA0183-mfd-UML.png)
## Example
### the mainscreen in daylight mode
![screenshot of the mainscreen](/screenshots/trip_screen_daylight.jpg)

### the main screen in sunlight mode
![screenshot of the brightness screen](/screenshots/trip_screen_sunlight.jpg)

### the main screen in dawn mode
![screenshot of the settings screen](/screenshots/trip_screen_dawn.jpg)

### the main screen in night mode
![screenshot of the settings screen](/screenshots/trip_screen_night.jpg)

### the wind panel with gauges
![Screnshot of the windpanel with gauges](/screenshots/wind_panel.jpg)
### video
>[Demo Video on tube](https://youtu.be/ISXRW4jD3Wc?si=ROqHOkwfuZjdwLJk)
