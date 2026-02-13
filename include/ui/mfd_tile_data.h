/**
 * @file mfd_data_box.h
 */

#ifndef MFD_TILE_DATA_H
#define MFD_TILE_DATA_H

#ifdef __cplusplus
extern "C"
{
#endif

  /*********************
   *      INCLUDES
   *********************/

#include <lvgl.h>
#include <ui/mfd_themes.h>
#include <font/mfd_fonts.h>

  /*********************
   *      DEFINES
   *********************/

  /**********************
   *      TYPEDEFS
   **********************/

  /**********************
   * GLOBAL PROTOTYPES
   **********************/
  lv_obj_t *mfd_tile_data_create(lv_obj_t *parent);

  /**********************
   *      MACROS
   **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MFD_DATA_BOX_H*/