/**
 * @file mfd_tile_gauge.h
 */

#ifndef MFD_TILE_GAUGE_H
#define MFD_TILE_GAUGE_H

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
  lv_obj_t *mfd_tile_gauge_create(lv_obj_t *tile);

  /**********************
   *      MACROS
   **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif //MFD_TILE_GAUGE_H