/**
 * @file mfd_panel_gen.c
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  Generic panel implementation. The generic panel has helper function to create the other panels
*/
/*********************
 *      INCLUDES
 *********************/
#include <ui/mfd_panel_gen.h>

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
 *   GLOBAL FUNCTIONS
 **********************/


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
