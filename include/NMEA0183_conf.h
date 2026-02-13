#ifndef NMEA0183_CONF_H
#define NMEA0183_CONF_H

#include <Arduino_GFX_Library.h> // v1.6.4
#include <lvgl.h>                // v9.3.0

#include <Wire.h>

#include <bb_captouch.h> // v1§.3.2
#include "font/mfd_fonts.h"
#include "mfd_conf.h"

/**
 * Configuration for NMEA0183_simple_MFD
 * Define your display and touch parameters here
 */

/**
 * Display and Touch objects
 */
Arduino_ESP32DSIPanel *dsipanel = new Arduino_ESP32DSIPanel(
    40 /* hsync_pulse_width */, 160 /* hsync_back_porch */, 160 /* hsync_front_porch */,
    10 /* vsync_pulse_width */, 23 /*vsync_back_porch  */, 12 /* vsync_front_porch */,
    48000000 /* prefer_speed */);
Arduino_DSI_Display *gfx = new Arduino_DSI_Display(
    GFX_H_RES /* width */, GFX_V_RES /* height */, dsipanel, GFX_ROTATION /* rotation */, true /* auto_flush */,
    27 /* RST */, jd9165_init_operations, sizeof(jd9165_init_operations) / sizeof(lcd_init_cmd_t));

lv_display_t *disp;
lv_color_t *disp_draw_buf;
uint32_t screenWidth;
uint32_t screenHeight;
uint32_t bufSize;
BBCapTouch bbct;
TOUCHINFO ti;
int iType;
bool touched;
int32_t w, h, n, n1, cx, cy, cx1, cy1, cn, cn1;
uint8_t tsa, tsb, tsc, ds;
lv_obj_t *btn1, *btn2, *btn3;
lv_obj_t *label1, *label2, *label3;
lv_obj_t *swtch1, *cmps1;

lv_style_t green, yellow, red;

uint32_t millis_cb(void)
{
  return millis();
}

// LVGL calls it when a rendered image needs to copied to the display
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
  // lv_log("+++++ In my_disp_flush() ");
#ifndef DIRECT_RENDER_MODE
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);
#endif                       // #ifndef DIRECT_RENDER_MODE
  lv_disp_flush_ready(disp); /*Call it to tell LVGL you are ready*/
}

void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
  if (bbct.getSamples(&ti))
  {
    ti.x[0] = map(ti.x[0], 0, 1023, 0, w - 1); // X-Achse auf 0 - 1023 abgebildet
    ti.y[0] = map(ti.y[0], 0, 599, 0, h - 1);  // Y-Achse auf 0 - 599 abgebildet
    // lv_log("TI Anzahl: %d X: %d Y: %d Area: %d Press: %d\n", ti.count, ti.x[0], ti.y[0], ti.area[0], ti.pressure[0]);
    data->point.x = ti.x[0];
    data->point.y = ti.y[0];
    data->state = LV_INDEV_STATE_PRESSED;
  }
  else
  {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

void touchscreen_setup()
{
  gfx->begin();
  gfx->setRotation(0);
  delay(500);
  brightness_changed = false;
  brightness_value = 100;
  delay(100);
#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  analogWrite(GFX_BL, DEFAULT_BRIGHTNESS);
#endif
  bbct.init(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);
  iType = bbct.sensorType();
  lv_log("+++++ iType=%d \n", iType);
  w = gfx->width();
  h = gfx->height();
  n = min(w, h);
  n1 = n - 1;
  cx = w / 2;
  cy = h / 2;
  cx1 = cx - 1;
  cy1 = cy - 1;
  cn = min(cx1, cy1);
  cn1 = cn - 1;

  lv_tick_set_cb(millis_cb); // register Tick-sSstem

  screenWidth = gfx->width();
  screenHeight = gfx->height();

  lv_style_init(&green);
  lv_style_set_bg_color(&green, lv_palette_main(LV_PALETTE_LIME));
  lv_style_init(&yellow);
  lv_style_set_bg_color(&yellow, lv_palette_main(LV_PALETTE_AMBER));
  lv_style_init(&red);
  lv_style_set_bg_color(&red, lv_palette_main(LV_PALETTE_PURPLE));

#ifdef DIRECT_RENDER_MODE
  bufSize = screenWidth * screenHeight;
#else
  bufSize = screenWidth * 40;
#endif

#ifdef ESP32
  lv_log("+++++ ESP32 defined\n");
#if defined(DIRECT_RENDER_MODE) && (defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL))
  disp_draw_buf = (lv_color_t *)gfx->getFramebuffer();
  lv_log("+++++ DIRECT_RENDER_MODE defined \n");
#else  // !(defined(DIRECT_RENDER_MODE) && (defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL)))
  lv_log("+++++ DIRECT_RENDER_MODE not defined \n");
  disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  if (!disp_draw_buf)
  {
    lv_log("\n\n+++++ Creating buffers(1) failed \n");
    // remove MALLOC_CAP_INTERNAL flag try again
    disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_8BIT);
  }
#endif // !(defined(DIRECT_RENDER_MODE) && (defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL)))
#else  // !ESP32
  Serial.println("LVGL disp_draw_buf heap_caps_malloc failed! malloc again...\n");
  disp_draw_buf = (lv_color_t *)malloc(bufSize * 2);
#endif // !ESP32
  if (!disp_draw_buf)
  {
    lv_log("\n\n+++++ Creating buffers(2) failed \n");
  }
  else
  {
    disp = lv_display_create(screenWidth, screenHeight);
    lv_display_set_flush_cb(disp, my_disp_flush);
#ifdef DIRECT_RENDER_MODE
    lv_log("\n\n+++++ LV_DISPLAY_RENDER_MODE_DIRECT \n");
    lv_display_set_buffers(disp, disp_draw_buf, NULL, bufSize * 2, LV_DISPLAY_RENDER_MODE_DIRECT);
#else
    lv_log("\n\n+++++ LV_DISPLAY_RENDER_MODE_PARTIAL \n");
    lv_display_set_buffers(disp, disp_draw_buf, NULL, bufSize * 2, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif

    // Initialize the (dummy) input device driver
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
    lv_indev_set_read_cb(indev, my_touchpad_read);
  }
}

#endif // NMEA0183_CONF_H
