# NMEA0183-MFD
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

## Example
### the mainscreen
![screenshot of the mainscreen](/screenshots/IMG_4331.jpg)

### the brightness screen
![screenshot of the brightness screen](/screenshots/IMG_4332.jpg)

### the settingsscreen
![screenshot of the settings screen](/screenshots/IMG_4333.jpg)


### video
>[Demo Video on tube](https://youtu.be/hAYLPJpzyHU)
