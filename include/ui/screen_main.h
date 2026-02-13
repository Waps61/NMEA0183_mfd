/**
 * @file screen_main.h
 */

#ifndef SCREEN_MAIN_H
#define SCREEN_MAIN_H

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
#define SCREEN_UPDATE_RATE 4
#define UPDATE_DELAY (1000/SCREEN_UPDATE_RATE)
  /**********************
   *      TYPEDEFS
   **********************/
  enum mfd_panel_id
  {
    TRIP,
    WIND,
    COURSE,
    BRIGHT,
    SETTING
  };
  /**********************
   * GLOBAL PROTOTYPES
   **********************/

  lv_obj_t *screen_main_create(void);

  void test_screen_data_updates();

  void mfd_update_tile_data();

  /**********************
   *      MACROS
   **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*SCREEN_MAIN_H*/