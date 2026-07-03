/**
 * @file mfd_persistent.h
 *
 * Fixes vs v1.3 
 *   - `nvr_millis` and `mfd_preferences` were declared `static` at this
 *     header's file scope, which means every .cpp that includes this
 *     header got its OWN private copy of both. They are now `extern`
 *     here and defined exactly once in mfd_persistent.cpp.
 */
#ifndef MFD_PERSISTENT_H
#define MFD_PERSISTENT_H
#include <Arduino.h>
#include <Preferences.h>

// 600000 ms = 10 minutes between persisted ship's-log writes
#define NVR_UPDATE_INTERVAL 600000

/** Timestamp (millis()) of the last NVR ship's-log write. Single owner:
 *  defined once in mfd_persistent.cpp, read/updated from main.cpp's loop(). */
extern unsigned long nvr_millis;

/**
 * The different keys for the persistent storage, used in the
 * mfd_update_persistent_key function to update specific keys in the
 * persistent storage
 */
enum mfd_pers_key
{
  MFD_BAUDRATE,
  MFD_WIFI_ON,
  MFD_SSID,
  MFD_PWD,
  MFD_SHIPLOG,
  MFD_DEPTH_OFFSET
};

/**
 * The structure for the persistent data, used in the
 * mfd_read_persistent_data and mfd_write_persistent_data functions to
 * read and write the persistent data.
 *
 * NOTE: the WiFi password is stored via Preferences (ESP32 NVS) in
 * plaintext, same as v1.3. Acceptable for a private boat network; flagging
 * it here as a deliberate, documented choice rather than an oversight.
 */
struct mfd_pers_t
{
  int baudrate;
  bool wifi_on;
  String SSID;
  String pwd;
  float ship_log;
  float depth_offset;
};

/**
 * The global variable for the ship configuration, used in the
 * mfd_ship_config_set and mfd_ship_config_get functions to set and get
 * the ship configuration values
 */
extern mfd_pers_t ship_config;

extern mfd_pers_t mfd_read_persistent_data();
extern bool mfd_write_persistent_data(mfd_pers_t *pers_data);
extern bool mfd_update_persistent_key(mfd_pers_key key_id, mfd_pers_t *perst_data);

/**
 * Due to strange behaviour resulting in data loss when variables are used
 * between different tasks, the functions to set and get the ship
 * configuration values are used to update specific keys in the persistent
 * storage, instead of using the global variable ship_config directly.
 */
extern void mfd_ship_config_set_baudrate(int value);
extern int mfd_ship_config_get_baudrate();
extern void mfd_ship_config_set_wifi(bool value);
extern bool mfd_ship_config_get_wifi();
extern void mfd_ship_config_set_ssid(const String &value);
extern String mfd_ship_config_get_ssid();
extern void mfd_ship_config_set_pwd(const String &value);
extern String mfd_ship_config_get_pwd();
extern void mfd_ship_config_set_log(float value);
extern float mfd_ship_config_get_log();
extern void mfd_ship_config_set_depth_offset(float value);
extern float mfd_ship_config_get_depth_offset();
#endif // MFD_PERSISTENT_H
