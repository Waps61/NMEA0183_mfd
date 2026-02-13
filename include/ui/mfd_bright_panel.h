#ifndef MFD_BRIGHT_PANEL_H
#define MFD_BRIGHT_PANEL_H

#include <lvgl.h>


#ifdef __cplusplus
extern "C"
{
#endif
  
  void mfd_brightness_panel_create(lv_obj_t *parent);
  extern void set_backlight(int value);
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // MFD_BRIGHT_PANEL_H