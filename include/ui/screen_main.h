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
#define UPDATE_DELAY 500 // Update the display every 500ms, to avoid updating the display too often and causing performance issues.
  /**********************
   *      TYPEDEFS
   **********************/
 
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