/**
 * @file mfd_communication.cpp
 */
#include <Arduino.h>
/*
  Project:  Yazz_NMEAtor_ESP32.cpp, Copyright 2020, Roy Wassili
  Contact:  waps61 @gmail.com
/*
    This is the NMEA communication unit for receiving and sending NMEA data
    Include the necessary libraries
*/
#include <comms/mfd_communication.h>

/*
   Definitions go here
*/
// *** Conditional Debug & Test Info to Serial Monitor
// *** by commenting out the line(s) below the debugger and or test statements will
// *** be ommitted from the code
#define DEBUG 1
// #define TEST 1

/*
   If there is some special treatment needed for some NMEA sentences then
   add the their definitions to the NMEA_SPECIALTY definition
   The pre-compiler concatenates string literals by using "" in between
*/

// unsigned long tmr1 = 0;

// SoftwareSerial nmeaSerialOut; // // signal need to be inverted for RS-232

/*
 * Setting for Serial interrupt
 */



/*
debugWrite() <--provides basic debug info from other tasks
takes a String as input parameter
*/
void debugWrite(const char *debugMsg)
{
#ifdef DEBUG
  if (strlen(debugMsg) > 1)
    lv_log("%s\n", debugMsg);

#endif
}

/*
Clear the inputbuffer by reading until empty
*/
void clearNMEAInputBuffer()
{
  int i = 0;
  while (Serial1.available() > 0)
  {
    Serial1.read();
  }
}

int NMEA_startTalking(const char *nmea_buff)
{
  int i = 0;
  while (nmea_buff[i] > 0)
    Serial2.write(nmea_buff[i++]);
  return i;
}

/**
 * reads the serial port  and ckeks for valid nmea data starting with
 * character '$' only (~ and ! can be skipped as start charcter)
 * This is a state based process, where the state is determined by the nmeaStatus variable
 * Initial state is INVALID
 * When a start character is found, the state changes to RECEIVING and the nmeaBuffer is filled
 * with the incoming characters until a '*' character is found, then the state changes to CHECKSUMMING
 * and the checksum characters are read until a newline character is found, then the state changes to
 * TERMINATING and the nmeaDataReady flag is set to true, and the nmeaBuffer is processed by the
 * processNMEAData function, and then the state changes back to INVALID
 */
void NMEA_startListening()
{
  char cIn = '\0';
  while (Serial1.available() > 0 && nmeaStatus != TERMINATING)
  {
    cIn = Serial1.read();
    switch (cIn)
    {
    // case '~':   //skipped and don't waste time on it, because it's not used in NMEA0183 and is reserved by NMEA
    //  reserved by NMEA
    // case '!':   //skipped and don't waste time on it, because we don't use AIS data in this project
    //  for AIS info
    case '$':
      // for general NMEA info
      nmeaStatus = RECEIVING;
      nmeaIndex = 0;
      break;
    case '*':
      if (nmeaStatus == RECEIVING)
      {
        nmeaStatus = CHECKSUMMING;
      }
      break;
    case '\n':
    case '\r':
      // in old v1.5 version, NMEA Data may not be checksummed!
      if (nmeaStatus == RECEIVING || nmeaStatus == CHECKSUMMING)
      {
        nmeaDataReady = true;
        nmeaStatus = TERMINATING;
      }
      else
        nmeaStatus = INVALID;

      break;
    }
    switch (nmeaStatus)
    {
    case INVALID:
      // do nothing
      nmeaIndex = 0;
      nmeaDataReady = false;
      break;
    case RECEIVING:
    case CHECKSUMMING:
      if( cIn>=32 && cIn <=126) // filter out non-printable characters, which can cause problems in the processing of the NMEA data
      {
      nmeaBuffer[nmeaIndex] = cIn;
      nmeaIndex++;
      } else 
      {
        nmeaStatus = INVALID; // if a non-printable character is found, the NMEA data is considered invalid and the state changes back to INVALID
        nmeaIndex = 0;
        nmeaDataReady = false;
      }
      if (nmeaIndex >= NMEA_BUFFER_SIZE)
      {
        nmeaStatus = TERMINATING;
        nmeaIndex = 0;
        nmeaDataReady = false;
      }
      break;
    case TERMINATING:

      nmeaStatus = INVALID;
      if (nmeaDataReady)
      {
        nmeaDataReady = false;

        // Clear the remaining buffer content with '\0'
        for (int y = nmeaIndex + 1; y < NMEA_BUFFER_SIZE + 1; y++)
        {
          nmeaBuffer[y] = '\0';
        }
#ifdef DEBUG
        debugWrite(nmeaBuffer);
#endif

        processNMEAData(nmeaBuffer);
        
        // clear the NMEAbuffer with 0
        memset(nmeaBuffer, 0, NMEA_BUFFER_SIZE + 1);
        nmeaIndex = 0;
      }

      break;
    }
  }
}

/*
 * Initializes UART 1 for incomming NMEA0183 data from the boat network
 */
bool initializeListener()
{
  bool status = true;
  int baudrate = lv_subject_get_int(&mfd_subject_baudrate);
  Serial1.begin(baudrate, SERIAL_8N1, LISTENER_RX, LISTENER_TX, true);
  memset(nmeaBuffer, 32, NMEA_BUFFER_SIZE - 1);
  nmeaBuffer[NMEA_BUFFER_SIZE] = '\0';

  lv_log("nmeaBuffer initialized with %s\n", nmeaBuffer);

#ifdef DEBUG
  debugWrite("Listener initialized...");
#endif
  return status;
}

/*
 * Initializes UART  for incomming NMEA0183 data from the boat network
 for now UART2 run at the same speed as UART1
 */
bool initializeTalker()
{
  bool status = true;
  int baudrate = lv_subject_get_int(&mfd_subject_baudrate);
  // Serial2.begin(baudrate, SERIAL_8N1, TALKER_RX, TALKER_TX, true);
  clearNMEAInputBuffer();
#ifdef DEBUG
  debugWrite("Talker initialized...");
#endif
  return status;
}

void mfd_setup_communication()
{

  initializeListener();
  // initializeTalker();
}
