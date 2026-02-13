/**
 * @file ui_screens.c
 */

/*********************
 *      INCLUDES
 *********************/

#include <ui/ui_screens.h>
#include <ui/screen_main.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
/*--------------------
 *  Permanent screens
 *-------------------*/

lv_obj_t *screen_main = NULL;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void ui_screens_init_gen(const char *asset_path)
{
  char buf[256];
  /*--------------------
   *  Permanent screens
   *-------------------*/
  /* To allow screens to reference each other, create them all before calling the sceen create functions */
  screen_main = lv_obj_create(NULL);
  screen_main_create();
}

void ui_screens_init(const char *asset_path)
{
  ui_screens_init_gen(asset_path);

  /* Add your own custom code here if needed */
}


/**********************
 *   STATIC FUNCTIONS
 **********************/