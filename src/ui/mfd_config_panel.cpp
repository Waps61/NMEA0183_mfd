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

// static void mfd_panel_event_cb(lv_event_t *e)
// {
//   lv_event_code_t code = lv_event_get_code(e);
//   lv_obj_t *parent = lv_event_get_target_obj(e);
  
//   if (code == LV_EVENT_FOCUSED)
//   {
//     lv_log("cfg panel focus event \n");
//     lv_obj_t *dd = lv_obj_get_child_by_name(parent, "mfd_baud_value");
//     if(ship_config.baudrate == 4800 )
//       lv_dropdown_set_selected(dd, 1);
//     else
//       lv_dropdown_set_selected(dd, 2);

//     dd = lv_obj_get_child_by_name(parent, "mfd_log_value");
//     lv_label_set_text_fmt(dd, "%.1f",ship_config.ship_log);
//   }
// }

void save_btn_event_cb(lv_event_t *event)
{
  lv_event_code_t code = lv_event_get_code(event);
  lv_obj_t *parent = (lv_obj_t *)lv_event_get_user_data(event);
  lv_log("SAve button pressed with code=%d\n with parent %s\n", code,lv_obj_get_name(parent));
  if (code == LV_EVENT_PRESSED | LV_EVENT_CLICKED)
  {
      lv_obj_t *obj = lv_obj_find_by_name(parent, "mfd_baud_value)");
    if(obj!=NULL)
    {
      lv_log("mfd_baud_value foud with selected index = %d\n", lv_dropdown_get_selected(obj));
     if (lv_dropdown_get_selected(obj) == 0)
          ship_config.baudrate = 4800;
      else ship_config.baudrate = 38400;
    }
    obj = lv_obj_find_by_name(parent, "mfd_wifi_value)");
    if (obj != NULL)
    {
      lv_log("mfd_wifi_value foud with selected state checked = %d\n", lv_obj_has_state(obj, LV_STATE_CHECKED));
      ship_config.wifi_on = lv_obj_has_state(obj, LV_STATE_CHECKED);
    }
    
    ship_config.SSID = "not available";
    ship_config.pwd = "n*t @vailable";
    obj = lv_obj_find_by_name(parent, "mfd_log_value)");
    if (obj != NULL)
    {
      lv_log("mfd_log_value found with value = %s\n", lv_textarea_get_text(obj));
      ship_config.ship_log = atof(lv_textarea_get_text(obj));
      
    }
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
void mfd_config_panel_create(lv_obj_t *parent)
{
  lv_log("creating panel %s\n",lv_obj_get_name(parent));

    lv_obj_t *baudlabel = lv_label_create(parent);
    lv_obj_set_pos(baudlabel, 30, 36);
    lv_obj_set_size(baudlabel, 250, 25);
    lv_obj_set_style_text_font(baudlabel, &ui_font_lv_conthrax_24, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    lv_obj_set_style_text_color(baudlabel, lv_color_hex(0xff515050), (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    // lv_obj_set_style_bg_color(baudlabel, lv_color_hex(0xffd8d8d8), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(baudlabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(baudlabel, 10, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    lv_obj_set_style_pad_right(baudlabel, 10, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    lv_label_set_text(baudlabel, "Baudrate");
  
  
    lv_obj_t *baudvalue = lv_dropdown_create(parent);
    lv_obj_set_name(baudvalue, "mfd_baud_value");
    lv_obj_set_pos(baudvalue, 300, 29);
    lv_obj_set_size(baudvalue, 200, 43);
    lv_dropdown_set_options(baudvalue, "4800\n38400");
    if( ship_config.baudrate == 4800)
    lv_dropdown_set_selected(baudvalue, 0);
    else
      lv_dropdown_set_selected(baudvalue, 1);
    lv_dropdown_set_symbol(baudvalue, LV_SYMBOL_DOWN);
    lv_obj_set_style_text_font(baudvalue, &ui_font_lv_awsome_26, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    //lv_dropdown_bind_value(baudvalue, &mfd_baudrate);

    lv_obj_t *wifilabel = lv_label_create(parent);
    lv_obj_set_pos(wifilabel, 100, 103);
    lv_obj_set_size(wifilabel, 130, 25);
    lv_obj_set_style_text_font(wifilabel, &ui_font_lv_conthrax_24, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    lv_obj_set_style_text_color(wifilabel, lv_color_hex(0xff515050), (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    // lv_obj_set_style_bg_color(wifilabel, lv_color_hex(0xffd8d8d8), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(wifilabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(wifilabel, 10, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    lv_obj_set_style_pad_right(wifilabel, 10, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    lv_label_set_text(wifilabel, "WiFi");
  
  
    lv_obj_t *wifivalue = lv_switch_create(parent);
    lv_obj_set_name(wifivalue, "mfd_wifi_value");
    lv_obj_set_pos(wifivalue, 300, 94);
    lv_obj_set_size(wifivalue, 50, 34);
    if(ship_config.wifi_on)
      lv_obj_set_state(wifivalue, LV_STATE_CHECKED,true);
      else
        lv_obj_set_state(wifivalue, LV_STATE_CHECKED, false);

    lv_obj_t *loglabel = lv_label_create(parent);
    lv_obj_set_pos(loglabel, 0, 188);
    lv_obj_set_size(loglabel, 306, 35);
    lv_obj_set_style_text_font(loglabel, &ui_font_lv_conthrax_24, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    lv_obj_set_style_text_color(loglabel, lv_color_hex(0xff515050), (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    // lv_obj_set_style_bg_color(loglabel, lv_color_hex(0xffd8d8d8), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(loglabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(loglabel, 10, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    lv_obj_set_style_pad_right(loglabel, 10, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    sprintf(tmpVal, "%.1f", ship_config.ship_log);
    lv_label_set_text(loglabel, "Log-ofsset");

    // cfgKeyboard
    lv_obj_t *kbd = lv_keyboard_create(parent);
    lv_obj_set_pos(kbd, 0, 300);
    lv_obj_set_size(kbd, 795, 216);
    //lv_keyboard_set_mode(kbd, LV_KEYBOARD_MODE_NUMBER);
    lv_keyboard_set_map(kbd, LV_KEYBOARD_MODE_USER_1, kb_map, kb_ctrl);
    lv_keyboard_set_mode(kbd, LV_KEYBOARD_MODE_USER_1);
    lv_obj_set_style_text_font(kbd, &ui_font_lv_awsome_26, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_align(kbd, LV_ALIGN_DEFAULT,(lv_style_selector_t)(LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT));
    //lv_obj_set_style_text_font(kbd, &ui_font_lv_awsome_26, LV_PART_ITEMS | LV_STATE_DISABLED);
    
    // logTextbox
    lv_obj_t *logvalue = lv_textarea_create(parent);
    lv_obj_set_name(logvalue, "mfd_log_value");
    lv_obj_set_pos(logvalue, 300, 179);
    lv_obj_set_size(logvalue, 150, 70);
    lv_textarea_set_accepted_chars(logvalue, "1234567890.");
    lv_textarea_set_max_length(logvalue, 6);
    

    lv_obj_add_flag(logvalue, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_state(logvalue, (lv_state_t)(LV_STATE_FOCUSED | LV_STATE_CHECKED));
    lv_obj_set_style_text_font(logvalue, &ui_font_lv_conthrax_24, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DISABLED));
    lv_obj_set_style_text_color(logvalue, lv_color_hex(0xffcc229c), (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DISABLED));
    lv_obj_set_style_text_opa(logvalue, 255, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DISABLED));
    lv_textarea_set_placeholder_text(logvalue, "123456");
    lv_textarea_set_one_line(logvalue, true);
    lv_textarea_set_password_mode(logvalue, false);
    lv_obj_add_event_cb(logvalue, ta_event_cb, LV_EVENT_ALL, kbd);
    lv_log("about to set log t %.1f\n", ship_config.ship_log);
    sprintf(tmpVal, "%.1f", ship_config.ship_log);
    lv_textarea_add_text(logvalue, tmpVal);
    lv_keyboard_set_textarea(kbd, logvalue);

    // SaveBtn
    lv_obj_t *savebtn = lv_button_create(parent);
    lv_obj_set_pos(savebtn, 625, 108);
    lv_obj_set_size(savebtn, 150, 75);
    lv_obj_set_style_text_font(savebtn, &ui_font_lv_conthrax_24, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    lv_obj_set_style_bg_color(savebtn, lv_color_hex(0xff2bc33b), (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    lv_obj_set_style_shadow_width(savebtn, 10, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    lv_obj_set_style_shadow_offset_y(savebtn, 5, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));

    lv_obj_t *btnlbl = lv_label_create(savebtn);
    //lv_obj_set_pos(btnlbl, 0, 0);
    //lv_obj_set_size(btnlbl, 127, 45);
    lv_obj_set_style_align(btnlbl, LV_ALIGN_CENTER, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    //lv_obj_set_style_text_font(btnlbl, &ui_font_lv_conthrax_24, (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT));
    lv_label_set_text(btnlbl, "Save");
    //lv_obj_set_style_text_align(btnlbl, LV_TEXT_ALIGN_CENTER, 0);

    lv_obj_add_event_cb(savebtn, save_btn_event_cb, LV_EVENT_CLICKED, parent);

    //lv_obj_add_event_cb(parent, mfd_panel_event_cb, LV_EVENT_ALL, NULL);
}
