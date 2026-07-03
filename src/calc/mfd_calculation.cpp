/**
 * @file mfd_calculation.cpp
 */
/*
  Project:  NMEA0183 Multi Function Display, Copyright 2026, Roy Wassili
  Contact:  waps61 ,gmail.com
  Tile NMEA calculation implementation. It is used to make calculations for de communication unit and
  the visualisation unit
*/
#include <calc/mfd_calculation.h>
#include <lvgl.h>
#include <string.h>

// *** Conditional Debug & Test Info to Serial Monitor
// *** by commenting out the line(s) below the debugger and or test statements will
// *** be ommitted from the code
// #define DEBUG 1
// #define TEST 1

/***********************************************************************************
   Global variables go here
*/

// Variable used to pass nmea tag values like SOG etc. back and forth between objects, max 15 char's long
char cvalue[FIELD_BUFFER] = {0};

/* -------------------------------------------------------------------------
 * Parses strings like "53°41,2344' N" or "006°22,1234' W"
 * Returns signed decimal degrees (negative for S / W).
 * ------------------------------------------------------------------------- */
static double ParseDegMinTick(const char *str)
{
  char buf[64];
  int i = 0, n;
  char degBuf[16] = {0};
  int dl = 0;
  char minBuf[16] = {0};
  int ml = 0;
  char hemi = 0;
  double deg, minutes, decimal;

  strncpy(buf, str, sizeof(buf) - 1);
  buf[sizeof(buf) - 1] = '\0';
  n = (int)strlen(buf);

  /* 1) read the degree digits */
  while (i < n && isdigit((unsigned char)buf[i]))
  {
    if (dl < (int)sizeof(degBuf) - 1)
      degBuf[dl++] = buf[i];
    i++;
  }

  /* 2) skip everything until the next digit (the degree symbol, etc.) */
  while (i < n && !isdigit((unsigned char)buf[i]))
    i++;

  /* 3) read the minutes (digits, plus ',' or '.' as decimal separator) */
  while (i < n && (isdigit((unsigned char)buf[i]) || buf[i] == ',' || buf[i] == '.'))
  {
    char c = buf[i];
    if (c == ',')
      c = '.';
    if (ml < (int)sizeof(minBuf) - 1)
      minBuf[ml++] = c;
    i++;
  }

  /* 4) scan ahead for the hemisphere letter */
  while (i < n)
  {
    char c = (char)toupper((unsigned char)buf[i]);
    if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
    {
      hemi = c;
      break;
    }
    i++;
  }

  deg = atof(degBuf);
  minutes = atof(minBuf);
  decimal = deg + minutes / 60.0;
  if (hemi == 'S' || hemi == 'W')
    decimal = -decimal;
  return decimal;
}


/**
 * Parse coordinate string in format "5221.5621,N" or "00540.8482,E"
 *
 * @param coord_str String like "5221.5621,N" or "00540.8482,E"
 * @param coord_type 0 for latitude, 1 for longitude
 * @return Decimal degrees as float
 */
float parse_coordinate(const char *coord_str, int coord_type)
{
  char value_str[20];
  char direction;
  float value;
  int degrees;
  float minutes;
  float decimal_degrees;

  // Find comma position
  const char *comma_pos = strchr(coord_str, ',');
  if (comma_pos == NULL)
  {
    return 0.0;
  }

  // Extract value part
  int value_len = comma_pos - coord_str;
  strncpy(value_str, coord_str, value_len);
  value_str[value_len] = '\0';
  value = atof(value_str);

  // Extract direction
  direction = *(comma_pos + 1);

  if (coord_type == 0)
  {
    // Latitude format: DDMM.MMMM (2 digits for degrees)
    degrees = (int)(value / 100);
    minutes = value - (degrees * 100);
  }
  else
  {
    // Longitude format: DDDMM.MMMM (3 digits for degrees)
    degrees = (int)(value / 100);
    minutes = value - (degrees * 100);
  }

  // Convert to decimal degrees
  decimal_degrees = degrees + (minutes / 60.0);

  // Apply direction (negative for South and West)
  if (direction == 'S' || direction == 'W')
  {
    decimal_degrees = -decimal_degrees;
  }

  return decimal_degrees;
}

/**
 * Claude AI generated code for calculating distance between two points
 * given in NMEA coordinate format, using the Haversine formula.
 *
 * @param lat1_str Latitude of point 1, e.g., "5221.5621,N"
 * @param lon1_str Longitude of point 1, e.g., "00540.8482,E"
 * @param lat2_str Latitude of point 2
 * @param lon2_str Longitude of point 2
 * @return Distance in nautical miles
 */
