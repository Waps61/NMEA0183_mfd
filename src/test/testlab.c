#include "test/testlab.h"
static lv_obj_t *main_screen;

static lv_button_styles_t styles;
void init_btn_style()
{
if (!mfd_styles_inited)
lv_style_init(&styles.style_main);
lv_style_init(&styles.style_pressed);

lv_style_set_max_width(&styles.style_main, lv_pct(100));
lv_style_set_max_width(&styles.style_pressed, lv_pct(100));
lv_style_set_min_width(&styles.style_main, lv_pct(100));
lv_style_set_min_width(&styles.style_pressed, lv_pct(100));
// lv_style_set_max_height(&styles.style_main,100);
// lv_style_set_max_height(&styles.style_pressed,100);
lv_style_set_min_height(&styles.style_main, 75);
lv_style_set_min_height(&styles.style_pressed, 75);
lv_style_set_pad_all(&styles.style_main, 10);
lv_style_set_pad_all(&styles.style_pressed, 10);
lv_style_set_margin_all(&styles.style_main, 10);
lv_style_set_margin_all(&styles.style_pressed, 10);
lv_style_set_shadow_color(&styles.style_main, lv_color_hex(NIGHT_SURFACE));
lv_style_set_shadow_color(&styles.style_pressed, lv_color_darken(lv_color_hex(NIGHT_SURFACE), LV_OPA_40));
lv_style_set_shadow_width(&styles.style_main, 5);
lv_style_set_shadow_width(&styles.style_pressed, 5);
}

void test_menu_btn_event_cb(lv_event_t *event)
{
  lv_obj_t *panel_btn = NULL;
  panel_btn = (lv_obj_t *)lv_event_get_user_data(event);
  const char *panel_name = lv_obj_get_name(panel_btn);
  lv_log("panel to show is %s\n", panel_name);

  // lv_log(" panels equal is: %d\n", strcmp(panel_name, "TRIP"));
  // if (strcmp(panel_name, "TRIP") == 0)
  // {
  //   lv_log("hidding panel: %s\n)", panel_name);
  //   if (lv_obj_has_flag(panel_btn, LV_OBJ_FLAG_HIDDEN))
  //     mfd_show_panel(panel_btn);
  //   lv_log("hidding panel: WIND\n");
  //   mfd_hide_panel(mfd_wind_panel);
  //   lv_log("hidding panel: COURSE\n");
  //   mfd_hide_panel(mfd_course_panel);
  //   lv_log("hidding panel: BRIGHTNESS\n");
  //   mfd_hide_panel(mfd_brightness_panel);
  //   lv_log("hidding panel: SETTING\n");
  //   mfd_hide_panel(mfd_config_panel);
  // }
  // else if (strcmp(panel_name, "WIND") == 0)
  // {
  //   if (lv_obj_has_flag(panel_btn, LV_OBJ_FLAG_HIDDEN))
  //     mfd_show_panel(panel_btn);
  //   mfd_hide_panel(mfd_trip_panel);
  //   mfd_hide_panel(mfd_course_panel);
  //   mfd_hide_panel(mfd_brightness_panel);
  //   mfd_hide_panel(mfd_config_panel);
  // }
  if (lv_obj_has_flag(panel_btn, LV_OBJ_FLAG_HIDDEN))
    mfd_show_panel(panel_btn);
  else
    mfd_hide_panel(panel_btn);
}

void testlab_init()
{
  main_screen = lv_screen_active();
  //
  test_screen = mfd_panel_create(main_screen, "test_panel");
  lv_obj_t *menu_bar = lv_obj_create(main_screen);
  //lv_obj_remove_style_all(menu_bar);
  lv_obj_set_flex_flow(menu_bar, LV_FLEX_FLOW_COLUMN);
  lv_obj_add_style(menu_bar, &styles.style_main, LV_STATE_DEFAULT);
  lv_obj_set_x(menu_bar, 5);
  lv_obj_set_y(menu_bar, 0);
  lv_obj_set_style_radius(menu_bar, 5, 0);
  lv_obj_set_style_margin_all(menu_bar, 5, 0);
  lv_obj_set_style_border_color(menu_bar, lv_color_black(), 0);
  
  // lv_obj_set_style_bg_color(menu_bar, lv_palette_main(LV_PALETTE_BLUE), LV_STATE_DEFAULT);

  lv_obj_set_size(menu_bar, 150, lv_pct(100));
  lv_obj_set_style_bg_color(menu_bar, lv_color_hex(NIGHT_BACKGROUND), LV_STATE_DEFAULT);

  lv_obj_t *trip_btn = mfd_button_create(menu_bar, "TRIP");
  lv_obj_add_flag(trip_btn, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_set_style_bg_color(trip_btn, lv_color_hex(0xFFA500), LV_STATE_FOCUSED);
  lv_obj_add_event_cb(trip_btn, test_menu_btn_event_cb, LV_EVENT_CLICKED, test_screen);

  lv_obj_t *wind_btn = mfd_button_create(menu_bar, "WIND");
  lv_obj_add_flag(wind_btn, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_add_event_cb(wind_btn, test_menu_btn_event_cb, LV_EVENT_CLICKED, test_screen);

  lv_obj_t *course_btn = mfd_button_create(menu_bar, "COURSE");
  lv_obj_add_flag(wind_btn, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_add_event_cb(course_btn, test_menu_btn_event_cb, LV_EVENT_CLICKED, test_screen);

  lv_obj_t *bright_btn = mfd_button_create(menu_bar, "BRIGHT");
  lv_obj_add_flag(wind_btn, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_add_event_cb(bright_btn, test_menu_btn_event_cb, LV_EVENT_CLICKED, test_screen);

  lv_obj_t *setting_btn = mfd_button_create(menu_bar, "SETTING");
  lv_obj_add_flag(wind_btn, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_add_event_cb(setting_btn, test_menu_btn_event_cb, LV_EVENT_CLICKED, test_screen);
}


