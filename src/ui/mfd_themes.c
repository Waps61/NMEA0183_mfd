/**
 * @file mfd_themes.c
 */

/*********************
 *      INCLUDES
 *********************/
#include <lvgl.h>
#include <ui/mfd_themes.h>
#include <font/mfd_fonts.h>
#include <string.h>
#include <stdio.h>

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
 * STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*
 * eventhandler for switching themes
 */
static void switch_theme_event_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  switch (lv_subject_get_int(&theme_subject))
  {
  case THEME_MODE_NIGHT:
    lv_subject_set_int(&theme_subject, THEME_MODE_NIGHT);
    break;

  case THEME_MODE_DAWN:
    lv_subject_set_int(&theme_subject, THEME_MODE_DAWN);
    break;

  case THEME_MODE_SUN:
    lv_subject_set_int(&theme_subject, THEME_MODE_SUN);
    break;

  default:
    lv_subject_set_int(&theme_subject, THEME_MODE_DAY);
    break;
  }
}

void mfd_update_style(lv_obj_t *parent, lv_style_t *newstyle)
{
  uint32_t i;
  lv_obj_t *chief = lv_obj_get_parent(parent);
  lv_obj_t *the_boss = NULL;
  char chief_name[50] = {0};
  // sprintf(chief_name,"%s",lv_obj_get_name(chief));
  lv_log("init chief's name = %s\n", lv_obj_get_name(chief));
  while (strcmp(chief_name, "screen_main_#") != 0)
  {
    /* code */
    chief = lv_obj_get_parent(chief);
    sprintf(chief_name, "%s", lv_obj_get_name(chief));
    lv_log("chief's name = %s\n", lv_obj_get_name(chief));
    the_boss = chief;
  }
  lv_log("the boss's name = %s\n", lv_obj_get_name(the_boss));
  uint32_t count = lv_obj_get_child_count(the_boss);
  lv_log("the boss has %d children", count);
  for (i = 0; i < count; i++)
  {
    lv_obj_t *child = lv_obj_get_child(parent, i);
    /* Do something with `child`. */

    lv_obj_replace_style(child, &mfd_style, newstyle, 0);
  }
  lv_style_copy(&mfd_style, newstyle);
}

void mfd_init_styles()
{
  mfd_styles_inited = true;
  lv_style_init(&mfd_style_day);
  lv_style_init(&mfd_style_night);
  lv_style_init(&mfd_style_sun);
  lv_style_init(&mfd_style_dawn);
  lv_style_init(&mfd_style_btn);
  lv_style_init(&mfd_style_btn_pressed);
  lv_style_init(&mfd_style_tile);
  lv_style_init(&mfd_style_menubar);
  lv_style_init(&mfd_style);
  lv_style_copy(&mfd_style, &mfd_style_day);
}

lv_obj_t *mfd_set_style_day(lv_obj_t *obj)
{
  lv_style_set_bg_color(&mfd_style_day, lv_color_hex(DAY_BACKGROUND));
  lv_style_set_bg_opa(&mfd_style_day, LV_OPA_50);
  lv_style_set_text_color(&mfd_style_day, lv_color_hex(DAY_TEXT_ON_PRIMARY));
  lv_style_set_text_opa(&mfd_style_day, LV_OPA_50);
  lv_style_set_text_font(&mfd_style_day, &ui_font_lv_conthrax_16);
  lv_style_set_border_color(&mfd_style_day, lv_color_hex(DAY_TEXT_ON_PRIMARY));
  lv_style_set_border_width(&mfd_style_day, 1);
  lv_style_set_text_font(&mfd_style_night, &ui_font_lv_conthrax_16);
  lv_style_set_text_align(&mfd_style_day, LV_ALIGN_CENTER);

  lv_style_set_radius(&mfd_style_day, 12);
  lv_style_set_pad_all(&mfd_style_day, 12);
  lv_style_set_pad_gap(&mfd_style_day, 16);
  lv_style_set_shadow_width(&mfd_style_day, 24);
  lv_style_set_shadow_offset_x(&mfd_style_day, 4);
  lv_style_set_shadow_offset_y(&mfd_style_day, 6);
  lv_style_set_shadow_color(&mfd_style_day, lv_color_hex(DAY_SURFACE));
  lv_style_set_shadow_width(&mfd_style_day, 5);
  lv_obj_add_style(obj, &mfd_style_day, 0);
  lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
  return obj;
}

