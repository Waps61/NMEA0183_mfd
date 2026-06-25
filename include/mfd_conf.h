/**
 * @file mfd_conf.h
 */
#ifndef MFD_CONF_H
#define MFD_CONF_H
#include <lvgl.h> // v9.5.0

// Version
#define VERSION_MAJOR "1"
#define VERSION_MINOR "1.1.1"
#define MFD_VERSION "Version: " VERSION_MAJOR "." VERSION_MINOR
// Configuration constants for MFD (Multi-Function Display)
#define MFD_SCREEN_WIDTH 1024
#define MFD_SCREEN_HEIGHT 600
#define TILE_WIDTH 260
#define TILE_HEIGHT 260
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
#define DEFAULT_BRIGHTNESS 75 // 75% brightness as default
#define ACTUAL_BRIGHTNESS DEFAULT_BRIGHTNESS

// #define LISTENER_RATE 38400 // Baudrate for the listner, but now set through mfd_subject_baudrate
#define LISTENER_RX 1 // Serial1 Rx port
#define LISTENER_TX 2 // Serial1 TX port
// #define TALKER_RATE 38400   // Baudrate for the talker
#define TALKER_RX 46
#define TALKER_TX 47 // SoftSerial port 2

// #define DEMO // When active data is simulated for the display outcomment if not used
//  #define TEST

extern lv_subject_t brightness_subject;
extern bool brightness_changed;
extern int brightness_value;
extern bool update_data_values;
extern lv_obj_t *screen_active;
extern lv_mem_monitor_t *mem_monitor;

/*Create a tile objects*/
extern lv_obj_t *SOGbox, *CTSbox, *COGbox, *DPTbox, *AWAbox, *TWAbox, *AWSbox, *TWSbox,
    *TRPbox, *VMGbox, *CMGbox, *LOGbox, *HDGbox;
// some tiles are used twice
extern lv_obj_t *SOGbox2, *COGbox2, *CTSbox2, *AWSbox2;

// and some tiles are gauges
extern lv_obj_t *AWAGaugeZoom, *AWAGaugeTileZoom;
extern lv_obj_t *AWAGauge, *AWAGaugeTile;

// and some tiles are mini tiles
extern lv_obj_t *LATminitile, *LONminitile, *DIRminitile, *LOGminitile, *DPTminitile, *SOGminitile,
    *TRPminitile, *MTWminitile;
extern float boat_log;     // = 0.1;  // Will be set to real value when EEPROM is read
extern float depth_offset; // = 0.0; // Will be set to real value when EEPROM is read

// and we have MOB tile objects updateds when MOB is active
extern lv_obj_t *MOB_cts_box, *MOB_dst_box, *MOB_pos_box;

// static Preferences mfdsettings;
extern lv_subject_t mfd_subject_baudrate;
extern lv_subject_t mfd_subject_wifi;
extern lv_subject_t mfd_subject_ssid;
extern lv_subject_t mfd_subject_pwd;
extern lv_subject_t mfd_subject_log;
extern lv_subject_t mfd_subject_depth_offset;
// extern lv_subject_t *mfd_groupsettings_array_subject[];
extern bool mfd_demo_mode; // set to true to enable demo mode, which simulates data for the display
extern bool mob_active;    //*** flag variable to indicate if the MOB is active

struct _mob_obj_t
{
    char lat[20];  // char arry to store the latitude in degrees and minutes format, e.g. "48° 51.0' N"
    char lon[20];  // char arry to store the longitude in degrees and minutes format, e.g. "005° 12.4' E"
    float cog;
    float time; // time of the MOB event, used to calculate the duration of the MOB event
    bool mob_set; // flag to indicate if the MOB data is set, used to prevent updating the MOB panel with invalid data
};
typedef struct _mob_obj_t mob_obj_t;
extern mob_obj_t *mob_data;

extern char mfd_ssid_curval[26];
extern char mfd_ssid_oldval[26];
extern char mfd_pwd_curval[26];
extern char mfd_pwd_oldval[26];

extern void set_boat_log(float value);
extern float increase_boat_log(float value);
extern float get_boat_log();
extern void set_demo_mode(bool value);
extern bool get_demo_mode();
extern void set_depth_offset(float value);
extern float get_depth_offset();
extern void set_MOB_active(bool value);
extern bool get_MOB_active();

#endif // MFD_CONF_H