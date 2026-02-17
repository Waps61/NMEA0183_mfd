/**
 * @file mfd_bright_panel.h
 */ 
#ifndef MFD_BRIGHT_PANEL_H
#define MFD_BRIGHT_PANEL_H

#include <lvgl.h>
#include <ui/mfd_panel_gen.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif
  
  lv_obj_t * mfd_brightness_panel_create(lv_obj_t *parent, const char* title);
  extern void set_backlight(int value);
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // MFD_BRIGHT_PANEL_H