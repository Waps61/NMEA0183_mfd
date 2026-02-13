#ifndef NMEA0183_DATA_H
#define NMEA0183_DATA_H

#define VESSEL_NAME "YAZZ"
#define PROGRAM_NAME "MFD ESP32"
#define PROGRAM_VERSION "0.1"

//*** Some conversion factors
#define FTM 0.3048    // feet to meters
#define MTF 3.28084   // meters to feet
#define NTK 1.852     // nautical mile to km
#define KTN 0.5399569 // km to nautical mile
// #define PI 3.14159265359 // The definition of Pi

//*** The NMEA defines in totl 82 characters including the starting
//*** characters $ or ! and the checksum character *, the checksum
//*** AND last but not least the <CR><LF> chacters.
//*** we define one more for the terminating '\0' character for char buffers
#define NMEA_BUFFER_SIZE 82 // According NEA0183 specs the max char is 82
#define NMEA_TERMINATOR "\r\n"

//*** The maximum number of fields in an NMEA string
//*** The number is based on the largest sentence MDA,
//***  the Meteorological Composite sentence
#define MAX_NMEA_FIELDS 21

#define STACKSIZE 10 // Size of the stack; adjust according use

#define TALKER_ID "AO"
#define VARIATION "1.57,E" // Varition in Lemmer on 12-05-2020, change 0.11 per year
//*** On my boat there is an ofsett of 0.2V between the battery monitor and what
//*** is measured by the Robertson Databox
#define BATTERY_OFFSET 0.2 // Volts

//*** define NMEA tags to be used
//*** make sure you know your Talker ID used in the sentences
//*** In my case next to GP for navigation related sentences
//*** II is used for Integrated Instruments and
//*** PS is used for vendor specific tags like Stowe Marine
//*** EP is used for my ESP32 generated sentences

/* for lab testing with an NMEA simulator tool
#define _DBK "$SDDBK"   // Depth below keel
#define _DBS "$SDDBS"   // Depth below surface
#define _DBT "$SDDBT"   // Depth below transducer
*/
#define _DBK "$IIDBK" // Depth below keel
#define _DBS "$IIDBS" // Depth below surface
#define _DBT "$IIDBT" // Depth below transducer
#define _HDG "$IIHDG" // Heading  Deviation & Variation
#define _HDM "$IIHDM" // Heading Magnetic
#define _HDT "$IIHDT" // Heading True
#define _MWD "$IIMWD" // Wind Direction & Speed
#define _MTW "$IIMTW" // Water Temperature
/* for lab testing with an NMEA simulator tool
#define _MWV "$WIMWV"  // Wind Speed and Angle
*/
#define _MWV "$IIMWV" // Wind Speed and Angle
#define _ROT "$IIROT" // Rate of Turn
#define _RPM "$IIRPM" // Revolutions
#define _RSA "$IIRSA" // Rudder sensor angle
#define _VDR "$IIVDR" // Set and Drift
#define _VHW "$IIVHW" // Water Speed and Heading
#define _VLW "$IIVLW" //  Distance Traveled through Water
#define _VTG "$IIVTG" //  Track Made Good and Ground Speed
#define _VWR "$IIVWR" //  Relative Wind Speed and Angle
#define _XDR "$IIXDR" //  Cross Track Error  Dead Reckoning
#define _XTE "$IIXTE" //  Cross-Track Error  Measured
#define _XTR "$IIXTR" //  Cross Track Error  Dead Reckoning
#define _ZDA "$IIZDA" //  Time & Date - UTC, day, month, year and local time zone
//*** Some specific GPS sentences
#define _GLL "$GPGLL" // Geographic Position  Latitude/Longitude
#define _GGA "$GPGGA" // GPS Fix Data. Time, Position and fix related data for a GPS receiver
#define _GSA "$GPGSA" // GPS DOP and active satellites
#define _GSV "$GPGSV" // Satellites in view
#define _RMA "$GPRMA" // Recommended Minimum Navigation Information
#define _RMB "$GPRMB" // Recommended Minimum Navigation Information
#define _RMC "$GPRMC" // Recommended Minimum Navigation Information

//*** Some specific Robertson / Stowe Marine tags below
#define _TON "$PSTON" // Distance Nautical since reset
#define _TOE "$PSTOE" // Engine hours
#define _TOB "$PSTOB" // Battery voltage
#define _TOD "$PSTOD" // depth transducer below waterline in feet
//*** Arduino generated TAGS
#define _xDR "$" TALKER_ID "" \
             "XDR" // Arduino Transducer measurement
#define _dPT "$" TALKER_ID "" \
             "DPT" // Arduino Transducer measurement
#define _hDG "$" TALKER_ID "" \
             "HDG" // Arduino Transducer measurement

/* SPECIAL NOTE:
XDR - Transducer Measurement
1 2   3 4            n
| |   | |            |
$--XDR,a,x.x,a,c--c, ..... *hh<CR><LF>
Field Number:
1:Transducer Type
2:Measurement Data
3:Units of measurement
4:Name of transducer

There may be any number of quadruplets like this, each describing a sensor. The last field will be a checksum as usual.
Example:
$HCXDR,A,171,D,PITCH,A,-37,D,ROLL,G,367,,MAGX,G,2420,,MAGY,G,-8984,,MAGZ*41
*/

#define FIELD_BUFFER 15
#define NR_OF_NMEA_TAGS 21 // make sure this equal to nr of NMEA_TAG

// Helper variables for calculation on secondar data
static float boat_sog = 0.0;
static int boat_hdg = 0;
static int boat_awa = 0;
static int boat_cts = 0;
static int boat_cog = 0;
static float boat_dpt = 0.0;
static float boat_vmg = 0.0;
static float boat_log = 0.0;  // Will be set to real value when EEPROM is read
static float boat_trp = 0.0;
static bool trip_started = false; // to ensure trip starts at 0
static char lat_new[FIELD_BUFFER] = {0};
static char lon_new[FIELD_BUFFER] = {0};
static char lat_old[FIELD_BUFFER] = {0};
static char lon_old[FIELD_BUFFER] = {0};

#define NR_OF_NMEA_TAGS 21 // make sure this equal to nr of NMEA_TAG
// Used as a reference to the required NMEA tag
// oldVal is used for specific purposes while processing nmea data
// if tags are used more then once add them at the last tag that 
// is used only onde, like BAT in this case
static enum sequence_id {
  oldVal,
  CTS,
  COG,
  SOG,
  DPT,
  AWA,
  AWS,
  TWA,
  TWS,
  HDG,
  LOG,
  TRP,
  VMG,
  CMG,
  DIR,
  STW,
  BAT,
  CTS2,
  COG2,
  SOG2,
  AWS2
} NMEA_TAG;

/**
 * The data store is the central place where NMEA data is stored  and filled
 * and is used for incomming NMEA data and and to update the data_tiles
 */
// Declare buffers for NMEA string and display parameters
static char NMEA_DATA_STORE[NR_OF_NMEA_TAGS][NMEA_BUFFER_SIZE + 1] = {0};
// and provide a function to initiate it
static bool data_store_inited = false;
extern void init_data_store();
extern void set_data_store(enum sequence_id tag, const char data[15]);

#endif // NMEA0183_DATA_H