/**
 * @file mfd_mob_panel.h
 */
#ifndef MFD_MOB_PANEL_H
#define MFD_MOB_PANEL_H

#include <lvgl.h>
#include <ui/mfd_panel_gen.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

  lv_obj_t *mfd_mob_panel_create(lv_obj_t *parent, const char *title);
  
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // MFD_MOB_PANEL_H