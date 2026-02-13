/**
 * @file mfd_data_box.h
 */

#ifndef MFD_TILE_H
#define MFD_TILE_H

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
#define TILE_WIDTH 260
#define TILE_HEIGHT 260
  /**********************
   *      TYPEDEFS
   **********************/

  /**********************
   * GLOBAL PROTOTYPES
   **********************/
  lv_obj_t *mfd_tile_create(lv_obj_t *parent);
  void mfd_tile_set_label(lv_obj_t *tile, const char *title);
  void mfd_tile_set_unit(lv_obj_t *tile, const char *unit);
  lv_obj_t *mfd_tile_add_tile_data(lv_obj_t *tile, lv_obj_t *tiledata);
  lv_obj_t *mfd_tile_add_mini_tile(lv_obj_t *tile, lv_obj_t *minitile);
  /**********************
   *      MACROS
   **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MFD_DATA_BOX_H*/