lv_obj_t *mfd_set_style_night(lv_obj_t *obj)
{
  lv_style_set_bg_color(&mfd_style_night, lv_color_hex(NIGHT_BACKGROUND));
  lv_style_set_bg_opa(&mfd_style_night, LV_OPA_50);
  lv_style_set_text_color(&mfd_style_night, lv_color_hex(NIGHT_TEXT_ON_BACKGROUND));
  lv_style_set_text_opa(&mfd_style_night, LV_OPA_50);
  lv_style_set_border_color(&mfd_style_night, lv_color_hex(NIGHT_TEXT_ON_PRIMARY));
  lv_style_set_border_width(&mfd_style_night, 1);
  lv_style_set_text_font(&mfd_style_night, &ui_font_lv_conthrax_16);
  lv_style_set_text_align(&mfd_style_night, LV_ALIGN_CENTER);
  lv_style_set_shadow_width(&mfd_style_day, 24);
  lv_style_set_shadow_offset_x(&mfd_style_night, 4);
  lv_style_set_shadow_offset_y(&mfd_style_night, 6);
  lv_style_set_shadow_color(&mfd_style_night, lv_color_hex(NIGHT_SURFACE));
  lv_style_set_shadow_width(&mfd_style_night, 5);
  lv_obj_add_style(obj, &mfd_style_night, 0);
  lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
  return obj;
}

lv_obj_t *mfd_set_style_sun(lv_obj_t *obj)
{
  lv_style_set_bg_color(&mfd_style_sun, lv_color_hex(SUN_BACKGROUND));
  lv_style_set_bg_opa(&mfd_style_sun, LV_OPA_50);
  lv_style_set_text_color(&mfd_style_sun, lv_color_hex(SUN_TEXT_ON_BACKGROUND));
  lv_style_set_text_opa(&mfd_style_sun, LV_OPA_50);
  lv_style_set_text_font(&mfd_style_sun, &ui_font_lv_conthrax_16);
  lv_style_set_border_color(&mfd_style_sun, lv_color_hex(SUN_TEXT_ON_PRIMARY));
  lv_style_set_border_width(&mfd_style_sun, 1);
  lv_style_set_text_font(&mfd_style_sun, &ui_font_lv_conthrax_16);
  lv_style_set_text_align(&mfd_style_sun, LV_ALIGN_CENTER);
  lv_style_set_shadow_width(&mfd_style_sun, 24);
  lv_style_set_shadow_offset_x(&mfd_style_sun, 4);
  lv_style_set_shadow_offset_y(&mfd_style_sun, 6);
  lv_style_set_shadow_color(&mfd_style_sun, lv_color_hex(SUN_SURFACE));
  lv_style_set_shadow_width(&mfd_style_sun, 5);

  lv_obj_add_style(obj, &mfd_style_sun, 0);
  lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
  return obj;
}

lv_obj_t *mfd_set_obj_style_dawn(lv_obj_t *obj)
{
  lv_style_set_bg_color(&mfd_style_dawn, lv_color_hex(DAWN_BACKGROUND));
  lv_style_set_bg_opa(&mfd_style_dawn, LV_OPA_50);
  lv_style_set_text_color(&mfd_style_dawn, lv_color_hex(DAWN_TEXT_ON_BACKGROUND));
  lv_style_set_text_opa(&mfd_style_dawn, LV_OPA_50);
  lv_style_set_text_font(&mfd_style_dawn, &ui_font_lv_conthrax_16);
  lv_style_set_border_color(&mfd_style_dawn, lv_color_hex(DAWN_TEXT_ON_PRIMARY));
  lv_style_set_border_width(&mfd_style_dawn, 1);
  lv_style_set_text_font(&mfd_style_dawn, &ui_font_lv_conthrax_16);
  lv_style_set_text_align(&mfd_style_dawn, LV_ALIGN_CENTER);
  lv_style_set_shadow_width(&mfd_style_dawn, 24);
  lv_style_set_shadow_offset_x(&mfd_style_dawn, 4);
  lv_style_set_shadow_offset_y(&mfd_style_dawn, 6);
  lv_style_set_shadow_color(&mfd_style_dawn, lv_color_hex(DAWN_SURFACE));
  lv_style_set_shadow_width(&mfd_style_dawn, 5);
  lv_obj_add_style(obj, &mfd_style_dawn, 0);
  lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
  return obj;
}

lv_obj_t *mfd_set_tile_style(lv_obj_t *tile)
{
  lv_style_set_radius(&mfd_style_tile, 12);
  lv_style_set_bg_color(&mfd_style_tile, lv_color_hex(DAY_BACKGROUND));
  lv_style_set_text_color(&mfd_style_tile, lv_color_hex(DAY_TEXT_ON_BACKGROUND));
  lv_style_set_bg_opa(&mfd_style_tile, LV_OPA_COVER);
  lv_style_set_shadow_width(&mfd_style_tile, 24);
  lv_style_set_shadow_offset_x(&mfd_style_tile, 4);
  lv_style_set_shadow_offset_y(&mfd_style_tile, 6);
  lv_style_set_pad_all(&mfd_style_tile, 12);
  lv_style_set_pad_gap(&mfd_style_tile, 16);
  lv_obj_add_style(tile, &mfd_style_tile, 0);
  lv_obj_set_scrollbar_mode(tile, LV_SCROLLBAR_MODE_OFF);
  return tile;
}

