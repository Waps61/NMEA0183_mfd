/**
 * @file screen_main_gen.c
 *
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  Main screen implementation. The main screen is a static screen. On top al other panels are stacked.
  The main screen initialises and controls the menubar and the 5 panels
  Trip, Wind and Course panels are used to display max 6 tiles per panel
  Brightness and Settings apnel have their own helper functions defined
*/
/*********************
 *      INCLUDES
 *********************/
#include <Arduino.h>

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <lvgl.h>
#include <calc/mfd_calculation.h>
#include <mfd_conf.h>
#include <ui/screen_main.h>
#include <ui/screen_about.h>
#include <ui/ui_screens.h>
#include <ui/mfd_themes.h>
#include <ui/mfd_panel.h>
#include <ui/mfd_btn.h>
#include <ui/mfd_tile.h>
#include <ui/mfd_tile_data.h>
#include <ui/mfd_mini_tile.h>
#include <NMEA0183_data.h>
#include <ui/mfd_bright_panel.h>
#include <ui/mfd_config_panel.h>
#include <esp_heap_caps.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t *mfd_menubar = NULL;          // a static menu bar that is used to selected a mfd panel
static lv_obj_t *mfd_trip_panel = NULL;       // a static panel to display trip data
static lv_obj_t *mfd_wind_panel = NULL;       // a static panel to display wind data
static lv_obj_t *mfd_course_panel = NULL;     // a static panel to display course data
static lv_obj_t *mfd_brightness_panel = NULL; // a static panel for brigtness settings
static lv_obj_t *mfd_config_panel = NULL;     // a static panel for the config setting
static lv_obj_t *mfd_panel_array[5] = {0};
static int TRIP_PNL = 0;
static int WIND_PNL = 1;
static int COURSE_PNL = 2;
static int BRIGHT_PNL = 3;
static int CONFIG_PNL = 4;
static int *panel_hash[] = {&TRIP_PNL, &WIND_PNL, &COURSE_PNL, &BRIGHT_PNL, &CONFIG_PNL};

static char tile_data_buffer[15];

unsigned long previous_millis = 0;
unsigned long current_millis = 0;
int mileage = 0;

size_t free_bytes;
/***********************
 *  STATIC PROTOTYPES
 **********************/
lv_obj_t *tile_hash[NR_OF_NMEA_TAGS] = {0};
lv_obj_t *sogplot, *dptplot;
lv_chart_series_t *ser_sog, *ser_dpt;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
// implement function declared in  NMEA0183.h

void set_boat_log(float value)
{
  boat_log = value;
  lv_subject_set_float(&mfd_subject_log, value);
}
float get_boat_log()
{
  return boat_log;
}
float increase_boat_log(float value)
{
  boat_log += value;
  lv_subject_set_float(&mfd_subject_log, boat_log);
  return boat_log;
}

void init_data_store()
{

  for (int i = 0; i < NR_OF_NMEA_TAGS; i++)
  {
    strcpy(NMEA_DATA_STORE[i], "---");
    // lv_log("NMEA_DATA_STORE[ %d] = %s\n", i, NMEA_DATA_STORE[i]);
  }
  data_store_inited = true;
}

void set_data_store(enum sequence_id tag, const char data[15])
{
  char fmt_data[15] = {0};
  int j = 0;
  for (int i = 0; (i < strlen(data) || data[i] == '.'); i++)
  {
    if (data[i] != '0' || j > 0)
      fmt_data[j++] = data[i];
  }
  switch (tag)
  {

  case SOG:
    boat_sog = atof(fmt_data);
    sprintf(NMEA_DATA_STORE[tag], "%2.4s", fmt_data);
    break;
  case DPT:
    boat_dpt = atof(fmt_data) * -1.0;
    sprintf(NMEA_DATA_STORE[tag], "%2.4s", fmt_data);
    break;
  case LOG:
    sprintf(NMEA_DATA_STORE[tag], "%2.5s", fmt_data);
    break;
  default:
    if (strlen(fmt_data) < 3)
      sprintf(NMEA_DATA_STORE[tag], "0%2.4s", fmt_data);
    else
      sprintf(NMEA_DATA_STORE[tag], "%2.4s", fmt_data);
    break;
  }
}

