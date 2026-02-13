/**
 * @file mfd_panel.h
 */

#ifndef MFD_PANEL_H
#define MFD_PANEL_H

#ifdef __cplusplus
extern "C"
{
#endif

  /*********************
   *      INCLUDES
   *********************/

#include <lvgl.h>

  /*********************
   *      DEFINES
   *********************/

  /**********************
   *      TYPEDEFS
   **********************/
  
  /**********************
   * GLOBAL PROTOTYPES
   **********************/
  void mfd_hide_panel(lv_obj_t *panel);
  void mfd_show_panel(lv_obj_t *panel);
  lv_obj_t *mfd_panel_add_tile(lv_obj_t *panel, char const *nmea_tag, char const *tag_unit, lv_obj_t *data_tile);
  lv_obj_t *mfd_panel_create(lv_obj_t *parent, const char *title);

  /**********************
   *      MACROS
   **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MFD_PANEL_H*/