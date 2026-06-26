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
#include <string.h>
#include <ui/mfd_themes.h>
#include <font/mfd_fonts.h>
#include <ui/mfd_btn.h>
#include <math.h>

#define RADAR_WIDTH 500 //500 pixels is diameter
#define RADAR_SCALE 1.0 // radius equals to 1.0 nm

#ifdef __cplusplus
extern "C"
{
#endif
  extern lv_obj_t *mob_person;
  extern lv_obj_t *mob_ship;

  lv_obj_t *mfd_mob_panel_create(lv_obj_t *parent, const char *title);
  lv_obj_t *mfd_mob_panel_add_tile(lv_obj_t *panel, char const *title, char const *unit, lv_obj_t *tile);
  void update_radar_position(MobResult act_mob);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // MFD_MOB_PANEL_H