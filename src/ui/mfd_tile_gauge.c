/**
 * @file mfd_tile_gauge.c
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  Gauge implementation. A gauge is a small 260x260 container like a tile_data to present
  an NMEA tag in a graphical way.

*/
/*********************
 *      INCLUDES
 *********************/
#include <lvgl.h>
#include <ui/mfd_themes.h>
#include <font/mfd_fonts.h>
#include <ui/mfd_tile_data.h>
#include <ui/mfd_tile_gauge.h>

/*********************
 *      DEFINES
 *********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
static lv_obj_t *needle_line = NULL;

typedef struct
{
  lv_style_t items;
  lv_style_t indicator;
  lv_style_t main;
} section_styles_t;

static section_styles_t zone1_styles;
static section_styles_t zone2_styles;
static section_styles_t zone3_styles;
static section_styles_t zone4_styles;
static section_styles_t zone5_styles;

void mfd_gauge_set_label(lv_obj_t *tile, const char *title)
{
  lv_obj_t *label = lv_label_create(tile);
  lv_obj_add_style(label, &mfd_style_day, 0);
  lv_obj_set_style_align(label, LV_ALIGN_BOTTOM_MID, 0);
  lv_label_set_text(label, title);
  lv_obj_set_style_text_font(label, &ui_font_lv_conthrax_24, 0);
}

void mfd_gauge_update(lv_obj_t *scale, int range, int value)
{

  if (value >= range)
  {
    value = range;
  }
  else if (value < -range)
  {
    value = -range;
  }

  lv_obj_t *needle = lv_obj_get_child_by_type(scale, 0, &lv_line_class);
  if (needle != NULL)
  {
    /* Update needle */
    lv_scale_set_line_needle_value(scale, needle, 124, value);
  }
  needle = NULL;
}

static void init_section_styles(section_styles_t *styles, lv_color_t color)
{
  lv_style_init(&styles->items);
  lv_style_set_line_color(&styles->items, color);
  lv_style_set_line_width(&styles->items, 0);

  lv_style_init(&styles->indicator);
  lv_style_set_line_color(&styles->indicator, color);
  lv_style_set_line_width(&styles->indicator, 0);

  lv_style_init(&styles->main);
  lv_style_set_arc_color(&styles->main, color);
  lv_style_set_arc_width(&styles->main, 20);
}

static void add_section(lv_obj_t *target_scale,
                        int32_t from,
                        int32_t to,
                        const section_styles_t *styles)
{
  lv_scale_section_t *sec = lv_scale_add_section(target_scale);
  lv_scale_set_section_range(target_scale, sec, from, to);
  lv_scale_set_section_style_items(target_scale, sec, &styles->items);
  lv_scale_set_section_style_indicator(target_scale, sec, &styles->indicator);
  lv_scale_set_section_style_main(target_scale, sec, &styles->main);
}

lv_obj_t *mfd_tile_gauge_create(lv_obj_t *tile, int range, int sector_start, int sector_end)
{

  lv_log("adding gauge to %s\n", lv_obj_get_name(tile));
  lv_obj_t *scale = lv_scale_create(tile);
  lv_obj_center(scale);
  lv_obj_set_size(scale, GAUGE_WIDTH, GAUGE_HEIGHT);

  lv_scale_set_mode(scale, LV_SCALE_MODE_ROUND_INNER);
  lv_scale_set_range(scale, (0 - range), range);
  lv_scale_set_total_tick_count(scale, 151);
  lv_scale_set_major_tick_every(scale, 15);
  lv_scale_set_angle_range(scale, 310);
  lv_scale_set_rotation(scale, 115);
  
  lv_scale_set_label_show(scale, true);

  lv_obj_set_style_length(scale, 6, LV_PART_ITEMS);
  lv_obj_set_style_length(scale, 10, LV_PART_INDICATOR);
  lv_obj_set_style_arc_width(scale, 0, LV_PART_MAIN);
  // lv_log(" gauce created....%s\n", lv_obj_get_name(scale));

  /* Zone 3: (Grey) */
  init_section_styles(&zone3_styles, lv_palette_main(LV_PALETTE_GREY));
  add_section(scale, (0 - range), range, &zone3_styles);
  /* Zone 1: (Red) Port*/
  init_section_styles(&zone1_styles, lv_palette_main(LV_PALETTE_RED));
  add_section(scale, (0 - sector_end), (0 - sector_start), &zone1_styles);
  /* Zone 2: (Green) Starbord*/
  init_section_styles(&zone2_styles, lv_palette_main(LV_PALETTE_GREEN));
  add_section(scale, sector_start, sector_end, &zone2_styles);

  lv_log("zone styles created...\n");
  lv_obj_t *needle_line = lv_line_create(scale);

  /* Optional styling */
  lv_obj_set_style_line_color(needle_line, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_line_width(needle_line, 12, LV_PART_MAIN);
  lv_obj_set_style_length(needle_line, 20, LV_PART_MAIN);
  lv_obj_set_style_line_rounded(needle_line, false, LV_PART_MAIN);
  lv_obj_set_style_pad_right(needle_line, 50, LV_PART_MAIN);
  lv_obj_set_style_pad_radial(scale, 15, LV_PART_INDICATOR);
  lv_scale_set_draw_ticks_on_top(scale, true);
  lv_log("neeedle line created created...\n");

  lv_scale_set_line_needle_value(scale, needle_line, 124, 0);

  lv_obj_t *circle = lv_obj_create(scale);
  lv_obj_set_size(circle, 140, 140);
  lv_obj_center(circle);

  lv_obj_set_style_radius(circle, LV_RADIUS_CIRCLE, 0);

  lv_obj_set_style_bg_color(circle, lv_obj_get_style_bg_color(tile, LV_PART_MAIN), 0);
  lv_obj_set_style_bg_opa(circle, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(circle, 0, LV_PART_MAIN);
  lv_log("circles created...\n");

  return scale;
}