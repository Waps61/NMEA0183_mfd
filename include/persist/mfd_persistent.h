/**
 * @file mfd_persistent.h
 */ 
#ifndef MFD_PERSISTENT_H
#define MFD_PERSISTENT_H
#include <Arduino.h>
#include <Preferences.h>

#define NVR_UPDATE_INTERVAL 300000 // in milli seconds this converts to 5 minutes

static long nvr_millis;  // stores the previous millis alues for NVR_UPDATE function

static Preferences mfd_preferences;
enum mfd_pers_key
{
  MFD_BAUDRATE,
  MFD_WIFI_ON,
  MFD_SSID,
  MFD_PWD,
  MFD_SHIPLOG
};

struct mfd_pers_t
{
  int baudrate;
  bool wifi_on;
  String SSID;
  String pwd;
  float ship_log;
};
extern mfd_pers_t ship_config;

extern mfd_pers_t mfd_read_persistent_data();
extern bool mfd_write_persistent_data(mfd_pers_t *pers_data);
extern bool mfd_update_persistent_key(mfd_pers_key key_id, mfd_pers_t *perst_data);

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