float calculate_distance(const char *lat1_str, const char *lon1_str,
                         const char *lat2_str, const char *lon2_str)
{
  // Parse coordinates to decimal degrees
  float lat1 = parse_coordinate(lat1_str, 0);
  float lon1 = parse_coordinate(lon1_str, 1);
  float lat2 = parse_coordinate(lat2_str, 0);
  float lon2 = parse_coordinate(lon2_str, 1);

  // Earth's radius in kilometers
  const float R = 6371.0;

  // Convert to radians
  float lat1_rad = lat1 * M_PI / 180.0;
  float lon1_rad = lon1 * M_PI / 180.0;
  float lat2_rad = lat2 * M_PI / 180.0;
  float lon2_rad = lon2 * M_PI / 180.0;

  // Haversine formula
  float dlat = lat2_rad - lat1_rad;
  float dlon = lon2_rad - lon1_rad;

  float a = sin(dlat / 2) * sin(dlat / 2) +
            cos(lat1_rad) * cos(lat2_rad) *
                sin(dlon / 2) * sin(dlon / 2);
  float c = 2 * atan2(sqrt(a), sqrt(1 - a));

  float distance = R * c * KTN;
  // lv_log(" distance between %s,%s anf %s,%s = %f\n", lat1_str, lon1_str, lat2_str, lon2_str, distance);

  return distance;
}


/* -------------------------------------------------------------------------
 * Main MOB function.
 *
 *   Lat / Lon   : MOB waypoint, format "53°41,2344' N" style
 *
 * Returns course to steer (deg, rounded, 0-359) and distance (NM).
 * ------------------------------------------------------------------------- */
MobResult CalculateMOB(const char *mobLat, const char *mobLon,
                       const char *boatLat, const char *boatLon)
{
  MobResult result;

  double lat1 = ParseDegMinTick(mobLat); /* MOB latitude  */
  double lon1 = ParseDegMinTick(mobLon); /* MOB longitude */
  double lat2 = ParseDegMinTick(boatLat); /* boat latitude  */
  double lon2 = ParseDegMinTick(boatLon); /* boat longitude */

  double phi1 = lat2 * M_PI / 180.0; /* boat (from) */
  double phi2 = lat1 * M_PI / 180.0; /* MOB (to)    */
  double dphi = (lat1 - lat2) * M_PI / 180.0;
  double dlambda = (lon1 - lon2) * M_PI / 180.0;

  /* Haversine great-circle distance */
  double a = sin(dphi / 2.0) * sin(dphi / 2.0) +
             cos(phi1) * cos(phi2) * sin(dlambda / 2.0) * sin(dlambda / 2.0);
  double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
  double distance = EARTH_RADIUS_NM * c;

  /* Initial bearing (course to steer) from boat to MOB */
  double y = sin(dlambda) * cos(phi2);
  double x = cos(phi1) * sin(phi2) - sin(phi1) * cos(phi2) * cos(dlambda);
  double theta = atan2(y, x);
  double bearingDeg = fmod((theta * 180.0 / M_PI) + 360.0, 360.0);

  result.courseToSteerDeg = ((int)floor(bearingDeg + 0.5)) % 360;
  result.distanceNM = (float)distance;
  return result;
}

/**
 *  function to processes the receivedChars buffer and filters sentence MWV,RM and VWR,
 * and more, which contain the SOG,COG, AWS and AWA parameters, when new data has arrived
 */
