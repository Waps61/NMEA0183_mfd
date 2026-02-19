#include "test/testlab.h"
static lv_obj_t *main_screen;

static lv_style_t tst_style, tst_style_color, tst_style_color2, tmp_style;
bool style_changed = false;
void init_style()
{
  lv_style_init(&tmp_style);
  lv_style_init(&tst_style);
  lv_style_set_bg_opa(&tst_style, LV_OPA_50);
  lv_style_set_text_opa(&tst_style, LV_OPA_50);
  lv_style_set_border_width(&tst_style, 1);
  lv_style_set_text_align(&tst_style, LV_ALIGN_CENTER);
  lv_style_set_radius(&tst_style, 12);
  lv_style_set_pad_all(&tst_style, 12);
  lv_style_set_pad_gap(&tst_style, 16);
  lv_style_set_shadow_width(&tst_style, 24);
  lv_style_set_shadow_offset_x(&tst_style, 4);
  lv_style_set_shadow_offset_y(&tst_style, 6);
  lv_style_set_shadow_width(&tst_style, 5);

  lv_style_init(&tst_style_color);
  lv_style_set_bg_color(&tst_style_color, lv_color_hex(DAWN_BACKGROUND));
  lv_style_set_text_color(&tst_style_color, lv_color_hex(DAWN_TEXT_ON_BACKGROUND));
  lv_style_set_border_color(&tst_style_color, lv_color_hex(DAWN_SECONDARY));
  lv_style_set_shadow_color(&tst_style_color, lv_color_hex(DAWN_SURFACE));
  

  lv_style_init(&tst_style_color2);
  lv_style_set_bg_color(&tst_style_color2, lv_color_hex(NIGHT_BACKGROUND));
  lv_style_set_text_color(&tst_style_color2, lv_color_hex(NIGHT_TEXT_ON_BACKGROUND));
  lv_style_set_border_color(&tst_style_color2, lv_color_hex(NIGHT_SECONDARY));
  lv_style_set_shadow_color(&tst_style_color2, lv_color_hex(NIGHT_SURFACE));
  lv_style_copy(&tmp_style, &tst_style_color2);
}
lv_obj_t *tst_set_style(lv_obj_t *obj)
{
 
  lv_obj_add_style(obj, &tst_style, 0);
  lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
  return obj;
}
lv_obj_t *tst_set_style_day(lv_obj_t *obj)
{
  lv_obj_add_style(obj, &tst_style_color, 0);
  return obj;
}
lv_obj_t *tst_set_style_night(lv_obj_t *obj)
{
  lv_obj_add_style(obj, &tst_style_color2, 0);
  return obj;
}
void test_menu_btn_event_cb(lv_event_t *event)
{
  lv_obj_t *obj = NULL;
  obj = (lv_obj_t *)lv_event_get_user_data(event);
  
  lv_obj_t *btn = lv_event_get_target_obj(event);
  const char *panel_name = lv_obj_get_name(btn);
  //lv_log("panel to show is %s\n", panel_name);

  //lv_obj_replace_style(&tmp_style, &mfd_style_night);
  //lv_obj_replace_style(obj, &tst_style_color, &tst_style_color2, 0);
  //lv_obj_replace_style(obj, &tst_style_color2, &tst_style_color, 0);
  // lv_style_remove_prop(&tmp_style, LV_STYLE_BG_COLOR);
  // lv_style_remove_prop(&tmp_style,LV_STYLE_TEXT_COLOR);
  // lv_style_remove_prop(&tmp_style, LV_STYLE_BORDER_COLOR);
  // lv_style_remove_prop(&tmp_style, LV_STYLE_SHADOW_COLOR);
// lv_style_set_bg_color(&tmp_style, lv_color_hex(DAY_BACKGROUND));
//   lv_style_set_text_color(&tmp_style, lv_color_hex(DAY_TEXT_ON_BACKGROUND));
//   lv_style_set_border_color(&tmp_style, lv_color_hex(DAY_SECONDARY));
//   lv_style_set_shadow_color(&tmp_style, lv_color_hex(DAY_SURFACE));
  mfd_style_changed = true;
  recolor_tree( main_screen, &tst_style_color);
}

