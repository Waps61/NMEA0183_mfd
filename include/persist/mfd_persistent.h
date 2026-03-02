/**
 * @file mfd_persistent.h
 */
#ifndef MFD_PERSISTENT_H
#define MFD_PERSISTENT_H
#include <Arduino.h>
#include <Preferences.h>

// in milli seconds this converts to 10 minutes that the ships lof
// is written to persitent storage
#define NVR_UPDATE_INTERVAL 600000
static long nvr_millis; // stores the previous millis alues for NVR_UPDATE function

static Preferences mfd_preferences;
/**
 * The different keys for the persistent storage, used in the mfd_update_persistent_key function
 * to update specific keys in the persistent storage
 */
enum mfd_pers_key
{
  MFD_BAUDRATE,
  MFD_WIFI_ON,
  MFD_SSID,
  MFD_PWD,
  MFD_SHIPLOG
};

/**
 * The structure for the persistent data, used in the mfd_read_persistent_data and mfd_write_persistent_data
 * functions to read and write the persistent data
 */
struct mfd_pers_t
{
  int baudrate;
  bool wifi_on;
  String SSID;
  String pwd;
  float ship_log;
};

/**
 * The global variable for the ship configuration, used in the mfd_ship_config_set and mfd_ship_config_get functions
 * to set and get the ship configuration values
 */
extern mfd_pers_t ship_config;

extern mfd_pers_t mfd_read_persistent_data();
extern bool mfd_write_persistent_data(mfd_pers_t *pers_data);
extern bool mfd_update_persistent_key(mfd_pers_key key_id, mfd_pers_t *perst_data);

/**
 * Due to strange behaviour reulting in data loss when variables are used between different tasks,
 * the functions to set and get the ship configuration values are used to update specific keys in the
 * persistent storage, instead of using the global variable ship_config directly
 * This is a workaround for the data loss issue, and is not the most efficient way to update the persistent
 * storage, but it works for now.
 */
extern void mfd_ship_config_set_baudrate(int value);
extern int mfd_ship_config_get_baudrate();
extern void mfd_ship_config_set_wifi(bool value);
extern bool mfd_ship_config_get_wifi();
extern void mfd_ship_config_set_ssid(String value);
extern String mfd_ship_config_get_ssid();
extern void mfd_ship_config_set_pwd(String value);
extern String mfd_ship_config_get_pwd();
extern void mfd_ship_config_set_log(float value);
extern float mfd_ship_config_get_log();

#endif // MFD_PERSISTENT_H