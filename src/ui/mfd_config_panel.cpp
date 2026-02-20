/**
 * @file mfd_config_panel.c
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  Config (settings) screen implementation. The settings screen is used to modify program settings
  like baudrate, WiFi switch, adres* and password* (* to be done) and the total mileage log
*/
#include <ui/mfd_config_panel.h>
#include <mfd_conf.h>
#include <ui/mfd_themes.h>

char tmpVal[30] = {0};
mfd_pers_t new_config;
static lv_style_t style_radio,style_radio_chk;
static int32_t radio_index = 0;

static void radio_event_handler(lv_event_t *e)
{
  int32_t *active_id = (int32_t *)lv_event_get_user_data(e);
  lv_obj_t *cont = (lv_obj_t *)lv_event_get_current_target(e);
  lv_obj_t *act_cb = lv_event_get_target_obj(e);
  lv_obj_t *old_cb = lv_obj_get_child(cont, *active_id);

  /*Do nothing if the container was clicked*/
  if (act_cb == cont)
    return;

  lv_obj_remove_state(old_cb, LV_STATE_CHECKED); /*Uncheck the previous radio button*/
  lv_obj_add_state(act_cb, LV_STATE_CHECKED);    /*Check the current radio button*/

  *active_id = lv_obj_get_index(act_cb);
  if(radio_index == 1)
    ship_config.baudrate = 38400;
    else
      ship_config.baudrate = 4800;
    lv_subject_set_int(&mfd_subject_baudrate, (int32_t)ship_config.baudrate);
    lv_log("Selected radio buttons: %d with value in subject = %d\n", (int)radio_index),lv_subject_get_int(&mfd_subject_baudrate);
}


void save_btn_event_cb(lv_event_t *event)
{
  lv_event_code_t code = lv_event_get_code(event);
  //the logvalue object is the userdata
  lv_obj_t *parent = (lv_obj_t *)lv_event_get_user_data(event);
  lv_log("SAve button pressed with code=%d\n with parent %s\n", code,lv_obj_get_name(parent));
  if (code == LV_EVENT_PRESSED | LV_EVENT_CLICKED)
  {
    mfd_ship_config_set_baudrate(lv_subject_get_int(&mfd_subject_baudrate));
    mfd_ship_config_set_wifi(lv_subject_get_int(&mfd_subject_wifi));
  mfd_ship_config_set_ssid("to be implemented");
    mfd_ship_config_set_pwd("n*t yet implemented");

    mfd_ship_config_set_log(atof(lv_textarea_get_text(parent)));
    set_boat_log(mfd_ship_config_get_log());
    mfd_write_persistent_data(&ship_config);
  }
}

static void ta_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = lv_event_get_target_obj(e);
  lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
  if (code == LV_EVENT_FOCUSED)
  {
    lv_keyboard_set_textarea(kb, ta);
    lv_obj_remove_flag(kb, LV_OBJ_FLAG_HIDDEN);
  }

  if (code == LV_EVENT_DEFOCUSED)
  {
    lv_keyboard_set_textarea(kb, NULL);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
  }
}

