/**
 * @file mfd_themes.h
 */


#ifndef MFD_THEMES_H
#define MFD_THEMES_H

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
/*
 * Day theme colours
 */
#define UNCHANGED_STYLE 0
#define DAY_STYLE 1
#define DAY_PRIMARY 0x37474F   // #37474F
#define DAY_SECONDARY 0x263238 // #263238
#define DAY_BACKGROUND 0xc7c7c7 // #c7c7c7
#define DAY_SURFACE 0x3d3d3d // #3d3d3d
#define DAY_TEXT_ON_PRIMARY 0xffffff // #ffffff
#define DAY_TEXT_ON_BACKGROUND 0x000000// #000000
#define DAY_LINE_COLOR 0x575757// #575757

/*
 * Night theme colours
 */
#define NIGHT_STYLE 2
#define NIGHT_PRIMARY 0x252525 // #252525
#define NIGHT_SECONDARY 0x5e5e5e // #5e5e5e
#define NIGHT_BACKGROUND 0x000000 // #000000
#define NIGHT_SURFACE 0x374151 // #374151
#define NIGHT_TEXT_ON_PRIMARY 0xc83232 // #c83232
#define NIGHT_TEXT_ON_BACKGROUND 0xc83232 // #c83232
#define NIGHT_LINE_COLOR 0x373737           // #373737

/*
 * Sun theme colours
 */
#define SUN_STYLE 3
#define SUN_PRIMARY 0xffea00   // #ffea00
#define SUN_SECONDARY 0x00061e // #000723
#define SUN_BACKGROUND 0xffffff // #ffffff
#define SUN_SURFACE 0xb7b7b7    // #b7b7b7
#define SUN_TEXT_ON_PRIMARY 0xffffff // #ffffff
#define SUN_TEXT_ON_BACKGROUND 0x000000 // #000000
#define SUN_LINE_COLOR 0xa4a4a4         // #a4a4a4

/*
 * Dawn theme colours
 */
#define DAWN_STYLE 4
#define DAWN_PRIMARY 0x311B92   // #311B92
#define DAWN_SECONDARY 0x4527A0 // #4527A0
#define DAWN_BACKGROUND 0x1A237E  // #1A237E
#define DAWN_SURFACE 0x303F9F     // #303F9F
#define DAWN_TEXT_ON_PRIMARY 0xc6abf4    // #c6abf4
#define DAWN_TEXT_ON_BACKGROUND 0xea01ff // #ea01ff
#define DAWN_LINE_COLOR 0x553ca0         // #553ca0

  /**********************
   *      TYPEDEFS
   **********************/
  /*
   * There are 4 different themes to deal with 4 typical daylight conditions
   * THEME_MODE_DAY for normal daylight operation, with moderate brightness at 50%
   * THEME_MODE_SUN for operation during sunlight with maximum brightness(255)
   * THEME_MODE_DAWN for operation during the twilight zone befor nigth
   * THEME_MODE_NIGHT for night opreation, moderate brightness and Red foreground and black background
   */
  typedef enum
  {
    THEME_MODE_DAY,
    THEME_MODE_SUN,
    THEME_MODE_DAWN,
    THEME_MODE_NIGHT
  } lv_theme_mode_t;

  /***********************
   *  STATIC VARIABLES
   **********************/

  static lv_subject_t theme_subject;
  static   lv_style_t mfd_style_day, mfd_style_night, mfd_style_sun, mfd_style_dawn, mfd_style;
  static lv_style_t mfd_style_tile, mfd_style_menubar, mfd_style_btn, mfd_style_btn_pressed;
  static bool mfd_styles_inited;
  static int mfd_style_changed;

  /**********************
   * GLOBAL PROTOTYPES
   *********************/

  
  void mfd_init_styles();
  void mfd_update_style( lv_obj_t *parent, lv_style_t *newstyle);
  int mfd_get_style_changed();
  void mfd_set_style_changed(int styleID);
  void mfd_recolor(lv_obj_t *parent);

  lv_obj_t *mfd_set_style(lv_obj_t *obj);
  lv_obj_t *mfd_set_style_day(lv_obj_t *obj);
  lv_obj_t *mfd_set_style_night(lv_obj_t *obj);
  lv_obj_t *mfd_set_style_sun(lv_obj_t *obj);
  lv_obj_t *mfd_set_obj_style_dawn(lv_obj_t *obj);
  lv_obj_t *mfd_set_menu_bar_style(lv_obj_t *mbar);
  lv_obj_t *mfd_set_panel_style(lv_obj_t *panel);
  lv_obj_t *mfd_set_tile_style(lv_obj_t *tile);
  lv_obj_t *mfd_set_btn_style(lv_obj_t *btn);
  lv_obj_t *mfd_set_btn_pressed_style(lv_obj_t *btn);

  /**********************
   *      MACROS
   **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MFD_THEMES_H*/