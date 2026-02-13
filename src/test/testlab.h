#ifndef TESTLAB_H
#define TESTLAB_h

/**
 * Playground For testing specific situation
 */

#ifdef __cplusplus
extern "C"
{
#endif
#include <Arduino.h>
#include <lvgl.h>
#include <mfd_conf.h>
#include <ui/mfd_themes.h>
#include <ui/mfd_panel.h>
#include <ui/mfd_btn.h>
 static  lv_obj_t *test_screen = NULL;

 typedef struct
 {
   lv_style_t style_main;
   lv_style_t style_pressed;
 } lv_button_styles_t;

  extern void testlab_init();

#ifdef __cplusplus
} /*extern "C"*/
#endif
#endif // TESTLAB_H