void test2_menu_btn_event_cb(lv_event_t *event)
{
  lv_obj_t *obj = NULL;
  obj = (lv_obj_t *)lv_event_get_user_data(event);

  lv_obj_t *btn = lv_event_get_target_obj(event);
  const char *panel_name = lv_obj_get_name(btn);
  // lv_log("panel to show is %s\n", panel_name);

  // lv_obj_replace_style(&tmp_style, &mfd_style_night);
  // lv_obj_replace_style(obj, &tst_style_color, &tst_style_color2, 0);
  // lv_obj_replace_style(obj, &tst_style_color2, &tst_style_color, 0);
  //  lv_style_remove_prop(&tmp_style, LV_STYLE_BG_COLOR);
  //  lv_style_remove_prop(&tmp_style,LV_STYLE_TEXT_COLOR);
  //  lv_style_remove_prop(&tmp_style, LV_STYLE_BORDER_COLOR);
  //  lv_style_remove_prop(&tmp_style, LV_STYLE_SHADOW_COLOR);
  // lv_style_set_bg_color(&tmp_style, lv_color_hex(DAY_BACKGROUND));
  //   lv_style_set_text_color(&tmp_style, lv_color_hex(DAY_TEXT_ON_BACKGROUND));
  //   lv_style_set_border_color(&tmp_style, lv_color_hex(DAY_SECONDARY));
  //   lv_style_set_shadow_color(&tmp_style, lv_color_hex(DAY_SURFACE));
  mfd_style_changed = true;
  recolor_tree(main_screen, &tst_style_color2);
}

void testlab_init(lv_obj_t *parent)
{
  main_screen = lv_screen_active();
  init_style();
  test_screen = lv_obj_create(main_screen);
  lv_obj_set_width(test_screen, lv_pct(98));
  lv_obj_set_height(test_screen, lv_pct(98));
  lv_obj_t *test_grnd = lv_obj_create(test_screen);
  lv_obj_set_width(test_grnd, lv_pct(98));
  lv_obj_set_height(test_grnd, lv_pct(98));
  // lv_obj_remove_style_all(menu_bar);

  lv_obj_t *tst_obj = lv_obj_create(test_screen);
  tst_set_style(tst_obj);
  lv_obj_add_style(tst_obj, &tmp_style,0);
  lv_obj_set_x(tst_obj, 250);
  lv_obj_set_y(tst_obj, 250);
  // lv_obj_set_align(tst_obj, LV_ALIGN_TOP_MID);
  // lv_obj_set_pos(tst_obj,300, 200);
  // lv_obj_set_size(tst_obj,300, 300);

  lv_obj_t *tst_btn = lv_button_create(test_screen);
  tst_set_style(tst_btn);
  tst_set_style_day(tst_btn);
  //lv_obj_set_align(tst_btn, LV_ALIGN_TOP_LEFT);
  lv_obj_set_x(tst_btn, 100);
  lv_obj_set_y(tst_btn, 50);
  // lv_obj_set_size(tst_btn,150, 75);
  lv_obj_add_flag(tst_btn, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_set_style_bg_color(tst_btn, lv_color_hex(0xFFA500), LV_STATE_FOCUSED);
  lv_obj_add_event_cb(tst_btn, test_menu_btn_event_cb, LV_EVENT_CLICKED, tst_obj);
  lv_obj_t *tst_lbl = lv_label_create(tst_btn);
  lv_label_set_text(tst_lbl, "Btn1");

  lv_obj_t *tst_btn2 = lv_button_create(test_screen);
  tst_set_style(tst_btn2);
  lv_obj_add_style(tst_btn2,&tmp_style,0);
  //lv_obj_set_align(tst_btn2, LV_ALIGN_TOP_RIGHT);
  lv_obj_set_x(tst_btn2, 400);
  lv_obj_set_y(tst_btn2, 50);
  //lv_obj_set_pos(tst_btn2,200, 50);
  //lv_obj_set_size(tst_btn2,150, 75);
  lv_obj_add_flag(tst_btn2, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_add_event_cb(tst_btn2, test2_menu_btn_event_cb, LV_EVENT_CLICKED, tst_obj);
  lv_obj_t *tst_lbl2 = lv_label_create(tst_btn2);
  lv_label_set_text(tst_lbl2, "Btn2");
  
  
}

// boss is the main screen widget
void recolor_tree(lv_obj_t *parent, lv_style_t *stl)
{
  uint32_t i;
  uint32_t count = lv_obj_get_child_count(parent);
  lv_style_value_t sval;
  for (i = 0; i < count; i++)
  {
    lv_log("child nr %d\n", i);
    lv_obj_t *child = lv_obj_get_child(parent, i);
    recolor_tree(child, stl);
    if(LV_RESULT_OK == lv_style_get_prop(stl,LV_STYLE_BG_COLOR,&sval))
    lv_obj_set_style_bg_color(child, sval.color, 0);
    if (LV_RESULT_OK == lv_style_get_prop(stl, LV_STYLE_TEXT_COLOR, &sval))
      lv_obj_set_style_text_color(child, sval.color, 0);
    if (LV_RESULT_OK == lv_style_get_prop(stl, LV_STYLE_BORDER_COLOR, &sval))
      lv_obj_set_style_border_color(child, sval.color, 0);
    if (LV_RESULT_OK == lv_style_get_prop(stl, LV_STYLE_SHADOW_COLOR, &sval))
      lv_obj_set_style_shadow_color(child, sval.color, 0);

    /* Do something with `child`. */
  }
  //lv_obj_report_style_change(NULL);
  //lv_obj_invalidate(lv_screen_active());
}
