/**
 * @file mfd_panel.h
 */

#ifndef MFD_PANEL_GEN_H
#define MFD_PANEL_GEN_H

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
#define TILE_START_POS_X 10
#define TILE_START_POS_Y 20
#define MAX_NR_OF_TILES 6
#define TILE_SPACING_X 10
#define TILE_SPACING_Y 10
  /**********************
   *      TYPEDEFS
   **********************/
  typedef struct
  {
    lv_style_t style_main;
    lv_style_t style_scrollbar;
  } lv_panel_styles_t;

  struct _mfd_panel_t
  {
    int max_nr_of_tiles;
    int tile_spacing_x;
    int tile_spacing_y;
    int draw_pos_x;
    int draw_pos_y;
    int tile_width;
    int tile_height;
    int tile_count;
  };
  typedef struct _mfd_panel_t mfd_panel_t;

  // static int max_row = 2;
  // static int max_col = 3;
  // static int max_tiles = 6;
  // static int tile_index = 0;
  /**********************
   * GLOBAL PROTOTYPES
   **********************/
  void mfd_hide_panel(lv_obj_t *panel);
  void mfd_show_panel(lv_obj_t *panel);

  /**********************
   *      MACROS
   **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MFD_PANEL_GEN_H*/