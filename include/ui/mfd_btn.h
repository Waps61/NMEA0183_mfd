/**
 * @file mfd_btn.h
 */

#ifndef MFD_BTN_H
#define MFD_BTN_H

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

  /**********************
   *      TYPEDEFS
   **********************/

  /**********************
   * GLOBAL PROTOTYPES
   **********************/
  lv_obj_t *mfd_button_create(lv_obj_t *parent, const char *text);

  /**********************
   *      MACROS
   **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*MFD_BTN_H*/
