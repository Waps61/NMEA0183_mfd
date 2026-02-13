/**
 * @file mfd_themes.c
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

typedef struct
{
  lv_style_t style_main;
  lv_style_t style_scrollbar;
} lv_data_box_styles_t;

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

static void mfd_tile_data_style_observer_cb(lv_observer_t *observer, lv_subject_t *subject)
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

 lv_obj_t *mfd_tile_data_create(lv_obj_t *tile)
{
  static bool inited = false;
  static lv_data_box_styles_t styles;
  if (!inited)
  {
    inited = true;

    lv_style_init(&styles.style_main);
    

    lv_style_init(&styles.style_scrollbar);
    

    lv_subject_add_observer_with_target(&theme_subject, mfd_tile_data_style_observer_cb, &styles, NULL);
  }

  lv_obj_t *tile_data = lv_label_create(tile);
  lv_obj_set_name_static(tile_data, "mfd_tile_data");
  if(tile_data != NULL)
  {
  lv_log(" mfd_tile_data created %s\n", lv_obj_get_name(tile_data));
  } else
    lv_log("---->mfd_tile_data creation failed!\n");
  lv_obj_remove_style_all(tile_data);
  //lv_obj_add_style(tile_data, &mfd_style_night, 0);
  lv_obj_set_style_text_font(tile_data, &ui_font_lv_conthrax_96,0);
  lv_obj_set_style_align(tile_data, LV_ALIGN_CENTER, 0);
  //lv_obj_set_style_text_color(tile_data, lv_color_hex(DAY_PRIMARY), 0);

  lv_label_set_text(tile_data,"...");
  return tile_data;
}