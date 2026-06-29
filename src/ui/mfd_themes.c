/**
 * @file mfd_themes.c
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 ,gmail.com
  Themes and Style implementation. It is used to initilialize the thems and style in use and and funuctions
  for swithing Thems and Syles
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
int mfd_get_style_changed()
{
  return mfd_style_changed;
}
void mfd_set_style_changed(int styleID)
{
  mfd_style_changed = styleID;
}

/**
 * Update the style of an object to the new style.
 * This function is called recursively for all child objects of the parent object, to update their styles as well.
 * The function checks if the new style has a background color, text color, border color or shadow color property,
 * and if so, it updates the corresponding style of the child object. The background color of the parent object is
 * also updated to the new background color of the style.
 */
void mfd_update_style(lv_obj_t *parent, lv_style_t *newstyle)
{

  uint32_t i;
  uint32_t count = lv_obj_get_child_count(parent);
  lv_style_value_t sval, bg_main;
  for (i = 0; i < count; i++)
  {
    // lv_log("child nr %d \n", i);
    lv_obj_t *child = lv_obj_get_child(parent, i);
    if (child != NULL)
    {

      mfd_update_style(child, newstyle);
      if (LV_RESULT_OK == lv_style_get_prop(newstyle, LV_STYLE_BG_COLOR, &sval))
      {
        lv_obj_set_style_bg_color(child, sval.color, 0);
        bg_main = sval;
      }
      // else
      // lv_log("no bg_color found.\n");
      if (LV_RESULT_OK == lv_style_get_prop(newstyle, LV_STYLE_TEXT_COLOR, &sval))
        lv_obj_set_style_text_color(child, sval.color, 0);
      
      if (LV_RESULT_OK == lv_style_get_prop(newstyle, LV_STYLE_BORDER_COLOR, &sval))
        lv_obj_set_style_border_color(child, sval.color, 0);
      
      if (LV_RESULT_OK == lv_style_get_prop(newstyle, LV_STYLE_SHADOW_COLOR, &sval))
        lv_obj_set_style_shadow_color(child, sval.color, 0);
      
    }
  }
  if (lv_color_eq(bg_main.color, lv_color_hex(DAY_BACKGROUND)))
    bg_main.color = lv_palette_lighten(LV_PALETTE_GREY, 3);
  lv_obj_set_style_bg_color(parent, bg_main.color, 0);
  lv_obj_remove_state(parent, LV_STATE_PRESSED | LV_STATE_FOCUSED | LV_STATE_CHECKED);
}

/**
 * Initialize the styles for the MFD application.
 */
