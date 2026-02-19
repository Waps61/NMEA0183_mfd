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

 
  extern void testlab_init();
  extern void recolor_tree();

#ifdef __cplusplus
} /*extern "C"*/
#endif
#endif // TESTLAB_H