/**
 * @file mfd_tile_data.c
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  Tile data implementation. A tile_data is a small label container to present the actual meter readingfrom
  an NMEA tag.
*/
/*********************
 *      INCLUDES
 *********************/

#include <ui/mfd_tile_data.h>

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


 lv_obj_t *mfd_tile_data_create(lv_obj_t *tile)
{


  lv_obj_t *tile_data = lv_label_create(tile);
  lv_obj_set_name_static(tile_data, "mfd_tile_data");
  if(tile_data != NULL)
  {
  lv_log(" mfd_tile_data created %s\n", lv_obj_get_name(tile_data));
  } else
    lv_log("---->mfd_tile_data creation failed!\n");
  lv_obj_remove_style_all(tile_data);
  // As per V1.1.x tile_data is reused for the MOB panel and the font size is set based on the height of the tile. 
  // If the height of the tile is less than TILE_HEIGHT, the font size is set to 48, otherwise it is set to 96 and
  // alignmet is set to the bottom center of the tile. 
  if (lv_obj_get_style_height(tile, LV_PART_MAIN) < TILE_HEIGHT)
  {
    lv_obj_set_style_text_font(tile_data, &ui_font_lv_conthrax_48, 0);
    lv_obj_set_style_align(tile_data, LV_ALIGN_BOTTOM_MID, 0);
  }
  else
  {
    
  
  lv_obj_set_style_text_font(tile_data, &ui_font_lv_conthrax_96,0);
  lv_obj_set_style_align(tile_data, LV_ALIGN_CENTER, 0);
  }
  lv_label_set_text(tile_data,"...");
  return tile_data;
}