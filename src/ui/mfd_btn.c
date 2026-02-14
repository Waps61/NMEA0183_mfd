/**
 * @file mfd_btn.c
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  Button implementation to control the typcal color schemes for Day,Sun, Dawn and Night settings.
*/
/*********************
 *      INCLUDES
 *********************/
#include <lvgl.h>
#include <ui/mfd_themes.h>
#include <ui/mfd_btn.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct
{
  lv_style_t style_main;
  lv_style_t style_pressed;
} lv_button_styles_t;

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
static void mfd_button_style_observer_cb(lv_observer_t *observer, lv_subject_t *subject)
{
  LV_UNUSED(subject);

  lv_theme_mode_t m = (lv_theme_mode_t)lv_subject_get_int(&theme_subject);
  lv_button_styles_t *styles = (lv_button_styles_t *)lv_observer_get_target(observer);
  if (m == THEME_MODE_DAY)
  {
    lv_style_set_bg_color(&styles->style_main, lv_color_hex(DAY_PRIMARY));
    lv_style_set_bg_grad_color(&styles->style_main, lv_color_hex(DAY_SECONDARY));
    lv_style_set_shadow_color(&styles->style_main, lv_color_hex(DAY_SECONDARY));
    lv_style_set_text_color(&styles->style_main, lv_color_hex(DAY_TEXT_ON_PRIMARY));

    lv_style_set_color_filter_opa(&styles->style_pressed, LV_OPA_70);
  }
  if (m == THEME_MODE_NIGHT)
  {
    lv_style_set_bg_color(&styles->style_main, lv_color_hex(NIGHT_PRIMARY));
    lv_style_set_bg_grad_color(&styles->style_main, lv_color_hex(NIGHT_SECONDARY));
    lv_style_set_shadow_color(&styles->style_main, lv_color_hex(NIGHT_SECONDARY));
    lv_style_set_text_color(&styles->style_main, lv_color_hex3(NIGHT_TEXT_ON_PRIMARY));

    lv_style_set_color_filter_opa(&styles->style_pressed, LV_OPA_30);
  }
  if (m == THEME_MODE_SUN)
  {
    lv_style_set_bg_color(&styles->style_main, lv_color_hex(SUN_PRIMARY));
    lv_style_set_bg_grad_color(&styles->style_main, lv_color_hex(SUN_SECONDARY));
    lv_style_set_shadow_color(&styles->style_main, lv_color_hex(SUN_SECONDARY));
    lv_style_set_text_color(&styles->style_main, lv_color_hex3(SUN_TEXT_ON_PRIMARY));

    lv_style_set_color_filter_opa(&styles->style_pressed, LV_OPA_70);
  }
  if (m == THEME_MODE_DAWN)
  {
    lv_style_set_bg_color(&styles->style_main, lv_color_hex(DAWN_PRIMARY));
    lv_style_set_bg_grad_color(&styles->style_main, lv_color_hex(DAWN_SECONDARY));
    lv_style_set_shadow_color(&styles->style_main, lv_color_hex(DAWN_SECONDARY));
    lv_style_set_text_color(&styles->style_main, lv_color_hex3(DAWN_TEXT_ON_PRIMARY));

    lv_style_set_color_filter_opa(&styles->style_pressed, LV_OPA_30);
  }

  lv_obj_report_style_change(&styles->style_main);
  lv_obj_report_style_change(&styles->style_pressed);
}

lv_obj_t *mfd_button_create(lv_obj_t *parent, const char *text)
{
  static lv_button_styles_t styles;
  if (!mfd_styles_inited)
    mfd_init_styles();
  lv_style_init(&styles.style_main);
  lv_style_init(&styles.style_pressed);
  lv_style_copy(&styles.style_main, &mfd_style_btn);
  //lv_style_copy(&styles.style_pressed, &mfd_style_btn_pressed);

  lv_obj_t *btn = lv_button_create(parent);
  mfd_set_btn_style(btn);

  lv_obj_t *label = lv_label_create(btn);
  lv_label_set_text(label, text);
  lv_obj_center(label);

  return btn;
}