void mfd_init_styles()
{
  mfd_styles_inited = true;
  mfd_style_changed = UNCHANGED_STYLE;
  lv_style_init(&mfd_style_day);
  lv_style_init(&mfd_style_night);
  lv_style_init(&mfd_style_sun);
  lv_style_init(&mfd_style_dawn);

  lv_style_set_bg_color(&mfd_style_day, lv_color_hex(DAY_BACKGROUND));
  lv_style_set_text_color(&mfd_style_day, lv_color_hex(DAY_TEXT_ON_BACKGROUND));
  lv_style_set_border_color(&mfd_style_day, lv_color_hex(DAY_SECONDARY));
  lv_style_set_shadow_color(&mfd_style_day, lv_color_hex(DAY_SURFACE));
  lv_style_set_line_color(&mfd_style_day, lv_color_hex(DAY_LINE_COLOR));

  lv_style_set_bg_color(&mfd_style_night, lv_color_hex(NIGHT_BACKGROUND));
  lv_style_set_text_color(&mfd_style_night, lv_color_hex(NIGHT_TEXT_ON_BACKGROUND));
  lv_style_set_border_color(&mfd_style_night, lv_color_hex(NIGHT_SECONDARY));
  lv_style_set_shadow_color(&mfd_style_night, lv_color_hex(NIGHT_SURFACE));
  lv_style_set_line_color(&mfd_style_night, lv_color_hex(NIGHT_LINE_COLOR));

  lv_style_set_bg_color(&mfd_style_sun, lv_color_hex(SUN_BACKGROUND));
  lv_style_set_text_color(&mfd_style_sun, lv_color_hex(SUN_TEXT_ON_BACKGROUND));
  lv_style_set_border_color(&mfd_style_sun, lv_color_hex(SUN_SECONDARY));
  lv_style_set_shadow_color(&mfd_style_sun, lv_color_hex(SUN_SURFACE));
  lv_style_set_line_color(&mfd_style_sun, lv_color_hex(SUN_LINE_COLOR));

  lv_style_set_bg_color(&mfd_style_dawn, lv_color_hex(DAWN_BACKGROUND));
  lv_style_set_text_color(&mfd_style_dawn, lv_color_hex(DAWN_TEXT_ON_BACKGROUND));
  lv_style_set_border_color(&mfd_style_dawn, lv_color_hex(DAWN_SECONDARY));
  lv_style_set_shadow_color(&mfd_style_dawn, lv_color_hex(DAWN_SURFACE));
  lv_style_set_line_color(&mfd_style_dawn, lv_color_hex(DAWN_LINE_COLOR));

  lv_style_init(&mfd_style_tile);
  lv_style_merge(&mfd_style_tile, &mfd_style_day);
  lv_style_init(&mfd_style_menubar);
  lv_style_init(&mfd_style);
  lv_style_copy(&mfd_style, &mfd_style_day);
  lv_style_init(&mfd_style_btn);
  lv_style_merge(&mfd_style_btn, &mfd_style_day);

  lv_style_set_bg_opa(&mfd_style, LV_OPA_50);
  lv_style_set_text_opa(&mfd_style, LV_OPA_50);
  lv_style_set_text_font(&mfd_style, &ui_font_lv_conthrax_16);
  lv_style_set_border_width(&mfd_style, 1);
  lv_style_set_text_align(&mfd_style, LV_ALIGN_CENTER);
  lv_style_set_radius(&mfd_style, 12);
  lv_style_set_pad_all(&mfd_style, 12);
  lv_style_set_pad_gap(&mfd_style, 16);
  lv_style_set_shadow_width(&mfd_style, 24);
  lv_style_set_shadow_offset_x(&mfd_style, 4);
  lv_style_set_shadow_offset_y(&mfd_style, 6);
  lv_style_set_shadow_width(&mfd_style, 5);

  lv_style_set_radius(&mfd_style_tile, 12);
  // lv_style_set_bg_color(&mfd_style_tile, lv_color_hex(DAY_BACKGROUND));
  // lv_style_set_text_color(&mfd_style_tile, lv_color_hex(DAY_TEXT_ON_BACKGROUND));
  // lv_style_set_bg_opa(&mfd_style_tile, LV_OPA_COVER);
  lv_style_set_shadow_width(&mfd_style_tile, 24);
  lv_style_set_shadow_offset_x(&mfd_style_tile, 4);
  lv_style_set_shadow_offset_y(&mfd_style_tile, 6);
  lv_style_set_pad_all(&mfd_style_tile, 12);
  lv_style_set_pad_gap(&mfd_style_tile, 16);

  // lv_style_set_text_color(&mfd_style_btn, lv_color_hex(DAY_TEXT_ON_PRIMARY));
  // lv_style_set_bg_color(&mfd_style_btn, lv_color_hex(DAY_PRIMARY));
  lv_style_set_width(&mfd_style_btn, 100);
  lv_style_set_height(&mfd_style_btn, 80);
  lv_style_set_max_width(&mfd_style_btn, lv_pct(100));
  lv_style_set_min_width(&mfd_style_btn, lv_pct(100));
  lv_style_set_max_height(&mfd_style_btn, 100);
  lv_style_set_max_height(&mfd_style_btn_pressed, 100);
  lv_style_set_min_height(&mfd_style_btn, 75);
  // lv_style_set_border_color(&mfd_style_btn, lv_color_hex(DAY_TEXT_ON_PRIMARY));
  lv_style_set_border_width(&mfd_style_btn, 1);
  lv_style_set_pad_all(&mfd_style_btn, 10);
  lv_style_set_margin_all(&mfd_style_btn, 10);
  // lv_style_set_shadow_color(&mfd_style_btn, lv_color_hex(DAY_SURFACE));
  lv_style_set_shadow_width(&mfd_style_btn, 5);
  lv_style_set_shadow_offset_x(&mfd_style_btn, 4);
  lv_style_set_shadow_offset_y(&mfd_style_btn, 6);

  lv_style_set_bg_color(&mfd_style_menubar, lv_color_hex(DAY_BACKGROUND));
  lv_style_set_bg_opa(&mfd_style_menubar, LV_OPA_50);
  //lv_style_set_text_color(&mfd_style_menubar, lv_color_hex(DAY_TEXT_ON_BACKGROUND));
  lv_style_set_text_opa(&mfd_style_menubar, LV_OPA_50);
  lv_style_set_text_font(&mfd_style_menubar, &ui_font_lv_conthrax_16);
  lv_style_set_border_color(&mfd_style_menubar, lv_color_hex(DAY_TEXT_ON_PRIMARY));
  lv_style_set_border_width(&mfd_style_menubar, 1);
  lv_style_set_text_font(&mfd_style_menubar, &ui_font_lv_conthrax_16);
  lv_style_set_text_align(&mfd_style_menubar, LV_ALIGN_CENTER);
  lv_style_set_shadow_color(&mfd_style_menubar, lv_color_hex(DAY_SURFACE));
  lv_style_set_shadow_width(&mfd_style_menubar, 5);
  lv_style_set_x(&mfd_style_menubar, 0);
  lv_style_set_y(&mfd_style_menubar, 0);
  lv_style_set_width(&mfd_style_menubar, 150);
  lv_style_set_height(&mfd_style_menubar, lv_pct(100));
  lv_style_set_radius(&mfd_style_menubar, 5);
  lv_style_set_margin_all(&mfd_style_menubar, 5);
}

