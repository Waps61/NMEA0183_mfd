#ifndef MFD_PERSISTENT_H
#define MFD_PERSISTENT_H
#include <Arduino.h>
#include <Preferences.h>

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
  double ship_log;
};
static mfd_pers_t ship_config;

extern mfd_pers_t mfd_read_persistent_data();
extern bool mfd_write_persistent_data(mfd_pers_t *pers_data);
extern bool mfd_update_persistent_key(mfd_pers_key key_id, mfd_pers_t *perst_data);

#endif // MFD_PERSISTENT_H