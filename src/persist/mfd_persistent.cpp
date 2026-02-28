
/**
 * @file mfd_persistent.c
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
  Persistent implementation. It is used to store some variables for configuration
  setting of the program
*/
#include <Arduino.h>
#include <lvgl.h>
#include <persist/mfd_persistent.h>

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
void mfd_ship_config_set_ssid(String value)
{
  ship_config.SSID = String(value);
}
String mfd_ship_config_get_ssid()
{
  return ship_config.SSID;
}
void mfd_ship_config_set_pwd(String value)
{
  ship_config.pwd = String(value);
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

bool mfd_persistent_open()
{
  bool success = false;
  Serial.begin(115200);
  success = mfd_preferences.begin("NMEA_mfd", false);
  lv_log("persistent opened = %d\n", success);
  // mfd_preferences.clear();
  return success;
}

void mfd_persistent_close()
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
    mfd_persistent_close();
  }
  else
    lv_log("reading failed\n");
  return persistent_data;
}

bool mfd_write_persistent_data(mfd_pers_t *perst_data)
{
  bool success = false;
  success = mfd_persistent_open();
  if (success)
  {
    lv_log("writing baudrate %d to EEPROM was =%d\n", perst_data->baudrate, (success &= (mfd_preferences.putInt("baudrate", perst_data->baudrate) != 0)));
    lv_log("writing wifi %d to EEPROM was =%d\n", (int)perst_data->wifi_on, (success &= (mfd_preferences.putBool("wifi_on", perst_data->wifi_on) != 0)));
    lv_log("writing SSID %s to EEPROM was =%d\n", perst_data->SSID, (success &= (mfd_preferences.putString("SSID", perst_data->SSID) != 0)));
    lv_log("writing pwd %s to EEPROM was =%d\n", perst_data->pwd, (success &= (mfd_preferences.putString("pwd", perst_data->pwd) != 0)));
    lv_log("writing log %0.1f to EEPROM was =%d\n", perst_data->ship_log, (success &= (mfd_preferences.putFloat("ship_log", perst_data->ship_log) != 0)));
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
      success = mfd_preferences.putInt("baudrate", perst_data->baudrate);
      lv_log("baudrate update writen with value %d\n", perst_data->baudrate);
      break;
    case MFD_WIFI_ON:
      success = mfd_preferences.putBool("wifi_on", perst_data->wifi_on);
      lv_log("wifi switch update writen with value %d\n", perst_data->wifi_on);
      break;
    case MFD_SSID:
      success = mfd_preferences.putString("SSID", perst_data->SSID);
      lv_log("ssid update writen with value *****\n");
      break;
    case MFD_PWD:
      success = mfd_preferences.putString("pwd", perst_data->pwd);
      lv_log("pwd update writen with value ###n", perst_data->pwd);
      break;
    case MFD_SHIPLOG:
      success = mfd_preferences.putFloat("ship_log", perst_data->ship_log);
      lv_log("ship log update writen with value %.1f\n", perst_data->ship_log);
      break;
    default:
      break;
    }
    mfd_persistent_close();
  }
  return success;
}
