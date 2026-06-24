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

lv_obj_t *mob_person=NULL;
lv_obj_t *mob_ship=NULL;

void end_btn_event_cb(lv_event_t *event)
{
  lv_event_code_t code = lv_event_get_code(event);
  // the logvalue object is the userdata
  //lv_obj_t *parent = (lv_obj_t *)lv_event_get_user_data(event);
  if (code == LV_EVENT_PRESSED )
  {
    mob_active = false;
    mob_data.cog = 0.0;
    mob_data.time = 0.0;
    mob_data.data_set = false;
    sprintf(mob_data.lat, "--º --.---'");
    sprintf(mob_data.lon, "---º --.---'");
    lv_log("MOB deactivated, lat: %s, lon: %s, cog: %.2f, time: %.2f\n", mob_data.lat, mob_data.lon, mob_data.cog, mob_data.time);
    
  }
}

/** Draws a radar ring on the MOB panel  */
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

void rotate_mob_ship(float cog, lv_obj_t *ship)
{
  // rotate the ship object to the course over ground (COG) value
  lv_image_set_rotation((lv_obj_t *)ship, cog*10);
}

/** Updates the position of the MOB person on the panel 
 * relatively to the ship position. The MOB  is represented by a small icon that 
 * is moved on the panel based on the latitude and longitude values of the MOB relatively to the  ship. 
 * The MOB person is moved on the panel based on the following formula:
 * The ship is represented by a larger icon that is fixed in the center of the panel
*/
void update_mob_position(float lat, float lon, lv_obj_t *person)
{
  // update the position of the ship object on the MOB panel based on the latitude and longitude values
  // The MOB panel is 865x480 pixels. The center of the panel is 432.5, 240. The latitude and longitude values are in degrees.
  // The latitude and longitude values are converted to pixels based on the following formula:
  // x = (lon + 180) * (865 / 360)
  // y = (90 - lat) * (480 / 180)
  int x = (lon + 180) * (865 / 360);
  int y = (90 - lat) * (480 / 180);
  lv_obj_set_x(person, x);
  lv_obj_set_y(person, y);
}

