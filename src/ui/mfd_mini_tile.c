/**
 * @file mfd_mini_tile.c
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  Mini Tile data implementation. A mini tile is a small container below the datalabel in the Tile
*/
/*********************
 *      INCLUDES
 *********************/

#include <ui/mfd_mini_tile.h>

/*********************
 *      DEFINES
 *********************/



/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void mfd_mini_tile_set_label(lv_obj_t *tile, const char *title)
{
  lv_obj_t *label = lv_label_create(tile);
  lv_obj_add_style(label, &mfd_style_day, 0);
  lv_obj_set_style_width(label, 50,0);
  lv_obj_set_style_align(label, LV_ALIGN_LEFT_MID, 0);
  lv_label_set_text(label, title);
  lv_obj_set_style_text_font(label, &ui_font_lv_conthrax_16, 0);
}

void mfd_mini_tile_set_unit(lv_obj_t *tile, const char *unit)
{
  lv_obj_t *label = lv_label_create(tile);
  lv_obj_add_style(label, &mfd_style_day, 0);
  lv_obj_set_style_width(label, 50, 0);
  lv_obj_set_style_align(label, LV_ALIGN_RIGHT_MID, 0);
  //lv_obj_set_y(label, 55);
  lv_label_set_text(label, unit);
  lv_obj_set_style_text_font(label, &ui_font_lv_conthrax_16, 0);
  // lv_obj_set_align(label, LV_ALIGN_TOP_MID);
  lv_obj_set_style_text_color(label, lv_color_hex(NIGHT_TEXT_ON_BACKGROUND), 0);
}

lv_obj_t *mfd_mini_tile_add_data(lv_obj_t *tile)
{
  static bool inited = false;
  
  lv_obj_t *tile_data = lv_label_create(tile);
  lv_obj_set_name(tile_data, "mfd_mini_tile_data");
  if (tile_data != NULL)
  {
    lv_log(" mfd_mini_tile created %s\n", lv_obj_get_name(tile_data));
  }
  else
    lv_log("---->mfd_mini_tile creation failed!\n");
  lv_obj_set_style_align(tile_data, LV_ALIGN_CENTER, 0);
  // lv_obj_set_style_text_color(tile_data, lv_color_hex(DAY_PRIMARY), 0);

  lv_label_set_text(tile_data, "...");
  return tile_data;
}

 lv_obj_t *mfd_mini_tile_create(lv_obj_t *tile)
{
  static bool inited = false;
  

  lv_obj_t *mini_tile = lv_obj_create(tile);
  lv_obj_set_name_static(mini_tile, "mfd_mini_tile");
  if (mini_tile != NULL)
  {
    lv_log(" mfd_mini_tile created %s\n", lv_obj_get_name(mini_tile));
  } else
    lv_log("---->mfd_mini_tile creation failed!\n");
  lv_obj_remove_style_all(mini_tile);
  //lv_obj_add_style(tile_data, &mfd_style_night, 0);
  lv_obj_set_style_text_font(mini_tile, &ui_font_lv_conthrax_24, 0);
  lv_obj_set_align(mini_tile, LV_ALIGN_CENTER);
  //lv_obj_set_style_border_width(mini_tile,1,0);

  lv_obj_set_width(mini_tile,MINI_TILE_WIDTH);
  lv_obj_set_height(mini_tile,MINI_TILE_HEIGHT);
  lv_obj_set_y(mini_tile, 75);
  // lv_obj_set_style_text_color(tile_data, lv_color_hex(DAY_PRIMARY), 0);

  //lv_label_set_text(mini_tile, "...");
  return mini_tile;
}