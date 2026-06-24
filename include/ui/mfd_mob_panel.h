/**
 * @file mfd_mob_panel.h
 */
#ifndef MFD_MOB_PANEL_H
#define MFD_MOB_PANEL_H

#include <lvgl.h>
#include <ui/mfd_panel_gen.h>
#include <ui/mfd_tile.h>
#include <ui/mfd_tile_data.h>
#include <mfd_conf.h>
#include <stdlib.h>
#include <stdio.h>
#include <ui/mfd_themes.h>
#include <font/mfd_fonts.h>
#include <ui/mfd_btn.h>

#ifdef __cplusplus
extern "C"
{
#endif
  extern lv_obj_t *mob_person;
  extern lv_obj_t *mob_ship;
  
  lv_obj_t *mfd_mob_panel_create(lv_obj_t *parent, const char *title);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // MFD_MOB_PANEL_H