/**
 * @file mfd_config_panel.h
 */
#ifndef MFD_CONFIG_PANEL_H
#define MFD_CONFIG_PANEL_H

#include <lvgl.h>
#include <font/mfd_fonts.h>
#include <persist/mfd_persistent.h>
#include <ui/mfd_panel_gen.h>
#include <mfd_conf.h>
#include <ui/mfd_themes.h>

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * Below are static variables for the configuration panel, such as the options for the baudrate
     * and the keyboard map for the on-screen keyboard
     *
     */
    /*Create an AZERTY keyboard map*/
    static const char *kb_map[] = {"1", "2", "3", LV_SYMBOL_BACKSPACE, "\n",
                                   "4", "5", "6", " ", "\n",
                                   "7", "8", "9", " ", "\n",
                                   "-", "0", ".", LV_SYMBOL_SAVE, NULL};

    /*Set the relative width of the buttons and other controls*/
    static const lv_buttonmatrix_ctrl_t kb_ctrl[] = {LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_6,
                                                     LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, (lv_buttonmatrix_ctrl_t)(LV_BUTTONMATRIX_CTRL_HIDDEN | LV_BUTTONMATRIX_CTRL_WIDTH_6),
                                                     LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, (lv_buttonmatrix_ctrl_t)(LV_BUTTONMATRIX_CTRL_HIDDEN | LV_BUTTONMATRIX_CTRL_WIDTH_6),
                                                     LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_6};

    lv_obj_t *mfd_config_panel_create(lv_obj_t *parent, const char *title);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // MFD_BRIGHT_PANEL_H