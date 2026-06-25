/**
 * @file mfd_panel.c
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  Main panel implementation. The main panels has helper function to create the other panels
*/
/*********************
 *      INCLUDES
 *********************/
#include <lvgl.h>
#include <mfd_conf.h>
#include <ui/mfd_themes.h>
#include <ui/mfd_panel.h>
#include <font/mfd_fonts.h>
#include <ui/mfd_tile.h>
#include <ui/mfd_tile_gauge.h>
#include <NMEA0183_data.h>
#include <stdlib.h>

/*********************
 *      DEFINES
 *********************/


/**********************
 *      TYPEDEFS
 **********************/


/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t *mfd_panel_add_gauge(lv_obj_t *panel, char const *nmea_tag, lv_obj_t *gauge_tile)
{
  mfd_panel_t *pdata = (mfd_panel_t *)lv_obj_get_user_data(panel);

  if (pdata->tile_count < pdata->max_nr_of_tiles)
  {
    gauge_tile = mfd_tile_create(panel);
    lv_obj_set_name_static(gauge_tile, "mfd_gauge");
    mfd_set_tile_style(gauge_tile);
    mfd_set_style_day(gauge_tile);
    lv_obj_set_style_x(gauge_tile, pdata->draw_pos_x, 0);
    lv_obj_set_style_y(gauge_tile, pdata->draw_pos_y, 0);
    pdata->tile_count++;
    pdata->draw_pos_x += pdata->tile_width + pdata->tile_spacing_x;
    if (pdata->draw_pos_x > (MFD_SCREEN_WIDTH - pdata->tile_width))
    {
      pdata->draw_pos_x = 10;
      pdata->draw_pos_y += pdata->tile_height + pdata->tile_spacing_y;
    }
    mfd_gauge_set_label(gauge_tile, nmea_tag);
    //lv_obj_align(gauge_tile, LV_ALIGN_BOTTOM_MID,0,-10);
    
  }
  return gauge_tile;
}


lv_obj_t *mfd_panel_add_tile(lv_obj_t *panel, char const *nmea_tag, char const *tag_unit, lv_obj_t *tile)
{
  mfd_panel_t *pdata = (mfd_panel_t *)lv_obj_get_user_data(panel);
  if (pdata->tile_count < pdata->max_nr_of_tiles)
  {
    tile = mfd_tile_create(panel);
    lv_obj_set_name_static(tile, "mfd_tile");
    mfd_set_tile_style(tile);
    mfd_set_style_day(tile);
    lv_obj_set_style_x(tile, pdata->draw_pos_x, 0);
    lv_obj_set_style_y(tile, pdata->draw_pos_y, 0);
    pdata->tile_count++;
    pdata->draw_pos_x += pdata->tile_width + pdata->tile_spacing_x;
    if (pdata->draw_pos_x > (MFD_SCREEN_WIDTH - pdata->tile_width))
    {
      pdata->draw_pos_x = 10;
      pdata->draw_pos_y += pdata->tile_height + pdata->tile_spacing_y;
    }
    mfd_tile_set_label(tile, nmea_tag);
    mfd_tile_set_unit(tile, tag_unit);
    }
  return tile;
}
/**
 * Add an empty space in the size of tile to the panel
 */
void mfd_panel_add_spacer(lv_obj_t *panel)
{
  mfd_panel_t *pdata = (mfd_panel_t *)lv_obj_get_user_data(panel);
  lv_obj_t *spacer = mfd_tile_create(panel);
  lv_obj_set_style_x(spacer, pdata->draw_pos_x, 0);
  lv_obj_set_style_y(spacer, pdata->draw_pos_y, 0);
  if (pdata->tile_count < pdata->max_nr_of_tiles)
  {
    pdata->tile_count++;
    pdata->draw_pos_x += pdata->tile_width + pdata->tile_spacing_x;
    if (pdata->draw_pos_x > (MFD_SCREEN_WIDTH - pdata->tile_width))
    {
      pdata->draw_pos_x = 10;
      pdata->draw_pos_y += pdata->tile_height + pdata->tile_spacing_y;
    }
  }
  }

lv_obj_t *mfd_panel_create(lv_obj_t *parent, const char *title) 
{
  static bool inited = false;
  static lv_panel_styles_t styles;
  if (!inited)
  {
    inited = true;

    lv_style_init(&styles.style_main);
   

    lv_style_init(&styles.style_scrollbar);
   

    
  }

  lv_obj_t *panel = lv_obj_create(parent);
  lv_obj_set_name(panel, title);
  mfd_set_style(panel);
  mfd_set_style_sun(panel);
  lv_obj_set_width(panel, 865);
  lv_obj_set_height(panel, lv_pct(98));
  lv_obj_set_x(panel, 150);
  lv_obj_set_y(panel, 0);

  mfd_panel_t *paneldata; 
  paneldata = (mfd_panel_t*)malloc(sizeof(mfd_panel_t));
  paneldata->draw_pos_x = TILE_START_POS_X;
  paneldata->draw_pos_y = TILE_START_POS_Y;
  paneldata->max_nr_of_tiles = MAX_NR_OF_TILES;
  paneldata->tile_count = 0;
  paneldata->tile_spacing_x = TILE_SPACING_X;
  paneldata->tile_spacing_y = TILE_SPACING_Y;
  paneldata->tile_width = TILE_WIDTH;
  paneldata->tile_height = TILE_HEIGHT;
  lv_obj_set_user_data(panel, paneldata);

  if (title != NULL)
  {
    lv_obj_t *label = lv_label_create(panel);
    lv_obj_set_style_align(label, LV_ALIGN_TOP_MID, 0);
    lv_label_set_text(label, title);
    lv_obj_set_align(label, LV_ALIGN_TOP_MID);
  }

  return panel;
}