void test_screen_data_updates()
{
  current_millis = millis();
  NMEA_runSoftGenerator();
}

/**
 * Read the data from the NMEA_DATA_STORE and write
 * the value to the specific label if it excist
 */
void mfd_update_tile_data()
{
  current_millis = millis();
  if (millis() - previous_millis > UPDATE_DELAY)
  {
    // update the tags thta are use once, the tags that are copies
    // ar eput in the switch statement.
    for (int i = CTS; i < BAT; i++)
    {

      // If label excist update value
      if (tile_hash[i] != NULL)
      {
        // lv_log("wrting value to label %d: %s\n", i, NMEA_DATA_STORE[i]);
        lv_label_set_text(tile_hash[i], NMEA_DATA_STORE[i]);
      }
      switch (i)
      {
      case CTS:
        lv_label_set_text(tile_hash[CTS2], NMEA_DATA_STORE[i]);
        break;
      case COG:
        lv_label_set_text(tile_hash[COG2], NMEA_DATA_STORE[i]);
        break;
      case SOG:
        lv_label_set_text(tile_hash[SOG2], NMEA_DATA_STORE[i]);
        lv_chart_set_next_value(sogplot, ser_sog, boat_sog);
        break;
      case AWS:
        lv_label_set_text(tile_hash[AWS2], NMEA_DATA_STORE[i]);
        break;
      case DPT:
        lv_chart_set_next_value(dptplot, ser_dpt, boat_dpt);
        break;
      default:
        break;
      }
    }
  }
}

/**
 * Menu button handler from the main menu to select
 * 1 of the 5 panels.
 * Cuurently trip panel is always visible but overlayed with the follwoing panels
 * So need to press a button once to show and twcie to hide
 *
 * TO DO: pressing a button should hide all others and visa versa
 */
void menu_btn_event_cb(lv_event_t *event)
{
  // lv_obj_t *panel_btn = NULL;
  lv_event_code_t code = lv_event_get_code(event);
  lv_obj_t *btn = lv_event_get_target_obj(event);
  if (code == LV_EVENT_CLICKED)
  {
    int *ipnl = (int *)lv_event_get_user_data(event);
    lv_log("index from button pressed = %d\n", *ipnl);
    for (int i = TRIP_PNL; i <= CONFIG_PNL; i++)
    {
      if (i == *ipnl)
      {
        lv_log("panel to show is %s\n", lv_obj_get_name(mfd_panel_array[i]));
        mfd_show_panel(mfd_panel_array[i]);
      }
      else
        mfd_hide_panel(mfd_panel_array[i]);
    }
  }
  /**
   * Without below code an Assertion is causing a crash after a couple of panelswitches due to low memory.
   * Loggin the memory monitor did not reveal a leakage of memory .
   * It seems that the memory allocated for the user data is not sufficient or not properly managed. 
   * By adding the below code the assertion does not seem to happen anymore. It is not clear why this is the case, 
   * but it might be related to the way the event system handles user data and memory management.
   */
  mem_monitor = (lv_mem_monitor_t *)malloc(sizeof(lv_mem_monitor_t));
  lv_mem_monitor(mem_monitor);
  // lv_log("Memory monitor after btn press: total size: %d, free size: %d, used size: %d\n", 
  //mem_monitor->total_size, mem_monitor->free_size, mem_monitor->total_size - mem_monitor->free_size);

  lv_free(mem_monitor);
}

/**
 * Basically the main routine for the displays etc
 */
