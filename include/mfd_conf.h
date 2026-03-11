/**
 * @file mfd_conf.h
 */
#ifndef MFD_CONF_H
#define MFD_CONF_H
#include <lvgl.h> // v9.5.0

// Version
#define VERSION_MAJOR "0"
#define VERSION_MINOR "8.7" // post FAT and pre SAT
#define MFD_VERSION "Version: " VERSION_MAJOR "." VERSION_MINOR
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


// #define LISTENER_RATE 38400 // Baudrate for the listner, but now set through mfd_subject_baudrate
#define LISTENER_RX 32 // Serial1 Rx port
#define LISTENER_TX 33 // Serial1 TX port
// #define TALKER_RATE 38400   // Baudrate for the talker
#define TALKER_RX 22
#define TALKER_TX 23 // SoftSerial port 2

// #define DEMO // When active data is simulated for the display outcomment if not used
//  #define TEST

extern lv_subject_t brightness_subject;
extern bool brightness_changed;
extern int brightness_value;
extern bool update_data_values;
extern lv_obj_t *screen_active;
extern lv_mem_monitor_t *mem_monitor;

/*Create a Tabview box object*/
extern lv_obj_t *SOGbox, *CTSbox, *COGbox, *DPTbox, *AWAbox, *TWAbox, *AWSbox, *TWSbox,
    *TRPbox, *VMGbox, *CMGbox, *LOGbox, *HDGbox;
// some boxes ure used twice
extern lv_obj_t *SOGbox2, *COGbox2, *CTSbox2, *AWSbox2;
extern lv_obj_t *AWAGaugeZoom, *AWAGaugeTileZoom;
extern lv_obj_t *AWAGauge, *AWAGaugeTile;

extern float boat_log; // = 0.1;  // Will be set to real value when EEPROM is read

// static Preferences mfdsettings;
extern lv_subject_t mfd_subject_baudrate;
extern lv_subject_t mfd_subject_wifi;
extern lv_subject_t mfd_subject_ssid;
extern lv_subject_t mfd_subject_pwd;
extern lv_subject_t mfd_subject_log;
// extern lv_subject_t *mfd_groupsettings_array_subject[];
extern bool mfd_demo_mode; // set to true to enable demo mode, which simulates data for the display

extern char mfd_ssid_curval[26];
extern char mfd_ssid_oldval[26];
extern char mfd_pwd_curval[26];
extern char mfd_pwd_oldval[26];

extern void set_boat_log(float value);
extern float increase_boat_log(float value);
extern float get_boat_log();
extern void set_demo_mode(bool value);
extern bool get_demo_mode();

#endif // MFD_CONF_H