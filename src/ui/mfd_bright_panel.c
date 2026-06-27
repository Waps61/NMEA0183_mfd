/**
 * @file mfd_bright_panel.c
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  Brightness screen implementation. The brightness screen controls the 4 themes setting for
  Daylight, Sunlight, Dawn and NIght setting for the display. Typical color schemes are used.
  It is also used to control the backlight ilumination between 5 and 100%
*/
#include <lvgl.h>

#include <ui/mfd_bright_panel.h>
#include <mfd_conf.h>
#include <ui/mfd_themes.h>
#include <font/mfd_fonts.h>

static const char *btnm_map[] = {"Day", "Sun", "Dawn", "Night", ""};
static int tstcntr = 0;

static lv_style_t style_btnbase;

/**
 * background holding place for the 4 brightness buttons
 */
void init_style_btnbase()
{
  lv_style_init(&style_btnbase);

  lv_style_set_bg_color(&style_btnbase, lv_color_hex(DAWN_BACKGROUND));

  lv_style_set_bg_color(&style_btnbase, lv_color_lighten(lv_color_hex(DAWN_SURFACE), LV_OPA_30));
  lv_style_set_border_width(&style_btnbase, 2);
  lv_style_set_border_color(&style_btnbase, lv_color_hex(DAY_LINE_COLOR));
  lv_style_set_radius(&style_btnbase, 10);
  lv_style_set_shadow_width(&style_btnbase, 10);
  lv_style_set_shadow_offset_y(&style_btnbase, 5);
  lv_style_set_shadow_opa(&style_btnbase, LV_OPA_50);
  lv_style_set_text_font(&style_btnbase, &ui_font_lv_conthrax_24);

  lv_style_set_width(&style_btnbase, 750);
  lv_style_set_height(&style_btnbase, 200);
  mfd_style_changed = UNCHANGED_STYLE;
}

static void brightness_slider_event_cb(lv_event_t *e)
{
  lv_obj_t *slider = lv_event_get_target_obj(e);
  lv_event_code_t sliderstate = lv_event_get_code(e);
  char buf[8];
  lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(slider));
  if (sliderstate == LV_INDEV_STATE_RELEASED || LV_EVENT_RELEASED)
  { // sent once when Slider is released
    brightness_changed = true;
    brightness_value = (int)(lv_slider_get_value(slider) * 2.5);

    set_backlight(brightness_value);
  }
}

static void buttonbar_event_cb(lv_event_t *e)
{

  lv_obj_t *obj = lv_event_get_target_obj(e);
  lv_draw_task_t *draw_task = lv_event_get_draw_task(e);
  lv_draw_dsc_base_t *base_dsc = (lv_draw_dsc_base_t *)lv_draw_task_get_draw_dsc(draw_task);

  /*When the button matrix draws the buttons...*/
  if (base_dsc->part == LV_PART_ITEMS)
  {
    bool pressed = false;
    if (lv_buttonmatrix_get_selected_button(obj) == base_dsc->id1 && lv_obj_has_state(obj, LV_STATE_PRESSED))
    {
      pressed = true;
    }
    lv_draw_box_shadow_dsc_t *box_shadow_draw_dsc = lv_draw_task_get_box_shadow_dsc(draw_task);
    if (box_shadow_draw_dsc)
    {
      box_shadow_draw_dsc->width = 16;
      box_shadow_draw_dsc->ofs_x = 3;
      box_shadow_draw_dsc->ofs_y = 3;
    }
    lv_draw_label_dsc_t *label_draw_dsc = lv_draw_task_get_label_dsc(draw_task);
    if (base_dsc->id1 == 0)
    {
      lv_draw_fill_dsc_t *fill_draw_dsc = lv_draw_task_get_fill_dsc(draw_task);
      if (fill_draw_dsc)
      {
        if (pressed)
        {
          fill_draw_dsc->color = lv_color_hex(DAY_SURFACE);
          mfd_set_style_changed(DAY_STYLE);
        }
        else
          fill_draw_dsc->color = lv_color_hex(DAY_BACKGROUND);
      }
      if (label_draw_dsc)
      {
        label_draw_dsc->color = lv_color_hex(DAY_TEXT_ON_BACKGROUND);
      }
    }
    else if (base_dsc->id1 == 1)
    {
      lv_draw_fill_dsc_t *fill_draw_dsc = lv_draw_task_get_fill_dsc(draw_task);
      if (fill_draw_dsc)
      {
        if (pressed)
        {
          fill_draw_dsc->color = lv_color_hex(SUN_SURFACE);
          mfd_set_style_changed(SUN_STYLE);
        }
        else
          fill_draw_dsc->color = lv_color_hex(SUN_BACKGROUND);
      }

      if (label_draw_dsc)
      {
        label_draw_dsc->color = lv_color_hex(SUN_TEXT_ON_BACKGROUND);
      }
    }

    else if (base_dsc->id1 == 2)
    {
      lv_draw_fill_dsc_t *fill_draw_dsc = lv_draw_task_get_fill_dsc(draw_task);
      if (fill_draw_dsc)
      {
        if (pressed)
        {
          fill_draw_dsc->color = lv_color_hex(DAWN_SURFACE);
          mfd_set_style_changed(DAWN_STYLE);
        }
        else
          fill_draw_dsc->color = lv_color_hex(DAWN_BACKGROUND);
      }

      if (label_draw_dsc)
      {
        label_draw_dsc->color = lv_color_hex(DAWN_TEXT_ON_BACKGROUND);
      }
    }
    /*Change the draw descriptor of the 3rd button*/
    else if (base_dsc->id1 == 3)
    {
      lv_draw_fill_dsc_t *fill_draw_dsc = lv_draw_task_get_fill_dsc(draw_task);
      if (fill_draw_dsc)
      {
        if (pressed)
        {
          fill_draw_dsc->color = lv_color_hex(NIGHT_SURFACE);
          mfd_set_style_changed(NIGHT_STYLE);
        }
        else
          fill_draw_dsc->color = lv_color_hex(NIGHT_BACKGROUND);
      }
      if (label_draw_dsc)
      {
        label_draw_dsc->color = lv_color_hex(NIGHT_TEXT_ON_BACKGROUND);
      }
    }
  }
}

