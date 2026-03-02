/**
 * @file screen_about.c
 * 
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  Screen_about is an Easter-egg for my wife Jeannet, who is the inspiration for this project. 
  It is a simple screen with a random message from a list of 9 messages that I have collected over the years. 
  The messages are in Frysian, since Jeannet is Frysian and I am not. 
  Future use is that this screen hold some favorite gauges etc she is using while at the helm :-)
*/
/*********************
 *      INCLUDES
 *********************/

#include "ui/screen_about.h"
#include "ui/ui_screens.h"
#include <ui/mfd_btn.h>
#include <font/mfd_fonts.h>

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
#define NR_OF_MESSAGES 9
const char *sjeans_msg[] = {
    "I love Sjean",
    "Hâ goeie\n Skipper Sjean",
    "Sjean en Jeppe,\n lekker Fierljeppe",
    "He,\n Opsokkebolje",
    "Hee \nlekkerding",
    "R <3 J",
    "Griene blêdjevretters",
    "Almachtich\n Twaentachtich",
  "Dikke tût"};
/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t *screen_about_create(void)
{
  LV_TRACE_OBJ_CREATE("begin");

  static bool style_inited = false;

  if (!style_inited)
  {

    style_inited = true;
  }

  lv_obj_t *lv_obj_0 = lv_obj_create(NULL);
  lv_obj_set_name_static(lv_obj_0, "screen_about_#");
  lv_obj_set_style_bg_color(lv_obj_0, lv_color_hex(0x041d3a), 0);
  lv_obj_set_style_text_color(lv_obj_0, lv_color_hex3(0xfff), 0);
  

  lv_obj_t *lv_label_0 = lv_label_create(lv_obj_0);
  lv_label_set_text(lv_label_0, "Jeannet's private display");
  lv_obj_set_align(lv_label_0, LV_ALIGN_TOP_MID);
  lv_obj_set_y(lv_label_0, 10);

  
  lv_obj_t *lv_label_1 = lv_label_create(lv_obj_0);
  lv_obj_set_style_text_font(lv_label_1,&ui_font_lv_conthrax_72,0);
  lv_obj_set_style_text_color(lv_label_1, lv_palette_main(LV_PALETTE_RED), 0);
  int msg = lv_rand(0, NR_OF_MESSAGES-1);
  lv_label_set_text(lv_label_1, sjeans_msg[msg]);
  lv_obj_set_y(lv_label_1, -30);
  lv_obj_set_align(lv_label_1, LV_ALIGN_CENTER);
  lv_obj_set_style_text_align(lv_label_1, LV_TEXT_ALIGN_CENTER, 0);

  lv_obj_t *lv_button_0 = lv_button_create(lv_obj_0);
  lv_obj_set_align(lv_button_0, LV_ALIGN_BOTTOM_MID);
  lv_obj_set_y(lv_button_0, -10);
  lv_obj_set_size(lv_button_0, 150, 75);
  lv_obj_t *lv_label_2 = lv_label_create(lv_button_0);
  lv_label_set_text(lv_label_2, "Back");
  lv_obj_set_align(lv_label_2, LV_ALIGN_CENTER);

  
  lv_obj_add_screen_load_event(lv_button_0, LV_EVENT_CLICKED, screen_main, LV_SCREEN_LOAD_ANIM_MOVE_BOTTOM, 500, 0);

  LV_TRACE_OBJ_CREATE("finished");

  return lv_obj_0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
