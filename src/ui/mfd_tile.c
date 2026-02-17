/**
 * @file mfd_tile.c
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  Tile implementation. A tile is a small 260x260 container that holds a top center aligned
  caption, indicating what NMEA tag is displayed.
  The unit, like KTS or nm, are displayed omn the rigth just above the center
  An additiona tile_data object is centered on the tile to display the actual reading from the
  NMEA0183 inputs
  It also provides space for a mini-tile which can be used for secondary data not available on a tile
*/
/*********************
 *      INCLUDES
 *********************/
#include <lvgl.h>
#include <ui/mfd_themes.h>
#include <ui/mfd_tile.h>
#include <ui/mfd_tile_data.h>
#include <ui/mfd_mini_tile.h>
#include <NMEA0183_data.h>
#include <font/mfd_fonts.h>

/*********************
 *      DEFINES
 *********************/

typedef struct
{
  lv_style_t style_main;
  lv_style_t style_scrollbar;
} lv_data_box_styles_t;

/***********************
 *  STATIC VARIABLES
 **********************/
lv_obj_t *mini_tile_container;
/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

static void mfd_tile_style_observer_cb(lv_observer_t *observer, lv_subject_t *subject)
{
  LV_UNUSED(subject);

  lv_theme_mode_t m = (lv_theme_mode_t)lv_subject_get_int(&theme_subject);
  lv_data_box_styles_t *styles = (lv_data_box_styles_t *)lv_observer_get_target(observer);
  if (m == THEME_MODE_DAY)
  {
    lv_style_set_bg_color(&styles->style_main, lv_color_hex3(0xfff));
    lv_style_set_shadow_color(&styles->style_main, lv_color_hex3(0x888));
    lv_style_set_text_color(&styles->style_main, lv_color_hex3(0x222));
    lv_style_set_bg_color(&styles->style_scrollbar, lv_color_hex3(0x888));
  }
  if (m == THEME_MODE_NIGHT)
  {
    lv_style_set_bg_color(&styles->style_main, lv_color_hex(0x040038));
    lv_style_set_shadow_color(&styles->style_main, lv_color_hex3(0xaaa));
    lv_style_set_text_color(&styles->style_main, lv_color_hex3(0xeee));
    lv_style_set_bg_color(&styles->style_scrollbar, lv_color_hex3(0xaaa));
  }
  if (m == THEME_MODE_DAWN)
  {
    lv_style_set_bg_color(&styles->style_main, lv_color_hex3(0xfff));
    lv_style_set_shadow_color(&styles->style_main, lv_color_hex3(0x888));
    lv_style_set_text_color(&styles->style_main, lv_color_hex3(0x222));
    lv_style_set_bg_color(&styles->style_scrollbar, lv_color_hex3(0x888));
  }
  if (m == THEME_MODE_SUN)
  {
    lv_style_set_bg_color(&styles->style_main, lv_color_hex(0x040038));
    lv_style_set_shadow_color(&styles->style_main, lv_color_hex3(0xaaa));
    lv_style_set_text_color(&styles->style_main, lv_color_hex3(0xeee));
    lv_style_set_bg_color(&styles->style_scrollbar, lv_color_hex3(0xaaa));
  }

  lv_obj_report_style_change(&styles->style_main);
  lv_obj_report_style_change(&styles->style_scrollbar);
}

void mfd_tile_set_label(lv_obj_t *tile, const char *title)
{
  lv_obj_t *label = lv_label_create(tile);
  lv_obj_add_style(label, &mfd_style_day, 0);
  lv_obj_set_style_align(label, LV_ALIGN_TOP_MID, 0);
  lv_label_set_text(label, title);
  // lv_obj_set_align(label, LV_ALIGN_TOP_MID);
  //lv_obj_set_style_text_color(label, lv_color_hex(DAY_TEXT_ON_BACKGROUND), 0);
  lv_obj_set_style_text_font(label, &ui_font_lv_conthrax_24, 0);
}

void mfd_tile_set_unit(lv_obj_t *tile, const char *unit)
{
  lv_obj_t *label = lv_label_create(tile);
  lv_obj_add_style(label, &mfd_style_day, 0);
  lv_obj_set_style_align(label, LV_ALIGN_TOP_RIGHT, 0);
  lv_obj_set_y(label, 55);
  lv_label_set_text(label, unit);
  // lv_obj_set_align(label, LV_ALIGN_TOP_MID);
  lv_obj_set_style_text_color(label, lv_color_hex(NIGHT_TEXT_ON_BACKGROUND), 0);
}

lv_obj_t *mfd_tile_add_tile_data(lv_obj_t *tile, lv_obj_t *tiledata)
{
  lv_log("adding tile-data to tile ");

  if (tile != NULL)
  {
    lv_log("%s", lv_obj_get_name(tile));

    tiledata = mfd_tile_data_create(tile);
    lv_log("with name %s\n", lv_obj_get_name(tiledata));
  }
  else
    lv_log("\n----->failure: tile not initialized\n");
  return tiledata;
}

lv_obj_t *mfd_tile_add_mini_tile(lv_obj_t *tile, lv_obj_t *minitile)
{
  lv_log("adding mini-tile to tile ");
  if(tile != NULL)
  {
    minitile = mfd_mini_tile_create(tile);
  }
  else
    lv_log("\n----->failure: minitile not initialized\n");
  return minitile;
}

lv_obj_t *mfd_tile_create(lv_obj_t *parent)
{
  static bool inited = false;
  static lv_data_box_styles_t styles;
  if (!inited)
  {
    inited = true;

    // lv_style_init(&styles.style_main);
    // lv_style_init(&styles.style_scrollbar);
    lv_subject_add_observer_with_target(&theme_subject, mfd_tile_style_observer_cb, &styles, NULL);
  }

  lv_obj_t *tile = lv_obj_create(parent);
  lv_obj_set_name(tile, "mfd_tile");
  mfd_set_tile_style(tile);
  lv_obj_set_width(tile, TILE_WIDTH);
  lv_obj_set_height(tile, TILE_HEIGHT);

  
  return tile;
}