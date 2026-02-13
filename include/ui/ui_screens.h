/**
 * @file ui_screens.h
 */

#ifndef UI_SCREENS_H
#define UI_SCREENS_H
#include <lvgl.h>
#ifdef __cplusplus
extern "C"
{
#endif

  /*********************
   *      INCLUDES
   *********************/


  /*********************
   *      DEFINES
   *********************/

#define UNIT_SMALL 6

#define UNIT_MEDIUM 12

#define UNIT_LARGE 24
  /**********************
   *      TYPEDEFS
   **********************/

  /**********************
   * GLOBAL VARIABLES
   **********************/
  /*-------------------
   * Permanent screens
   *------------------*/

  extern lv_obj_t *screen_main;

  /**********************
   * GLOBAL PROTOTYPES
   **********************/

  /**
   * Initialize the component library
   */

  void ui_screens_init_gen(const char *asset_path);
  void ui_screens_init(const char *asset_path);

  /**********************
   *      MACROS
   **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*UI_SCREENS_H*/