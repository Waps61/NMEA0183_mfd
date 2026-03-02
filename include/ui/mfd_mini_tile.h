/**
 * @file mfd_mini_tile.h
 */

#ifndef MFD_MINI_TILE_H
#define MFD_MINI_TILE__H

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
#define MINI_TILE_WIDTH 250 // The width of the mini tile, set to 250 pixels
#define MINI_TILE_HEIGHT 65 // The height of the mini tile, set to 65 pixels

  /**********************
   *      TYPEDEFS
   **********************/

  /**********************
   * GLOBAL PROTOTYPES
   **********************/
  lv_obj_t *mfd_mini_tile_create(lv_obj_t *parent);
  void mfd_mini_tile_set_label(lv_obj_t *tile, const char *title);
  void mfd_mini_tile_set_unit(lv_obj_t *tile, const char *unit);
  lv_obj_t *mfd_mini_tile_add_data(lv_obj_t *tile);

  /**********************
   *      MACROS
   **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MFD_DATA_BOX_H*/