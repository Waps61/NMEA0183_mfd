/**
 * @file mfd_persistent.cpp
 *
 * Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
 * Contact:  waps61 @gmail.com
 *
 * Persistent storage implementation. Fixes vs v1.3 
 *   - `nvr_millis` and `mfd_preferences` now have exactly one definition
 *     (here), instead of one private copy per translation unit that
 *     included the header.
 *   - Arduino `String` objects are now passed through `.c_str()` before
 *     reaching a `%s` format specifier (they previously weren't, which is
 *     undefined behavior -- a plausible contributor to the memory
 *     assertions chased down in the v1.0 changelog).
 */
#include <Arduino.h>
#include <lvgl.h>
#include <persist/mfd_persistent.h>

unsigned long nvr_millis = 0;
static Preferences mfd_preferences;

void mfd_ship_config_set_baudrate(int value)
{
  ship_config.baudrate = value;
}

int mfd_ship_config_get_baudrate()
{
  return ship_config.baudrate;
}
void mfd_ship_config_set_wifi(bool value)
{
  ship_config.wifi_on = value;
}
bool mfd_ship_config_get_wifi()
{
  return ship_config.wifi_on;
}
void mfd_ship_config_set_ssid(const String &value)
{
  ship_config.SSID = value;
}
String mfd_ship_config_get_ssid()
{
  return ship_config.SSID;
}
void mfd_ship_config_set_pwd(const String &value)
{
  ship_config.pwd = value;
}
String mfd_ship_config_get_pwd()
{
  return ship_config.pwd;
}
void mfd_ship_config_set_log(float value)
{
  ship_config.ship_log = value;
}
float mfd_ship_config_get_log()
{
  return ship_config.ship_log;
}
void mfd_ship_config_set_depth_offset(float value)
{
  ship_config.depth_offset = value;
}
float mfd_ship_config_get_depth_offset()
{
  return ship_config.depth_offset;
}

static bool mfd_persistent_open()
{
  bool success = mfd_preferences.begin("NMEA_mfd", false);
  lv_log("persistent opened = %d\n", success);
  return success;
}

static void mfd_persistent_close()
{
  mfd_preferences.end();
}

mfd_pers_t mfd_read_persistent_data()
{
  mfd_pers_t persistent_data;
  if (mfd_persistent_open())
  {
    lv_log("reading persistent data\n");
    persistent_data.baudrate = mfd_preferences.getInt("baudrate", 9600);
    persistent_data.wifi_on = mfd_preferences.getBool("wifi_on", true);
    persistent_data.SSID = mfd_preferences.getString("SSID", "---");
    persistent_data.pwd = mfd_preferences.getString("pwd", "***");
    persistent_data.ship_log = mfd_preferences.getFloat("ship_log", -0.1);
    persistent_data.depth_offset = mfd_preferences.getFloat("depth_offset", 0.0);
    mfd_persistent_close();
  }
  else
  {
    lv_log("reading failed\n");
    /* Fall back to safe defaults so callers always get a usable struct,
     * even if NVS couldn't be opened (e.g. first boot / corrupted partition). */
    persistent_data.baudrate = 9600;
    persistent_data.wifi_on = true;
    persistent_data.SSID = "---";
    persistent_data.pwd = "***";
    persistent_data.ship_log = -0.1f;
    persistent_data.depth_offset = 0.0f;
  }
  return persistent_data;
}

bool mfd_write_persistent_data(mfd_pers_t *perst_data)
{
  bool success = mfd_persistent_open();
  if (success)
  {
    success &= (mfd_preferences.putInt("baudrate", perst_data->baudrate) != 0);
    lv_log("writing baudrate %d to EEPROM was =%d\n", perst_data->baudrate, success);

    success &= (mfd_preferences.putBool("wifi_on", perst_data->wifi_on) != 0);
    lv_log("writing wifi %d to EEPROM was =%d\n", (int)perst_data->wifi_on, success);

    success &= (mfd_preferences.putString("SSID", perst_data->SSID) != 0);
    lv_log("writing SSID %s to EEPROM was =%d\n", perst_data->SSID.c_str(), success);

    success &= (mfd_preferences.putString("pwd", perst_data->pwd) != 0);
    lv_log("writing pwd to EEPROM was =%d\n", success); /* never log the password itself */

    success &= (mfd_preferences.putFloat("ship_log", perst_data->ship_log) != 0);
    lv_log("writing log %0.1f to EEPROM was =%d\n", perst_data->ship_log, success);

    success &= (mfd_preferences.putFloat("depth_offset", perst_data->depth_offset) != 0);
    lv_log("writing depth offset %0.1f to EEPROM was =%d\n", perst_data->depth_offset, success);

    mfd_persistent_close();
  }

  return success;
}

bool mfd_update_persistent_key(mfd_pers_key key_id, mfd_pers_t *perst_data)
{
  bool success = false;
  if (mfd_persistent_open())
  {
    switch (key_id)
    {
    case MFD_BAUDRATE:
      success = mfd_preferences.putInt("baudrate", perst_data->baudrate) != 0;
      lv_log("baudrate update written with value %d (ok=%d)\n", perst_data->baudrate, success);
      break;
    case MFD_WIFI_ON:
      success = mfd_preferences.putBool("wifi_on", perst_data->wifi_on) != 0;
      lv_log("wifi switch update written with value %d (ok=%d)\n", perst_data->wifi_on, success);
      break;
    case MFD_SSID:
      success = mfd_preferences.putString("SSID", perst_data->SSID) != 0;
      lv_log("ssid update written (ok=%d)\n", success);
      break;
    case MFD_PWD:
      success = mfd_preferences.putString("pwd", perst_data->pwd) != 0;
      lv_log("pwd update written (ok=%d)\n", success); /* never log the password itself */
      break;
    case MFD_SHIPLOG:
      success = mfd_preferences.putFloat("ship_log", perst_data->ship_log) != 0;
      lv_log("ship log update written with value %.1f (ok=%d)\n", perst_data->ship_log, success);
      break;
    case MFD_DEPTH_OFFSET:
      success = mfd_preferences.putFloat("depth_offset", perst_data->depth_offset) != 0;
      lv_log("depth offset update written with value %.1f (ok=%d)\n", perst_data->depth_offset, success);
      break;
    default:
      break;
    }
    mfd_persistent_close();
  }
  return success;
}
