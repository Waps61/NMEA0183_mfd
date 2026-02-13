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
#include <NMEA0183_data.h>
#include <stdlib.h>

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
  int tile_count;
};
typedef struct _mfd_panel_t mfd_panel_t;

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
int max_row = 2;
int max_col = 3;
int max_tiles = 6;
int tile_index = 0;

void mfd_hide_panel(lv_obj_t *panel)
{
  if (!lv_obj_has_flag(panel, LV_OBJ_FLAG_HIDDEN))
    lv_obj_add_flag(panel, LV_OBJ_FLAG_HIDDEN);
}

void mfd_show_panel(lv_obj_t *panel)
{
  if (lv_obj_has_flag(panel, LV_OBJ_FLAG_HIDDEN))
    lv_obj_remove_flag(panel, LV_OBJ_FLAG_HIDDEN);
}

static void mfd_panel_style_observer_cb(lv_observer_t *observer, lv_subject_t *subject)
{
  LV_UNUSED(subject);

  lv_theme_mode_t m = (lv_theme_mode_t)lv_subject_get_int(&theme_subject);
  lv_panel_styles_t *styles = (lv_panel_styles_t *)lv_observer_get_target(observer);
  if (m == THEME_MODE_DAY)
  {
    lv_style_set_bg_color(&styles->style_main, lv_color_hex(DAY_BACKGROUND));
    lv_style_set_shadow_color(&styles->style_main, lv_color_hex(DAY_SECONDARY));
    lv_style_set_text_color(&styles->style_main, lv_color_hex(DAY_TEXT_ON_BACKGROUND));
    lv_style_set_bg_color(&styles->style_scrollbar, lv_color_hex(DAY_SURFACE));
  }
  if (m == THEME_MODE_NIGHT)
  {
    lv_style_set_bg_color(&styles->style_main, lv_color_hex(NIGHT_BACKGROUND));
    lv_style_set_shadow_color(&styles->style_main, lv_color_hex(NIGHT_SECONDARY));
    lv_style_set_text_color(&styles->style_main, lv_color_hex(NIGHT_TEXT_ON_BACKGROUND));
    lv_style_set_bg_color(&styles->style_scrollbar, lv_color_hex(NIGHT_SURFACE));
  }
  if (m == THEME_MODE_DAWN)
  {
    lv_style_set_bg_color(&styles->style_main, lv_color_hex(DAWN_BACKGROUND));
    lv_style_set_shadow_color(&styles->style_main, lv_color_hex(DAWN_SECONDARY));
    lv_style_set_text_color(&styles->style_main, lv_color_hex(DAWN_TEXT_ON_BACKGROUND));
    lv_style_set_bg_color(&styles->style_scrollbar, lv_color_hex(DAWN_SURFACE));
  }
  if (m == THEME_MODE_SUN)
  {
    lv_style_set_bg_color(&styles->style_main, lv_color_hex(SUN_BACKGROUND));
    lv_style_set_shadow_color(&styles->style_main, lv_color_hex(SUN_SECONDARY));
    lv_style_set_text_color(&styles->style_main, lv_color_hex(SUN_TEXT_ON_BACKGROUND));
    lv_style_set_bg_color(&styles->style_scrollbar, lv_color_hex(SUN_SURFACE));
  }

  lv_obj_report_style_change(&styles->style_main);
  lv_obj_report_style_change(&styles->style_scrollbar);
}

lv_obj_t *mfd_panel_add_tile(lv_obj_t *panel, char const *nmea_tag, char const *tag_unit, lv_obj_t *tile)
{
  lv_log("adding tile %s with unit %s to panel %s\n", nmea_tag, tag_unit, lv_obj_get_name(panel));
  mfd_panel_t *pdata = (mfd_panel_t *)lv_obj_get_user_data(panel);
  lv_log("paneldata :\n");

  lv_log("max_tiles: %d, spacing x,y: %d, %d, nt_tiles: %d\n", pdata->max_nr_of_tiles, pdata->tile_spacing_x, pdata->tile_spacing_y, pdata->tile_count);
  if (pdata->tile_count < pdata->max_nr_of_tiles)
  {
    tile = mfd_tile_create(panel);
    lv_obj_set_name_static(tile, "mfd_tile");
    mfd_set_tile_style(tile);
    lv_obj_set_style_x(tile, pdata->draw_pos_x, 0);
    lv_obj_set_style_y(tile, pdata->draw_pos_y, 0);
    pdata->tile_count++;
    pdata->draw_pos_x += TILE_WIDTH + pdata->tile_spacing_x;
    if (pdata->draw_pos_x > (MFD_SCREEN_WIDTH-TILE_WIDTH))
    {
      pdata->draw_pos_x = 10;
      pdata->draw_pos_y += TILE_HEIGHT + pdata->tile_spacing_y;
    }
    mfd_tile_set_label(tile, nmea_tag);
    mfd_tile_set_unit(tile, tag_unit);
    }
  return tile;
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
   

    lv_subject_add_observer_with_target(&theme_subject, mfd_panel_style_observer_cb, &styles, NULL);
  }

  lv_obj_t *panel = lv_obj_create(parent);
  lv_obj_set_name_static(panel, title);
  
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
