/**
 * @file mfd_calculation.h
 */
#ifndef MFD_CALCULATION_H
#define MFD_CALCULATION_H
#include <Arduino.h>
#include <NMEA0183_data.h>
#include <mfd_conf.h>
#include <ctype.h>
#include <math.h>



extern bool isNumeric(char *value);
extern int get_course_to_mob(const char *mob_lat, const char *mob_lon);
extern float get_distance_to_mob();
extern void processNMEAData(const char *buff);

#endif // MFD_CALCULATION_H
