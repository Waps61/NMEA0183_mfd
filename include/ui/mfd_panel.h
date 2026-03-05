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
#include <ui/mfd_panel_gen.h>

  /*********************
   *      DEFINES
   *********************/

  /**********************
   *      TYPEDEFS
   **********************/
  
  /**********************
   * GLOBAL PROTOTYPES
   **********************/
  lv_obj_t *mfd_panel_add_tile(lv_obj_t *panel, char const *nmea_tag, char const *tag_unit, lv_obj_t *data_tile);
  lv_obj_t *mfd_panel_add_gauge(lv_obj_t *panel, char const *nmea_tag,  lv_obj_t *gauge_tile);
  lv_obj_t *mfd_panel_create(lv_obj_t *parent, const char *title);
  void mfd_panel_add_spacer(lv_obj_t *panel);

  /**********************
   *      MACROS
   **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MFD_PANEL_H*/