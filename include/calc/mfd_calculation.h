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

MobResult CalculateMOB(const char *mobLat, const char *mobLon,
                       const char *boatLat, const char *boatLon);

extern void processNMEAData(const char *buff);

#endif // MFD_CALCULATION_H
