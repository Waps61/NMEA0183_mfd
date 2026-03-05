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
#define GAUGE_WIDTH 250
#define GAUGE_HEIGHT 250
  /**********************
   *      TYPEDEFS
   **********************/

  /**********************
   * GLOBAL PROTOTYPES
   **********************/
  void mfd_gauge_update(lv_obj_t *scale, int range, int value);
  void mfd_gauge_set_label(lv_obj_t *tile, const char *title);
  lv_obj_t *mfd_tile_gauge_create(lv_obj_t *tile, int range, int sector_start, int sector_end);

  /**********************
   *      MACROS
   **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // MFD_TILE_GAUGE_H