/**
 * Set the style of an object to the default style.
 */
lv_obj_t *mfd_set_style(lv_obj_t *obj)
{

  lv_obj_add_style(obj, &mfd_style, 0);
  lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
  return obj;
}

/**
 * Set the style of an object to the day style.
 */
lv_obj_t *mfd_set_style_day(lv_obj_t *obj)
{

  lv_obj_add_style(obj, &mfd_style_day, 0);
  return obj;
}

/**
 * Set the style of an object to the night style.
 */
lv_obj_t *mfd_set_style_night(lv_obj_t *obj)
{

  lv_obj_add_style(obj, &mfd_style_night, 0);
  return obj;
}

/**
 * Set the style of an object to the sun style.
 */
lv_obj_t *mfd_set_style_sun(lv_obj_t *obj)
{

  lv_obj_add_style(obj, &mfd_style_sun, 0);
  return obj;
}

/**
 * Set the style of an object to the dawn style.
 */
lv_obj_t *mfd_set_obj_style_dawn(lv_obj_t *obj)
{
  lv_obj_add_style(obj, &mfd_style_dawn, 0);
  return obj;
}

/**
 * Set the style of a tile.
 */
lv_obj_t *mfd_set_tile_style(lv_obj_t *tile)
{

  lv_obj_add_style(tile, &mfd_style_tile, 0);
  lv_obj_set_scrollbar_mode(tile, LV_SCROLLBAR_MODE_OFF);
  return tile;
}

/**
 * Set the style of a button and add the checkable flag. The button will change its background color when pressed or focused
 */
lv_obj_t *mfd_set_btn_style(lv_obj_t *btn)
{

  lv_obj_add_style(btn, &mfd_style_btn, 0);//LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(btn, lv_color_hex(0x008591), LV_STATE_FOCUSED);
  lv_obj_set_scrollbar_mode(btn, LV_SCROLLBAR_MODE_OFF);
  return btn;
}

/**
 * Set the style of a button and add the checkable flag. The button will change its background color when pressed or focused
 */
lv_obj_t *mfd_set_mob_btn_style(lv_obj_t *btn)
{

  lv_obj_add_style(btn, &mfd_style_btn, LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(btn, lv_color_hex(MOB_BTN_COLOR), LV_STATE_FOCUSED);
  lv_obj_set_scrollbar_mode(btn, LV_SCROLLBAR_MODE_OFF);
  return btn;
}

lv_obj_t *mfd_set_menu_bar_style(lv_obj_t *mbar)
{

  lv_obj_add_style(mbar, &mfd_style_menubar, 0);//LV_PART_ANY);
  return mbar;
}

/**
 * Two step function to set the style of the whole tree. First set the style of the parent
 * and then call mfd_recolor to update the child objects
 */
void mfd_recolor(lv_obj_t *parent)
{
  if (UNCHANGED_STYLE != mfd_get_style_changed())
  {

    //lv_log(" recoloring tree...\n)");

    if (DAY_STYLE == mfd_get_style_changed())
      mfd_update_style(parent, &mfd_style_day);
    if (SUN_STYLE == mfd_get_style_changed())
      mfd_update_style(parent, &mfd_style_sun);
    if (DAWN_STYLE == mfd_get_style_changed())
      mfd_update_style(parent, &mfd_style_dawn);
    if (NIGHT_STYLE == mfd_get_style_changed())
      mfd_update_style(parent, &mfd_style_night);
    mfd_set_style_changed(UNCHANGED_STYLE);
  }
}