// create the config screen
lv_obj_t* mfd_config_panel_create(lv_obj_t *parent, const char * title )
{
  
  lv_obj_t *panel = lv_obj_create(parent);
  lv_obj_set_name(panel, title);
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
  paneldata->max_nr_of_tiles = MAX_NR_OF_TILES;
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

  lv_obj_t *baudlabel = lv_label_create(panel);
  lv_obj_set_pos(baudlabel, 30, 36);
  lv_obj_set_size(baudlabel, 250, 25);
  lv_obj_set_style_text_font(baudlabel, &ui_font_lv_conthrax_24, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
  lv_obj_set_style_text_color(baudlabel, lv_color_hex(0xff515050), (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
  // lv_obj_set_style_bg_color(baudlabel, lv_color_hex(0xffd8d8d8), LV_PART_MAIN | LV_STATE_DEFAULT);
  // lv_obj_set_style_bg_opa(baudlabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(baudlabel, 10, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
  lv_obj_set_style_pad_right(baudlabel, 10, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
  lv_label_set_text(baudlabel, "Baudrate");

  lv_style_init(&style_radio);
  lv_style_set_radius(&style_radio, LV_RADIUS_CIRCLE);
  lv_style_init(&style_radio_chk);
  lv_style_set_bg_image_src(&style_radio_chk, NULL);

  lv_obj_t *baudvalue = lv_obj_create(panel);
  lv_obj_set_x(baudvalue, 300);
  lv_obj_set_y(baudvalue, 29);
  lv_obj_set_width(baudvalue, 300);
  lv_obj_set_height(baudvalue, 43);
  lv_obj_add_event_cb(baudvalue, radio_event_handler, LV_EVENT_CLICKED, &radio_index);
  lv_subject_set_int(&mfd_subject_baudrate, ship_config.baudrate);
  lv_obj_set_style_border_width(baudvalue, 1, 0);
  lv_obj_set_scrollbar_mode(baudvalue, LV_SCROLLBAR_MODE_OFF);

  lv_obj_t *radio_btn1 = lv_checkbox_create(baudvalue);
  lv_checkbox_set_text(radio_btn1, "4800");
  lv_obj_add_flag(radio_btn1, LV_OBJ_FLAG_EVENT_BUBBLE);
  lv_obj_add_style(radio_btn1, &style_radio, LV_PART_INDICATOR);
  lv_obj_add_style(radio_btn1, &style_radio_chk, LV_PART_INDICATOR | LV_STATE_CHECKED);
  lv_obj_set_style_align(radio_btn1, LV_ALIGN_LEFT_MID, 0);

  lv_obj_t *radio_btn2 = lv_checkbox_create(baudvalue);
  lv_checkbox_set_text(radio_btn2, "38400");
  lv_obj_add_flag(radio_btn2, LV_OBJ_FLAG_EVENT_BUBBLE);
  lv_obj_add_style(radio_btn2, &style_radio, LV_PART_INDICATOR);
  lv_obj_add_style(radio_btn2, &style_radio_chk, LV_PART_INDICATOR | LV_STATE_CHECKED);
  lv_obj_set_style_align(radio_btn2, LV_ALIGN_RIGHT_MID, 0);
  if (ship_config.baudrate != 38400)
    lv_obj_add_state(lv_obj_get_child(baudvalue, 0), LV_STATE_CHECKED);
    else
      lv_obj_add_state(lv_obj_get_child(baudvalue, 1), LV_STATE_CHECKED);

    lv_obj_t *wifilabel = lv_label_create(panel);
    lv_obj_set_pos(wifilabel, 100, 103);
    lv_obj_set_size(wifilabel, 130, 25);
    lv_obj_set_style_text_font(wifilabel, &ui_font_lv_conthrax_24, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    lv_obj_set_style_text_color(wifilabel, lv_color_hex(0xff515050), (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    // lv_obj_set_style_bg_color(wifilabel, lv_color_hex(0xffd8d8d8), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(wifilabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(wifilabel, 10, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    lv_obj_set_style_pad_right(wifilabel, 10, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    lv_label_set_text(wifilabel, "WiFi");

    lv_obj_t *wifivalue = lv_switch_create(panel);
    //lv_subject_init_int(&mfd_subject_wifi, ship_config.wifi_on);
    lv_subject_set_int(&mfd_subject_wifi, (int32_t)ship_config.wifi_on);
    lv_obj_set_name(wifivalue, "mfd_wifi_value");
    lv_obj_set_pos(wifivalue, 300, 94);
    lv_obj_set_size(wifivalue, 50, 34);
    lv_obj_set_style_border_width(wifivalue, 1, 0);
    if(ship_config.wifi_on)
      lv_obj_set_state(wifivalue, LV_STATE_CHECKED,true);
      else
        lv_obj_set_state(wifivalue, LV_STATE_CHECKED, false);
      lv_obj_bind_checked(wifivalue,&mfd_subject_wifi );

      lv_obj_t *loglabel = lv_label_create(panel);
      lv_obj_set_pos(loglabel, 0, 188);
      lv_obj_set_size(loglabel, 306, 35);
      lv_obj_set_style_text_font(loglabel, &ui_font_lv_conthrax_24, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
      lv_obj_set_style_text_color(loglabel, lv_color_hex(0xff515050), (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
      // lv_obj_set_style_bg_color(loglabel, lv_color_hex(0xffd8d8d8), LV_PART_MAIN | LV_STATE_DEFAULT);
      // lv_obj_set_style_bg_opa(loglabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_pad_left(loglabel, 10, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
      lv_obj_set_style_pad_right(loglabel, 10, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
      sprintf(tmpVal, "%.1f", mfd_ship_config_get_log());
      lv_label_set_text(loglabel, "Log-ofsset");
      

      // cfgKeyboard
      lv_obj_t *kbd = lv_keyboard_create(panel);
      lv_obj_set_pos(kbd, 0, 300);
      lv_obj_set_size(kbd, 795, 216);
      // lv_keyboard_set_mode(kbd, LV_KEYBOARD_MODE_NUMBER);
      lv_keyboard_set_map(kbd, LV_KEYBOARD_MODE_USER_1, kb_map, kb_ctrl);
      lv_keyboard_set_mode(kbd, LV_KEYBOARD_MODE_USER_1);
      lv_obj_set_style_text_font(kbd, &ui_font_lv_awsome_26, LV_PART_MAIN | LV_STATE_DISABLED);
      lv_obj_set_style_align(kbd, LV_ALIGN_DEFAULT, (lv_style_selector_t)(LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT));
      // lv_obj_set_style_text_font(kbd, &ui_font_lv_awsome_26, LV_PART_ITEMS | LV_STATE_DISABLED);

      // logTextbox
      lv_obj_t *logvalue = lv_textarea_create(panel);
      //lv_subject_init_float(&mfd_subject_log, ship_config.ship_log);
      lv_subject_set_float(&mfd_subject_log, (float)ship_config.ship_log);
      lv_obj_set_name(logvalue, "mfd_log_value");
      lv_obj_set_pos(logvalue, 300, 179);
      lv_obj_set_size(logvalue, 150, 70);
      lv_textarea_set_accepted_chars(logvalue, "1234567890.");
      lv_textarea_set_max_length(logvalue, 7);
      lv_label_bind_text(logvalue, &mfd_subject_log,NULL);

      lv_obj_add_flag(logvalue, LV_OBJ_FLAG_CHECKABLE);
      lv_obj_add_state(logvalue, (lv_state_t)(LV_STATE_FOCUSED | LV_STATE_CHECKED));
      lv_obj_set_style_text_font(logvalue, &ui_font_lv_conthrax_24, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DISABLED));
      lv_obj_set_style_text_color(logvalue, lv_color_hex(0xffcc229c), (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DISABLED));
      lv_obj_set_style_text_opa(logvalue, 255, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DISABLED));
      lv_textarea_set_placeholder_text(logvalue, "123456");
      lv_textarea_set_one_line(logvalue, true);
      lv_textarea_set_password_mode(logvalue, false);
      lv_obj_add_event_cb(logvalue, ta_event_cb, LV_EVENT_ALL, kbd);
      lv_log("about to set log t %.1f\n", mfd_ship_config_get_log());
      sprintf(tmpVal, "%.1f", mfd_ship_config_get_log());
      lv_textarea_add_text(logvalue, tmpVal);
      lv_keyboard_set_textarea(kbd, logvalue);

      // SaveBtn
      lv_obj_t *savebtn = lv_button_create(panel);
      lv_obj_set_pos(savebtn, 625, 108);
      lv_obj_set_size(savebtn, 150, 75);
      lv_obj_set_style_text_font(savebtn, &ui_font_lv_conthrax_24, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
      lv_obj_set_style_bg_color(savebtn, lv_color_hex(0xff2bc33b), (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
      lv_obj_set_style_shadow_width(savebtn, 10, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
      lv_obj_set_style_shadow_offset_y(savebtn, 5, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
      lv_obj_set_style_border_width(savebtn, 1, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));

      lv_obj_t *btnlbl = lv_label_create(savebtn);
      // lv_obj_set_pos(btnlbl, 0, 0);
      // lv_obj_set_size(btnlbl, 127, 45);
      lv_obj_set_style_align(btnlbl, LV_ALIGN_CENTER, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
      // lv_obj_set_style_text_font(btnlbl, &ui_font_lv_conthrax_24, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
      lv_label_set_text(btnlbl, "Save");
      // lv_obj_set_style_text_align(btnlbl, LV_TEXT_ALIGN_CENTER, 0);

      lv_obj_add_event_cb(savebtn, save_btn_event_cb, LV_EVENT_CLICKED, logvalue);

      lv_obj_t *version = lv_label_create(panel);
      lv_obj_set_style_text_font(version, &ui_font_lv_conthrax_16, 0);
      lv_obj_set_style_align(version, LV_ALIGN_BOTTOM_LEFT, 0);
      lv_obj_set_style_opa(version,LV_OPA_40, 0);
      lv_obj_set_style_text_color(version, lv_color_hex(DAY_BACKGROUND), 0);
      lv_label_set_text(version, MFD_VERSION);

      // lv_obj_add_event_cb(parent, mfd_panel_event_cb, LV_EVENT_ALL, NULL);
      return panel;
}
