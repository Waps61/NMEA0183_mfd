/**
 * @file mfd_mob_panel.cpp
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  MOB screen implementation. The MOB screen displays information about the Man Overboard event.
*/
#include <lvgl.h>

#include <ui/mfd_mob_panel.h>
#include <mfd_conf.h>
#include <ui/mfd_themes.h>
#include <font/mfd_fonts.h>
#include <ui/mfd_btn.h>

void end_btn_event_cb(lv_event_t *event)
{
  lv_event_code_t code = lv_event_get_code(event);
  // the logvalue object is the userdata
  lv_obj_t *parent = (lv_obj_t *)lv_event_get_user_data(event);
  if (code == LV_EVENT_PRESSED | LV_EVENT_CLICKED)
  {
    
  }
}

void draw_radar_ring( int radius,  lv_obj_t *parent)
{
  lv_obj_t *circle = lv_obj_create(parent);
  lv_obj_set_size(circle, radius * 2, radius * 2);
  lv_obj_set_style_radius(circle, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_border_width(circle, 2, 0);
  lv_obj_set_style_border_color(circle, lv_color_hex(MOB_BTN_COLOR), 0);
  lv_obj_set_style_bg_opa(circle, LV_OPA_TRANSP, 0);
  lv_obj_center(circle);
  
}

lv_obj_t *mfd_mob_panel_create(lv_obj_t *parent, const char *title)
{

  lv_obj_t *panel = lv_obj_create(parent);
  lv_obj_set_name_static(panel, title);
  lv_log("creating panel %s\n", lv_obj_get_name(panel));
  mfd_set_style_sun(panel);
  lv_obj_set_width(panel, 865);
  lv_obj_set_height(panel, lv_pct(98));
  lv_obj_set_x(panel, 150);
  lv_obj_set_y(panel, 0);
  mfd_panel_t *paneldata;
  paneldata = (mfd_panel_t *)malloc(sizeof(mfd_panel_t));
  paneldata->draw_pos_x = TILE_START_POS_X;
  paneldata->draw_pos_y = TILE_START_POS_Y;
  paneldata->max_nr_of_tiles = 1; //there is only 1 big tile on the MOB panel
  paneldata->tile_count = 0;
  paneldata->tile_spacing_x = TILE_SPACING_X;
  paneldata->tile_spacing_y = TILE_SPACING_Y;
  lv_obj_set_user_data(panel, paneldata);

  if (title != NULL)
  {
    lv_obj_t *label = lv_label_create(panel);
    lv_obj_set_style_align(label, LV_ALIGN_TOP_MID, 0);
    lv_label_set_text(label, title);
    lv_obj_set_align(label, LV_ALIGN_TOP_MID);
  }

  // draw 4 radar rings on the MOB panel to indicate the distance from the MOB position. The rings are drawn at 50, 100, 150 and 200 meters from the MOB position.
  for (int i = 0; i<3; i++) {
    draw_radar_ring(50 + i*75, panel);
  }

  lv_obj_t *end_mob_btn = mfd_button_create(panel, "END");
  lv_obj_set_width(end_mob_btn, 50);
  lv_obj_set_height(end_mob_btn, 50);
  lv_obj_set_x(end_mob_btn, 500);
  lv_obj_set_y(end_mob_btn, 475);
  lv_obj_add_event_cb(end_mob_btn, end_btn_event_cb, LV_EVENT_ALL, NULL);

  return panel;
}