lv_obj_t *mfd_mob_panel_create(lv_obj_t *parent, const char *title)
{

  lv_obj_t *panel = lv_obj_create(parent);
  lv_obj_set_name_static(panel, title);
  lv_log("creating panel %s\n", lv_obj_get_name(panel));
  mfd_set_style(panel);
  lv_obj_set_width(panel, 865);
  lv_obj_set_height(panel, lv_pct(98));
  lv_obj_set_x(panel, 150);
  lv_obj_set_y(panel, 0);
  mfd_panel_t *paneldata;
  paneldata = (mfd_panel_t *)malloc(sizeof(mfd_panel_t));
  paneldata->draw_pos_x = TILE_START_POS_X;
  paneldata->draw_pos_y = TILE_START_POS_Y;
  paneldata->max_nr_of_tiles = 3; //there is only 1 big tile on the MOB panel
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
    draw_radar_ring(50 + i*95, panel);
  }

  //draw horizontal and vertical lines to indicate the direction of the MOB position. The lines are drawn from the center of the panel to the edge of the panel.
  lv_obj_t *hline = lv_line_create(panel);
  static lv_point_precise_t hline_points[] = {{5, 0}, {485, 0}};
  lv_line_set_points(hline, hline_points, 2);
  lv_obj_set_style_line_color(hline, lv_color_hex(MOB_BTN_COLOR), 0);
  lv_obj_set_style_line_width(hline, 2, 0);
  lv_obj_set_style_line_rounded(hline, true, 0);
  lv_obj_center(hline); 

  lv_obj_t *vline = lv_line_create(panel);
  static lv_point_precise_t vline_points[] = {{0, 0}, {0, 480}};
  lv_line_set_points(vline, vline_points, 2);
  lv_obj_set_style_line_color(vline, lv_color_hex(MOB_BTN_COLOR), 0);
  lv_obj_set_style_line_width(vline, 2, 0);
  lv_obj_set_style_line_rounded(vline, true, 0);
  lv_obj_center(vline); 

  // Add ther end button. When pressed the MOB status is set to false
  lv_obj_t *end_mob_btn = lv_button_create(panel);
  lv_obj_t *label = lv_label_create(end_mob_btn);
  lv_label_set_text(label, "END");
  lv_obj_set_align(label, LV_ALIGN_CENTER);
  lv_obj_set_width(end_mob_btn, 140);
  lv_obj_set_height(end_mob_btn, 75);
  lv_obj_set_x(end_mob_btn, 650);
  lv_obj_set_y(end_mob_btn, 475);
  lv_obj_add_event_cb(end_mob_btn, end_btn_event_cb, LV_EVENT_ALL, NULL);

  // Add the labels for CTS, DST en MOB POS
  MOB_cts_box = mfd_tile_create(panel);
  lv_obj_set_style_x(MOB_cts_box, 5, 0);
  lv_obj_set_style_y(MOB_cts_box, 5, 0);
  lv_obj_set_width(MOB_cts_box, 200);
  lv_obj_set_height(MOB_cts_box, 125);  
  mfd_tile_set_label(MOB_cts_box, "CTS");
  mfd_tile_set_unit(MOB_cts_box, "º");

  MOB_dst_box = mfd_tile_create(panel);
  lv_obj_set_style_x(MOB_dst_box, 630, 0);
  lv_obj_set_style_y(MOB_dst_box, 5, 0);
  lv_obj_set_width(MOB_dst_box, 200);
  lv_obj_set_height(MOB_dst_box, 125);  
  mfd_tile_set_label(MOB_dst_box, "DST");
  mfd_tile_set_unit(MOB_dst_box, "nm");

  MOB_pos_box = mfd_tile_create(panel);
  lv_obj_set_style_x(MOB_pos_box, 5, 0);
  lv_obj_set_style_y(MOB_pos_box, 425, 0);
  lv_obj_set_width(MOB_pos_box, 220);
  lv_obj_set_height(MOB_pos_box, 125);
  mfd_tile_set_label(MOB_pos_box, "MOB POS");

  

  //Demo dummy values for the MOB position, CTS and DST. These values will be updated by the NMEA data received from the GPS
  lv_obj_t *lbl_cts_data = mfd_tile_data_create(MOB_cts_box);
  //lv_obj_set_style_text_font(lbl_cts_data, &ui_font_lv_conthrax_48, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
  lv_label_set_text(lbl_cts_data, "235");
  lv_obj_t *lbl_dst_data = mfd_tile_data_create(MOB_dst_box);
  //lv_obj_set_style_text_font(lbl_dst_data, &ui_font_lv_conthrax_48, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
  lv_label_set_text(lbl_dst_data, "0.8");
  lv_obj_t *lbl_mob_pos_data = mfd_tile_data_create(MOB_pos_box);
  lv_obj_set_style_text_font(lbl_mob_pos_data, &ui_font_lv_conthrax_24, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
  char tmp_lbl[50] = {0};
  sprintf(tmp_lbl, " %s\n%s", mob_data.lat, mob_data.lon);
  lv_label_set_text(lbl_mob_pos_data, tmp_lbl);
  
  
  

  //put a demo images on screen
  LV_IMAGE_DECLARE(location_arrow_solid);
  mob_ship = lv_image_create(panel);
  lv_image_set_src(mob_ship, &location_arrow_solid);
  lv_obj_align(mob_ship, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_bg_color(mob_ship, lv_color_hex(DAY_BACKGROUND), 0);
  //lv_image_set_rotation((lv_obj_t *)mob_ship, 2350);
  rotate_mob_ship(235.0, mob_ship);


  LV_IMAGE_DECLARE(person_drowning_solid);
  mob_person = lv_image_create(panel);
  lv_image_set_src(mob_person, &person_drowning_solid);
  lv_obj_align(mob_person, LV_ALIGN_CENTER, -100, 100);
  lv_obj_set_style_bg_color(mob_person, lv_color_hex(DAY_BACKGROUND), 0);

  return panel;
}