lv_obj_t *mfd_set_btn_style(lv_obj_t *btn)
{
  lv_style_set_bg_color(&mfd_style_btn, lv_color_hex(DAWN_PRIMARY));
  lv_style_set_text_color(&mfd_style_btn, lv_color_hex(DAY_TEXT_ON_PRIMARY));
  lv_style_set_width(&mfd_style_btn, 100);
  lv_style_set_height(&mfd_style_btn, 80);
  lv_style_set_max_width(&mfd_style_btn, lv_pct(100));
  lv_style_set_min_width(&mfd_style_btn, lv_pct(100));
  lv_style_set_max_height(&mfd_style_btn, 100);
  lv_style_set_max_height(&mfd_style_btn_pressed, 100);
  lv_style_set_min_height(&mfd_style_btn, 75);
  lv_style_set_pad_all(&mfd_style_btn, 10);
  lv_style_set_margin_all(&mfd_style_btn, 10);
  lv_style_set_shadow_color(&mfd_style_btn, lv_color_hex(DAWN_SURFACE));
  lv_style_set_shadow_width(&mfd_style_btn, 5);
  lv_style_set_shadow_offset_x(&mfd_style_btn, 4);
  lv_style_set_shadow_offset_y(&mfd_style_btn, 6);
  lv_obj_add_style(btn, &mfd_style_btn, 0);
  lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_set_style_bg_color(btn, lv_color_hex(0x008591), LV_STATE_CHECKED);
  lv_obj_set_scrollbar_mode(btn, LV_SCROLLBAR_MODE_OFF);
  return btn;
}

lv_obj_t *mfd_set_btn_style_pressed(lv_obj_t *btn)
{
  lv_style_set_width(&mfd_style_btn_pressed, 100);
  lv_style_set_height(&mfd_style_btn_pressed, 80);
  lv_style_set_max_width(&mfd_style_btn_pressed, lv_pct(100));
  lv_style_set_min_width(&mfd_style_btn_pressed, lv_pct(100));
  lv_style_set_min_height(&mfd_style_btn_pressed, 75);
  lv_style_set_pad_all(&mfd_style_btn_pressed, 10);
  lv_style_set_margin_all(&mfd_style_btn_pressed, 10);
  lv_style_set_shadow_color(&mfd_style_btn_pressed, lv_color_lighten(lv_color_hex(DAWN_SECONDARY), LV_OPA_40));
  lv_style_set_shadow_width(&mfd_style_btn_pressed, 5);
  lv_obj_set_style_bg_color(btn, lv_color_hex(0xFFA500), LV_STATE_CHECKED);
  lv_obj_add_style(btn, &mfd_style_btn_pressed, LV_STATE_CHECKED);
  return btn;
}

lv_obj_t *mfd_set_menu_bar_style(lv_obj_t *mbar)
{
  lv_style_set_flex_flow(&mfd_style_menubar, LV_FLEX_FLOW_COLUMN);
  lv_style_set_bg_color(&mfd_style_menubar, lv_color_hex(DAWN_BACKGROUND));
  lv_style_set_bg_opa(&mfd_style_menubar, LV_OPA_50);
  lv_style_set_text_color(&mfd_style_menubar, lv_color_hex(DAWN_TEXT_ON_BACKGROUND));
  lv_style_set_text_opa(&mfd_style_menubar, LV_OPA_50);
  lv_style_set_text_font(&mfd_style_menubar, &ui_font_lv_conthrax_16);
  lv_style_set_border_color(&mfd_style_menubar, lv_color_hex(DAWN_TEXT_ON_PRIMARY));
  lv_style_set_border_width(&mfd_style_menubar, 1);
  lv_style_set_text_font(&mfd_style_menubar, &ui_font_lv_conthrax_16);
  lv_style_set_text_align(&mfd_style_menubar, LV_ALIGN_CENTER);
  lv_style_set_x(&mfd_style_menubar, 0);
  lv_style_set_y(&mfd_style_menubar, 0);
  lv_style_set_width(&mfd_style_menubar, 150);
  lv_style_set_height(&mfd_style_menubar, lv_pct(100));
  lv_style_set_radius(&mfd_style_menubar, 5);
  lv_style_set_margin_all(&mfd_style_menubar, 5);
  lv_obj_add_style(mbar, &mfd_style_menubar, 0);
  return mbar;
}