lv_obj_t *screen_main_create(void)
{
  LV_TRACE_OBJ_CREATE("begin");

  static bool style_inited = false;

  if (!style_inited)
  {
    lv_log("Initiating styles...\n");
    mfd_styles_inited = false;
    mfd_init_styles();
    style_inited = true;
  }

  if (screen_main == NULL)
    screen_main = lv_obj_create(lv_screen_active()); // lv_obj_create(NULL);
  // Create the main screen as a permanent screen

  screen_active = screen_main;
  // lv_log("screen active = %s", lv_obj_get_name(screen_main));
  lv_obj_set_name_static(screen_active, "screen_main_#");

  // 1st thing to do is initialize the data_store for NMEA values
  if (!data_store_inited)
    init_data_store();

  // Add a menubar
  lv_obj_t *menu_bar = lv_obj_create(screen_active);
  lv_obj_remove_style_all(menu_bar);
   lv_obj_set_flex_flow(menu_bar, LV_FLEX_FLOW_COLUMN);
  mfd_set_menu_bar_style(menu_bar);
  lv_obj_set_x(menu_bar, 0);
  lv_obj_set_y(menu_bar, 0);
  lv_obj_set_width(menu_bar, 145);
  lv_obj_set_height(menu_bar, lv_pct(100));
  lv_obj_set_style_radius(menu_bar, 5, 0);
  lv_obj_set_style_margin_all(menu_bar, 2, 0);

  // Create the panels TRIP, WIND, COURSE, BRIGHTNESS and SETTINGS
  lv_obj_t *mfd_trip_panel = mfd_panel_create(screen_active, "TRIP");
  mfd_panel_array[TRIP_PNL] = mfd_trip_panel;
  mfd_show_panel(mfd_trip_panel);
  lv_obj_t *mfd_wind_panel = mfd_panel_create(screen_active, "WIND");
  mfd_panel_array[WIND_PNL] = mfd_wind_panel;
  mfd_hide_panel(mfd_wind_panel);

  lv_obj_t *mfd_course_panel = mfd_panel_create(screen_active, "COURSE");
  mfd_panel_array[COURSE_PNL] = mfd_course_panel;
  mfd_hide_panel(mfd_course_panel);

  lv_obj_t *mfd_bright_panel = mfd_brightness_panel_create(screen_active, "BRIGHT");
  // mfd_brightness_panel_create(mfd_bright_panel);
  mfd_panel_array[BRIGHT_PNL] = mfd_bright_panel;
  mfd_hide_panel(mfd_bright_panel);

  lv_obj_t *mfd_settings_panel = mfd_config_panel_create(screen_active, " SETTING");
  mfd_panel_array[CONFIG_PNL] = mfd_settings_panel;
  // mfd_config_panel_create(mfd_settings_panel);
  mfd_hide_panel(mfd_settings_panel);

  // Add the buttons and their link to their panel to the menubar
  // The void *userdata reference  is the reference to the panel to show
  lv_obj_t *trip_btn = mfd_button_create(menu_bar, "TRIP");
  // lv_obj_add_flag(trip_btn, LV_OBJ_FLAG_CHECKABLE);
  // lv_obj_add_event_cb(trip_btn, menu_btn_event_cb, LV_EVENT_CLICKED, mfd_trip_panel);
  lv_obj_add_event_cb(trip_btn, menu_btn_event_cb, LV_EVENT_ALL, panel_hash[TRIP_PNL]);

  lv_obj_t *wind_btn = mfd_button_create(menu_bar, "WIND");
  // lv_obj_add_event_cb(wind_btn, menu_btn_event_cb, LV_EVENT_CLICKED, mfd_wind_panel);
  lv_obj_add_event_cb(wind_btn, menu_btn_event_cb, LV_EVENT_ALL, panel_hash[WIND_PNL]);

  lv_obj_t *course_btn = mfd_button_create(menu_bar, "COURSE");
  // lv_obj_add_event_cb(course_btn, menu_btn_event_cb, LV_EVENT_CLICKED, mfd_course_panel);
  lv_obj_add_event_cb(course_btn, menu_btn_event_cb, LV_EVENT_ALL, panel_hash[COURSE_PNL]);

  lv_obj_t *bright_btn = mfd_button_create(menu_bar, "BRIGHT");
  // lv_obj_add_event_cb(bright_btn, menu_btn_event_cb, LV_EVENT_CLICKED, mfd_bright_panel);
  lv_obj_add_event_cb(bright_btn, menu_btn_event_cb, LV_EVENT_ALL, panel_hash[BRIGHT_PNL]);

  lv_obj_t *setting_btn = mfd_button_create(menu_bar, "SETTING");
  // lv_obj_add_event_cb(setting_btn, menu_btn_event_cb, LV_EVENT_CLICKED, mfd_settings_panel);
  lv_obj_add_event_cb(setting_btn, menu_btn_event_cb, LV_EVENT_ALL, panel_hash[CONFIG_PNL]);

  lv_obj_t *lv_button_0 = mfd_button_create(menu_bar, "Sjean");
  // lv_obj_add_flag(wind_btn, LV_OBJ_FLAG_CHECKABLE);

  // Create the about screen as a child of the main screen
  lv_obj_add_screen_create_event(lv_button_0, LV_EVENT_CLICKED, screen_about_create, LV_SCREEN_LOAD_ANIM_MOVE_TOP, 500, 0);

  // Add the tiles and their tile_data objects to the trip panel
  CTSbox = mfd_panel_add_tile(mfd_trip_panel, "CTS", "o", CTSbox);
  tile_hash[CTS] = mfd_tile_add_tile_data(CTSbox, tile_hash[CTS]);

  COGbox = mfd_panel_add_tile(mfd_trip_panel, "COG", "o", COGbox);
  tile_hash[COG] = mfd_tile_add_tile_data(COGbox, tile_hash[COG]);

  SOGbox = mfd_panel_add_tile(mfd_trip_panel, "SOG", "KTS", SOGbox);
  tile_hash[SOG] = mfd_tile_add_tile_data(SOGbox, tile_hash[SOG]);
  lv_obj_t *SOGminitile = mfd_tile_add_mini_tile(SOGbox, SOGminitile);
  sogplot = lv_chart_create(SOGminitile);
  lv_obj_set_size(sogplot, MINI_TILE_WIDTH, MINI_TILE_HEIGHT);
  lv_chart_set_type(sogplot, LV_CHART_TYPE_LINE);
  lv_chart_set_point_count(sogplot, 1500);
  lv_chart_set_axis_range(sogplot, LV_CHART_AXIS_PRIMARY_Y, 0, 10);
  lv_obj_set_style_bg_color(sogplot, lv_color_hex(DAY_BACKGROUND), LV_PART_MAIN);
  lv_chart_set_div_line_count(sogplot, 5, 6);
  lv_obj_set_style_line_color(sogplot, lv_color_hex(SUN_LINE_COLOR), 0);
  ser_sog = lv_chart_add_series(sogplot, lv_color_hex(NIGHT_TEXT_ON_BACKGROUND), LV_CHART_AXIS_PRIMARY_Y);
  lv_chart_set_all_value(sogplot, ser_sog, 0);
  lv_obj_t *lbl_sog = lv_label_create(SOGminitile);
  lv_label_set_text(lbl_sog, "0");
  lv_obj_set_style_text_font(lbl_sog, &ui_font_lv_conthrax_16, 0);
  lv_obj_set_align(lbl_sog, LV_ALIGN_BOTTOM_LEFT);

  DPTbox = mfd_panel_add_tile(mfd_trip_panel, "DPT", "m", DPTbox);
  tile_hash[DPT] = mfd_tile_add_tile_data(DPTbox, tile_hash[DPT]);
  lv_obj_t *DPTminitile = mfd_tile_add_mini_tile(DPTbox, DPTminitile);
  dptplot = lv_chart_create(DPTminitile);
  lv_obj_set_size(dptplot, MINI_TILE_WIDTH, MINI_TILE_HEIGHT);
  lv_chart_set_type(dptplot, LV_CHART_TYPE_LINE);
  lv_chart_set_point_count(dptplot, 1500);
  lv_chart_set_axis_range(dptplot, LV_CHART_AXIS_PRIMARY_Y, -10, 0);
  lv_obj_set_style_bg_color(dptplot, lv_color_hex(DAY_BACKGROUND), LV_PART_MAIN);
  lv_chart_set_div_line_count(dptplot, 5, 6);
  lv_obj_set_style_line_color(dptplot, lv_color_hex(SUN_LINE_COLOR), 0);
  ser_dpt = lv_chart_add_series(dptplot, lv_color_hex(NIGHT_TEXT_ON_BACKGROUND), LV_CHART_AXIS_PRIMARY_Y);
  lv_chart_set_all_value(dptplot, ser_dpt, 0);
  lv_obj_t *lbl_dpt = lv_label_create(DPTminitile);
  lv_label_set_text(lbl_dpt, "0");
  lv_obj_set_style_text_font(lbl_dpt, &ui_font_lv_conthrax_16, 0);
  lv_obj_set_align(lbl_dpt, LV_ALIGN_TOP_LEFT);

  AWSbox = mfd_panel_add_tile(mfd_trip_panel, "AWS", "KTS", AWSbox);
  tile_hash[AWS] = mfd_tile_add_tile_data(AWSbox, tile_hash[AWS]);

  TRPbox = mfd_panel_add_tile(mfd_trip_panel, "TRP", "nm", TRPbox);
  tile_hash[TRP] = mfd_tile_add_tile_data(TRPbox, tile_hash[TRP]);

  lv_obj_t *TRPminitile = mfd_tile_add_mini_tile(TRPbox, TRPminitile);
  tile_hash[LOG] = mfd_mini_tile_add_data(TRPminitile);
  mfd_mini_tile_set_label(TRPminitile, "LOG");
  mfd_mini_tile_set_unit(TRPminitile, "nm");

  // Add the tiles and their tile_data objects to thwe wind panel
  AWAbox = mfd_panel_add_tile(mfd_wind_panel, "AWA", "o", AWAbox);
  tile_hash[AWA] = mfd_tile_add_tile_data(AWAbox, tile_hash[AWA]);
  lv_obj_t *DIRminitile = mfd_tile_add_mini_tile(AWAbox, DIRminitile);
  tile_hash[DIR] = mfd_mini_tile_add_data(DIRminitile);

  TWAbox = mfd_panel_add_tile(mfd_wind_panel, "TWA", "o", TWAbox);
  tile_hash[TWA] = mfd_tile_add_tile_data(TWAbox, tile_hash[TWA]);

  mfd_panel_add_spacer(mfd_wind_panel);

  AWSbox2 = mfd_panel_add_tile(mfd_wind_panel, "AWS", "KTS", AWSbox2);
  tile_hash[AWS2] = mfd_tile_add_tile_data(AWSbox2, tile_hash[AWS2]);

  TWSbox = mfd_panel_add_tile(mfd_wind_panel, "TWS", "KTS", SOGbox);
  tile_hash[TWS] = mfd_tile_add_tile_data(TWSbox, tile_hash[TWS]);

  mfd_panel_add_spacer(mfd_wind_panel);

  // Add the tiles and their tile_data objects to thwe course panel
  CTSbox2 = mfd_panel_add_tile(mfd_course_panel, "CTS", "o", CTSbox2);
  tile_hash[CTS2] = mfd_tile_add_tile_data(CTSbox2, tile_hash[CTS2]);

  HDGbox = mfd_panel_add_tile(mfd_course_panel, "HDG", "o", HDGbox);
  tile_hash[HDG] = mfd_tile_add_tile_data(HDGbox, tile_hash[HDG]);

  COGbox2 = mfd_panel_add_tile(mfd_course_panel, "COG", "o", COGbox2);
  tile_hash[COG2] = mfd_tile_add_tile_data(COGbox2, tile_hash[COG2]);

  SOGbox2 = mfd_panel_add_tile(mfd_course_panel, "SOG", "KTS", SOGbox2);
  tile_hash[SOG2] = mfd_tile_add_tile_data(SOGbox2, tile_hash[SOG2]);

  VMGbox = mfd_panel_add_tile(mfd_course_panel, "VMG", "KTS", VMGbox);
  tile_hash[VMG] = mfd_tile_add_tile_data(VMGbox, tile_hash[VMG]);

  CMGbox = mfd_panel_add_tile(mfd_course_panel, "CMG", "0", CMGbox);
  tile_hash[CMG] = mfd_tile_add_tile_data(CMGbox, tile_hash[CMG]);

  LV_TRACE_OBJ_CREATE("finished");

  return screen_active;
}


