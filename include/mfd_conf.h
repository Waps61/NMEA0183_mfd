#ifndef MFD_CONF_H
#define MFD_CONF_H
#include <lvgl.h> // v9.3.0
//#include <Preferences.h>
// Configuration constants for MFD (Multi-Function Display)
#define MFD_SCREEN_WIDTH 1024
#define MFD_SCREEN_HEIGHT 600
#define BOX_BUFFER_SIZE 15
#define MFD_USE_DATABOX 1

#define JC1060P470
#define GFX_DEV_DEVICE JC1060P470
#define GFX_ROTATION 0
#define GFX_H_RES 1024
#define GFX_V_RES 600
#define GFX_BL 23 // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
#define DSI_PANEL
#define TOUCH_SDA 7
#define TOUCH_SCL 8
#define TOUCH_INT 21
#define TOUCH_RST -1
#define DEFAULT_BRIGHTNESS 25 // 25% brightness as default
#define ACTUAL_BRIGHTNESS DEFAULT_BRIGHTNESS

#define DEMO // When active data is simulated for the display outcomment if not used
// #define TEST

static lv_subject_t brightness_subject;
static bool brightness_changed;
static int brightness_value;
static bool update_data_values;
static lv_obj_t *screen_active = NULL;

/*Create a Tabview box object*/
static lv_obj_t *SOGbox, *CTSbox, *COGbox, *DPTbox, *AWAbox, *TWAbox, *AWSbox, *TWSbox,
    *TRPbox, *VMGbox, *CMGbox, *LOGbox, *HDGbox = NULL;
// some boxes ure used twice
static lv_obj_t *SOGbox2, *COGbox2, *CTSbox2, *AWSbox2 = NULL;

//static Preferences mfdsettings;
static int mfd_baudrate; // default to 38400
static bool mfd_wifi;    // default to 0 = off
//static String mfd_ssid[25];
//static String mfd_pwd[25];
static long mfd_log;

#endif // MFD_CONF_H