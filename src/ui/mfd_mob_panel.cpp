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

lv_obj_t *mob_person = NULL;
lv_obj_t *mob_ship = NULL;

void end_btn_event_cb(lv_event_t *event)
{
  lv_event_code_t code = lv_event_get_code(event);
  // the logvalue object is the userdata
  // lv_obj_t *parent = (lv_obj_t *)lv_event_get_user_data(event);
  if (code == LV_EVENT_PRESSED)
  {
    mob_active = false;
    mob_data->cog = 0.0;
    mob_data->time = 0.0;
    mob_data->mob_set = false;
    sprintf(mob_data->lat, "--º --.---'");
    sprintf(mob_data->lon, "---º --.---'");
  }
}

/** Draws a radar ring on the MOB panel  */
void draw_radar_ring(int radius, lv_obj_t *parent)
{
  lv_obj_t *circle = lv_obj_create(parent);
  lv_obj_set_size(circle, radius * 2, radius * 2);
  lv_obj_set_style_radius(circle, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_border_width(circle, 2, 0);
  lv_obj_set_style_border_color(circle, lv_color_hex(MOB_BTN_COLOR), 0);
  lv_obj_set_style_bg_opa(circle, LV_OPA_TRANSP, 0);
  lv_obj_center(circle);

  // draw horizontal and vertical lines to indicate the direction of the MOB position. The lines are drawn from the center of the panel to the edge of the panel.
  lv_obj_t *hline = lv_obj_create(parent);
  lv_obj_set_size(hline, RADAR_WIDTH, 2);
  lv_obj_set_style_border_width(hline, 2, 0);
  lv_obj_set_style_border_color(hline, lv_color_hex(MOB_BTN_COLOR), 0);
  lv_obj_set_style_bg_opa(hline, LV_OPA_TRANSP, 0);
  lv_obj_center(hline);

  lv_obj_t *vline = lv_obj_create(parent);
  lv_obj_set_size(vline, 2, RADAR_WIDTH);
  lv_obj_set_style_border_width(vline, 2, 0);
  lv_obj_set_style_border_color(vline, lv_color_hex(MOB_BTN_COLOR), 0);
  lv_obj_set_style_bg_opa(vline, LV_OPA_TRANSP, 0);
  lv_obj_center(vline);

  lv_obj_t *radar_scale = lv_label_create(parent);
  lv_label_set_text_fmt(radar_scale,"scale = %.1f nm",RADAR_SCALE);
  lv_obj_set_style_text_font(radar_scale, &ui_font_lv_conthrax_16, 0);
  lv_obj_set_style_align(radar_scale, LV_ALIGN_RIGHT_MID, 0);
}

void rotate_mob_ship(int cog, lv_obj_t *ship)
{
  // rotate the ship object to the course over ground (COG) value
  lv_image_set_rotation((lv_obj_t *)ship, cog * 10);
}

void update_radar_position(MobResult act_mob)
{
  float radar_scale = RADAR_WIDTH / (RADAR_SCALE * 2);
  float r = act_mob.distanceNM * radar_scale;
  if (r>radar_scale)
    r = radar_scale;
  float radar_x = r * cos((90 - act_mob.courseToSteerDeg) * M_PI / 180);
  float radar_y = r * sin((90 - act_mob.courseToSteerDeg) * M_PI / 180);
  // if( act_mob.courseToSteerDeg > 180)
  //   radar_x *= -1; // 180-360 degrees is on the negative side of the x-axis

  lv_image_set_rotation(mob_ship, act_mob.courseToSteerDeg * 10);
  // Correct for the alignment direction in the y-direction, where
  // y-direction the opposite in the XY-axis
  lv_obj_align(mob_person, LV_ALIGN_CENTER, (int)radar_x, (int)-radar_y);
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

lv_obj_t *mfd_mob_panel_add_tile(lv_obj_t *panel, char const *title, char const *unit, lv_obj_t *tile)
{
  mfd_panel_t *pdata = (mfd_panel_t *)lv_obj_get_user_data(panel);
  if (pdata->tile_count < pdata->max_nr_of_tiles)
  {
    tile = mfd_tile_create(panel);
    lv_obj_set_name_static(tile, "mfd_mob_tile");
    mfd_set_tile_style(tile);
    mfd_set_style_day(tile);
    lv_obj_set_style_x(tile, pdata->draw_pos_x, 0);
    lv_obj_set_style_y(tile, pdata->draw_pos_y, 0);
    pdata->tile_count++;
    pdata->draw_pos_x += pdata->tile_width + pdata->tile_spacing_x;
    if (pdata->draw_pos_x > (MFD_SCREEN_WIDTH - pdata->tile_width))
    {
      pdata->draw_pos_x = 5;
      pdata->draw_pos_y += pdata->tile_height + pdata->tile_spacing_y;
    }
    mfd_tile_set_label(tile, title);
    mfd_tile_set_unit(tile, unit);
  }
  return tile;
}

lv_obj_t *mfd_mob_panel_create(lv_obj_t *parent, const char *title)
{
  static bool inited = false;
  static lv_panel_styles_t styles;
  if (!inited)
  {
    inited = true;

    lv_style_init(&styles.style_main);

    lv_style_init(&styles.style_scrollbar);
  }

  lv_obj_t *panel = lv_obj_create(parent);
  lv_obj_set_name_static(panel, title);
  mfd_set_style(panel);
  mfd_set_style_sun(panel);
  lv_obj_set_width(panel, 865);
  lv_obj_set_height(panel, lv_pct(98));
  lv_obj_set_x(panel, 150);
  lv_obj_set_y(panel, 0);

  mfd_panel_t *mobpaneldata;
  mobpaneldata = (mfd_panel_t *)malloc(sizeof(mfd_panel_t));
  mobpaneldata->draw_pos_x = 2;
  mobpaneldata->draw_pos_y = 0;
  mobpaneldata->max_nr_of_tiles = 3;
  mobpaneldata->tile_count = 0;
  mobpaneldata->tile_spacing_x = 395;
  mobpaneldata->tile_spacing_y = 305;
  mobpaneldata->tile_width = 220;
  mobpaneldata->tile_height = 125;
  if (strcmp(title, "MOB POS") == 0)
    mobpaneldata->font_size = 24;
  else
    mobpaneldata->font_size = 48;
  lv_obj_set_user_data(panel, mobpaneldata);

  if (title != NULL)
  {
    lv_obj_t *label = lv_label_create(panel);
    lv_obj_set_style_align(label, LV_ALIGN_TOP_MID, 0);
    lv_label_set_text(label, title);
    lv_obj_set_align(label, LV_ALIGN_TOP_MID);
  }

  // draw 4 radar rings on the MOB panel to indicate the distance from the MOB position. The rings are drawn at 50, 100, 150 and 200 meters from the MOB position.
  for (int i = 0; i < 6; i++)
  {
    draw_radar_ring(i * 50, panel);
  }

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

  // put a demo images on screen
  LV_IMAGE_DECLARE(location_arrow_solid);
  mob_ship = lv_image_create(panel);
  lv_image_set_src(mob_ship, &location_arrow_solid);
  
  lv_image_set_scale(mob_ship, 512);
  lv_obj_align(mob_ship, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_bg_color(mob_ship, lv_color_hex(DAY_BACKGROUND), 0);
  //lv_obj_set_style_bg_image_recolor(mob_ship, lv_color_hex( MOB_BTN_COLOR), 0);
  // lv_image_set_rotation((lv_obj_t *)mob_ship, 2350);
  rotate_mob_ship(235.0, mob_ship);

  LV_IMAGE_DECLARE(person_drowning_solid);
  mob_person = lv_image_create(panel);
  lv_image_set_src(mob_person, &person_drowning_solid);
  lv_image_set_scale(mob_person, 512);
  // lv_obj_align(mob_person, LV_ALIGN_CENTER, -100, 100);
  lv_obj_set_style_bg_color(mob_person, lv_color_hex(DAY_BACKGROUND), 0);

  return panel;
}