void processNMEAData(const char *buff)
{

  // lv_log("received string: %s length= %d\n", buff, strlen(buff));
  if (strlen(buff) > 0)
  {
    int field = 0; // the field number in the NMEA string,

    double tmpVal = 0.0;
    double tmpVar = 0.0;
    float distance = 0.0;
    float Y, a, b, bb = 0.0; // temp variables to calculate TWA and TWS
    char tmp[15];

    char *ptr;

    char *nmea_str;              // used to proces the tokens (seperated by ',' )
    char bfr[83] = {0};          // a copy of the referenced nmead buffer to prevent pointe problems
    strcpy(bfr, buff);           // make the copy!
    field = 0;                   // ignore sentence tag, the first field as the tag
    nmea_str = strstr(bfr, ","); // find the tokens in the NMEA string
    if (nmea_str != NULL)
    {
      char *token = strtok(nmea_str, ","); // the token is actually holding the value of the fields

      // Adjust field for empty fields at start of the NMEA string, for example when the NMEA string
      // starts with ",," then the first two fields are empty and the first value is in field 3, so we
      // need to adjust the field number accordingly
      while (nmea_str[field] == ',')
        field++;
      // Get all the fields from the NMEAstring seperated by ','
      while (token != NULL)
      {
        cvalue[0] = 0;
        // The below NMEA TAGs are processed, if the NMEA string contains one of these
        // tags then the value of the field is stored in the cvalue variable and processed further
        if ((ptr = strstr(bfr, "MWV")) != NULL ||
            (ptr = strstr(bfr, "RMC")) != NULL ||
            (ptr = strstr(bfr, "CTS")) != NULL ||
            (ptr = strstr(bfr, "TOB")) != NULL ||
            (ptr = strstr(bfr, "VWR")) != NULL ||
            (ptr = strstr(bfr, "XDR")) != NULL ||
            (ptr = strstr(bfr, "HDG")) != NULL ||
            (ptr = strstr(bfr, "MTW")) != NULL ||
            (ptr = strstr(bfr, "DPT")) != NULL ||
            (ptr = strstr(bfr, "DBT")) != NULL ||
            (ptr = strstr(bfr, "DBK")) != NULL ||
            (ptr = strstr(bfr, "BWC")) != NULL ||
            (ptr = strstr(bfr, "VHW")) != NULL)
        {
          // prepare for further manupulation and to prevent pointer problems
          // make a copy of the token
          strcpy(cvalue, token);

          // Prepare AWS and AWA values, from MWV or VWR tags
          if ((strstr(bfr, "MWV") != NULL && strstr(bfr, ",R,") != NULL) ||
              strstr(bfr, "VWR") != NULL)
          {
            if (field == 1)
            {
              boat_awa = atof(cvalue);

              boat_vmg = (float)(boat_sog * cos(boat_awa * PI / 180));
              sprintf(tmp, "%3.1f", boat_vmg);
              set_data_store(AWA, cvalue);
              set_data_store(VMG, tmp);
            }
            if (field == 2)
            {
              // Correct for +(SB) or -(Port) depending on value
              if (cvalue[0] == 'L' || cvalue[0] == 'T')
              {

                memmove(cvalue + 1, cvalue, FIELD_BUFFER - 2);
                cvalue[0] = '-';
                set_data_store(DIR, ">>>");
              }
              else
                set_data_store(DIR, "<<<");
            }
            if (field == 3)
            {
              /**
               * TWA and TWs calculation based on document from Joseph George Caldwwell,
               * "Calculating True Wind Speed and Direction from Apparent Wind Speed and Direction"
               * see: https://www.starpath.com/freeware/truewind.pdf
               **/

              Y = 90.0 - boat_awa;
              a = atof(cvalue) * cos(Y * PI / 180);
              bb = atof(cvalue) * sin(Y * PI / 180);
              b = bb - boat_sog;
              if (a != 0.0)
              {
                boat_tws = sqrt(a * a + b * b);
                boat_twa = 90 - (atan(b / a) * 180 / PI);
              }
              else
              {
                if (boat_awa > 179.0)
                  boat_tws = boat_sog + atof(cvalue);
                else
                  boat_tws = boat_sog - atof(cvalue);
                boat_twa = boat_awa;
              }

              set_data_store(AWS, cvalue);
              sprintf(tmp, "%3.1f", boat_tws);
              set_data_store(TWS, tmp);
              sprintf(tmp, "%3.0f", boat_twa);
              set_data_store(TWA, tmp);
            }
          }
          // Prepare for SOG and COG values from RMC tags
          if (strstr(bfr, "RMC") != NULL)
          {
            if (field == 3)
            {
              strcpy(lat_old, lat_new);
              sprintf(lat_new, "%s,", cvalue);
            }
            if (field == 4)
            {
              strcat(lat_new, cvalue);
              set_data_store(LAT, lat_new);
            }
            if (field == 5)
            {
              strcpy(lon_old, lon_new);
              sprintf(lon_new, "%s,", cvalue);
            }
            if (field == 6)
            {
              strcat(lon_new, cvalue);
              set_data_store(LON, lon_new);
              // when the lat lon is received for the 1st time set old values to this
              // position to make sure trip start at 0
              if (!trip_started)
              {
                trip_started = true;
                lv_subject_set_float(&mfd_subject_log, get_boat_log());
                lv_log("trip started with boat log %.1f\n and mfd log %.1f", get_boat_log(), lv_subject_get_float(&mfd_subject_log));
                strcpy(lat_old, lat_new);
                strcpy(lon_old, lon_new);
              }
              distance = calculate_distance(lat_old, lon_old, lat_new, lon_new);

              // distance between the new and previous position based omn a max speed of 20 knots
              // greater than 20/3600 = 0,00556  nautical miles per second is not possible, so if the
              // distance is greater than this value then we assume that the position is not reliable and we do
              // not update the log and trip values. To be on the safe side lets assume that it will not take
              // more then 30 seconds to receive a position update, so we will use a distance of
              // 0.00556 * 30 = 0.1668 nautical miles as the threshold for unreliable position updates
              if (distance > 0.1668)
              {

                distance = 0.0;
              }
            }
            if (field == 7)
            {
              boat_sog = atof(cvalue);
              boat_vmg = (float)(boat_sog * cos(boat_awa * PI / 180));
              sprintf(tmp, "%3.1f", boat_vmg);
              set_data_store(SOG, cvalue);
              set_data_store(VMG, tmp);
              // use a SOG threshold of 0.3 knots to prevent log and trip updates when the boat is not moving, because at
              // low speeds the position updates are not reliable and can cause log and trip values to increase when
              // the boat is actually stationary
              if (boat_sog > 0.3)
              {
                boat_trp += distance;
                increase_boat_log(distance);

                sprintf(tmp, "%.1f", boat_trp);
                set_data_store(TRP, tmp);
                sprintf(tmp, "%f", get_boat_log());
                set_data_store(LOG, tmp);
              }
              
            }
            if (field == 8)
            {
              sprintf(tmp, "%.3s", cvalue);
              boat_cog = atoi(cvalue);
              set_data_store(COG, tmp);
            }
          }
          // Prepare for DPT values, can be comming DBK,DBT or DPT tags
          if (strstr(bfr, "DBK") != NULL)
          {
            if (field == 2)
            {
              boat_dpt = atof(cvalue) + get_depth_offset();
              sprintf(tmp, "%1.1f", boat_dpt);
              set_data_store(DPT, tmp);
            }
            if (field == 3 && cvalue[0] == 'f')
            {
              
              boat_dpt = boat_dpt * FTM;
              sprintf(tmp, "%1.1f", boat_dpt);
              set_data_store(DPT, tmp);
            }
          }
          else if (strstr(bfr, "DBT") != NULL)
          {
            if (field == 3)
            {
              boat_dpt = atof(cvalue) + get_depth_offset();
              sprintf(tmp, "%1.1f", boat_dpt);
              set_data_store(DPT, tmp);
            }
          }
          else if (strstr(bfr, "DPT") != NULL)
          {
            if (field == 1)
            {
              boat_dpt = atof(cvalue) + get_depth_offset();
              sprintf(tmp, "%1.1f", boat_dpt);
              set_data_store(DPT, tmp);
            }
          }

          // Prepare for Water Temperature values, comming from MTW tags
          if (strstr(bfr, "MTW") != NULL)
          {
            if (field == 1)
            {
              set_data_store(MTW, cvalue);
            }
          }

          // Prepare for BAT values, either from TOB or BAT tags
          if (strstr(bfr, "TOB") != NULL)
          {
            if (field == 1)
            {
              set_data_store(BAT, cvalue);
            }
          }
          else if (strstr(bfr, "XDR") != NULL)
          {
            if (field == 2)
            {
              set_data_store(BAT, cvalue);
            }
          }
          // Get CTS from BWC
          if (strstr(bfr, "BWC") != NULL)
          {
            if (field == 6)
            {
              sprintf(tmp, "%.3s", cvalue);
              boat_cts = atoi(cvalue);
              set_data_store(CTS, tmp);
              sprintf(tmp, "%d", boat_cts - boat_cog);
              set_data_store(CMG, tmp);
            }
          }
          // Prepare for HDG values, converting from magnetic to true values
          if (strstr(bfr, "HDG") != NULL)
          {
            if (field == 1)
            {
              // Magnetinc Sensor reading
              tmpVal = atof(cvalue);
            }
            if (field == 2)
            {
              // Magnetic Deviation
              tmpVar = atof(cvalue);
            }
            if (field == 3)
            {
              if (cvalue[0] == 'E')
              {
                // For Easterly deviation add to magnetic sensor reading
                // For Magnetic Heading
                tmpVal += tmpVar;
              }
              else
              {
                // For Westerly deviation subtract from magnetic sensor reading
                // for Mahnetic Heading
                tmpVal -= tmpVar;
              }
            }
            if (field == 4)
            {
              // Magnetic Variation
              tmpVar = atof(cvalue);
            }
            if (field == 5)
            {
              if (cvalue[0] == 'E')
              {
                // For Easterly variation add to magnetic heading
                // For True Heading
                tmpVal += tmpVar;
              }
              else
              {
                // For Westerly variation subtract from magnetic heading
                // for True Heading
                tmpVal -= tmpVar;
              }
            }

            // Storing True Heading!!
            sprintf(tmp, "%.1f", tmpVal);
            set_data_store(HDG, tmp);
          }
          else if (strstr(bfr, "VHW") != NULL)
          {
            if (field == 1 && cvalue != NULL)
            {
              // Heading degrees True
              set_data_store(HDG, cvalue);
            }
            if (field == 3 && cvalue != NULL)
            {
              // Heading degrees Magnetic

              set_data_store(HDG, cvalue);
            }
            if (field == 5 && cvalue != NULL)
            {
              // Speed trough water
              set_data_store(STW, cvalue);
            }
          }
          // get next token
        }
        field++;
        token = strtok(NULL, ",");
      }
    }
#ifdef DEBUG
    else
      lv_log("\nno comma found in NMEA string: %s\n", bfr);
#endif
  }
}