lv_obj_t *mfd_brightness_panel_create(lv_obj_t *parent, const char *title)
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
  paneldata->max_nr_of_tiles = MAX_NR_OF_TILES;
  paneldata->tile_count = 0;
  paneldata->tile_spacing_x = TILE_SPACING_X;
  paneldata->tile_spacing_y = TILE_SPACING_Y;
  paneldata->tile_width = TILE_WIDTH;
  paneldata->tile_height = TILE_HEIGHT;
  paneldata->font_size = 96;
  lv_obj_set_user_data(panel, paneldata);

  if (title != NULL)
  {
    lv_obj_t *label = lv_label_create(panel);
    lv_obj_set_style_align(label, LV_ALIGN_TOP_MID, 0);
    lv_label_set_text(label, title);
    lv_obj_set_align(label, LV_ALIGN_TOP_MID);
  }

  init_style_btnbase();
  lv_subject_init_int(&brightness_subject, (int32_t)ACTUAL_BRIGHTNESS);
  lv_obj_t *btnm = lv_buttonmatrix_create(panel);
  lv_obj_add_style(btnm, &style_btnbase, 0);
  lv_obj_add_event_cb(btnm, buttonbar_event_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
  lv_obj_add_flag(btnm, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);
  lv_buttonmatrix_set_map(btnm, btnm_map);
  lv_obj_center(btnm);

  lv_obj_t *slider = lv_slider_create(panel);
  lv_obj_add_event_cb(slider, brightness_slider_event_cb, LV_EVENT_RELEASED | LV_INDEV_STATE_RELEASED, NULL); /* Assign an event callback */
  lv_slider_set_range(slider, 5, 100);
  lv_bar_set_start_value(slider, 10, LV_ANIM_OFF);
  lv_slider_bind_value(slider, &brightness_subject);
  lv_obj_align(slider, LV_ALIGN_BOTTOM_MID, 0, -60);
  lv_obj_set_style_border_width(slider, 1, 0);
  lv_obj_add_style(slider, &mfd_style, 0);

  lv_obj_t *dimdown = lv_label_create(panel);
  lv_label_set_text(dimdown, "-");
  lv_obj_set_style_text_font(dimdown, &ui_font_lv_conthrax_24, 0);
  lv_obj_align(dimdown, LV_ALIGN_BOTTOM_LEFT, 225, -60);

  lv_obj_t *dimup = lv_label_create(panel);
  lv_label_set_text(dimup, "+");
  lv_obj_set_style_text_font(dimup, &ui_font_lv_conthrax_24, 0);
  lv_obj_align(dimup, LV_ALIGN_BOTTOM_RIGHT, -225, -60);

  // Create a label below the slider
  lv_obj_t *label = lv_label_create(panel);
  lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -90);
  lv_obj_add_style(label, &mfd_style_day, LV_STATE_DEFAULT);
  lv_label_bind_text(label, &brightness_subject, "%d %%");
  return panel;
}