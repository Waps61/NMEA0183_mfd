#ifndef MFD_CALCULATION_H
#define MFD_CALCULATION_H
#include <Arduino.h>
#include <NMEA0183_data.h>
#include <ctype.h>
#include <math.h>



extern bool isNumeric(char *value);
extern void processNMEAData(  const char* buff );
extern void NMEA_runSoftGenerator();

#endif // MFD_CALCULATION_H
