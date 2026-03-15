/**
 * @file mfd_communication.cpp
 */
#include <Arduino.h>
/*
  Project:  NMEA0183_mfd, Copyright 2026, Roy Wassili
  Contact:  waps61 @gmail.com
/*
    This is the NMEA communication unit for receiving and sending NMEA data
    Include the necessary libraries based on the Yazz_Winddisplay project from 2020
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

HardwareSerial nmeaIn(1);
HardwareSerial nmeaOut(2);

/**
 * Test data set if nog real data is available
 */
#define SIZE_NMEA_STREAM 22
String NmeaStream[SIZE_NMEA_STREAM] = {
    "$GPRMC,095218.000,A,5251.5621,N,00540.8482,E,5.1,201.77,120420,,,D*6D",
    "$IIVWR,25,R,12.4,N,,,,",
    "$TZBWC,095218.000,5251.3071,N,00541.7231,E,251.0,M,0.05,N,,S*7F",
    "$IIMTW,12.2,C",
    "!$SDDPT,004.3,*78",
    "$IIDBK,A,0014.4,f,,,,",
    "$IIVLW,1149.1,N,001.07,N",
    "$GPGLL,5251.3091,N,00541.8037,E,151314.000,A,D*5B",
    "$PSTOB,13.0,v",
    "$IIVWR,28,R,02.3,N,,,,",
    "$TZBWC,095318.000,5251.3171,N,00541.7331,E,250.0,M,0.05,N,,S*7F",
    "$IIVHW,,,213,M,04.57,N,,",
    "$GPRMC,095318.000,A,5251.6721,N,00540.9582,E,5.3,203.35,120420,,,D*6D",
    "$IIVWR,30,R,13.3,N,,,,",
    "$IIMTW,12.2,C",
    "!$SDDPT,004.2,*78",
    "$IIDBK,A,0014.7,f,,,,",
    "$IIVLW,1151.1,N,002.07,N",
    "$GPGLL,5251.3191,N,00541.8137,E,151314.000,A,D*5B",
    "$PSTOB,13.1,v",
    "$IIVWR,26,R,12.7,N,,,,",
    "$IIVHW,,,210,M,05.57,N,,"};

/**
 * Helper function to generate NMEA data streams for testing if
 * no NMEA data is available from talkers
 */
int softIndex = 0;
long softTimerNow = millis();

void NMEA_runSoftGenerator()
{
  if (millis() - softTimerNow > 25)
  {
    softTimerNow = millis();

    // lv_log("sending msg %d data=%s\n", softIndex, NmeaStream[softIndex].c_str());
    processNMEAData(NmeaStream[softIndex++].c_str());
    // Due to low output voltage of 0.65V on the TX port it is below the trheshold of 1,5V
    // and receivers wont's see any data. So for know sending NMEA data is disbaled.
    //NMEA_startTalking(NmeaStream[softIndex++].c_str());
    softIndex %= SIZE_NMEA_STREAM;
  }
}

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
  while (nmeaIn.available() > 0)
  {
    nmeaIn.read();
  }
}

int NMEA_startTalking(const char *nmea_buff)
{
  int i = 0;
  while (nmea_buff[i] > 0)
  {
    
    nmeaOut.write(nmea_buff[i++]);
  }
  lv_log("%s\n", nmea_buff);
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
  while (nmeaIn.available() > 0 && nmeaStatus != TERMINATING)
  {
    cIn = nmeaIn.read();
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
      if (cIn >= 32 && cIn <= 126) // filter out non-printable characters, which can cause problems in the processing of the NMEA data
      {
        nmeaBuffer[nmeaIndex] = cIn;
        nmeaIndex++;
      }
      else
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
        // Due to low output voltage of 0.65V on the TX port it is below the trheshold of 1,5V
        // and receivers wont's see any data. So for know sending NMEA data is disbaled.
        //NMEA_startTalking(nmeaBuffer);
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
  nmeaIn.begin(baudrate, SERIAL_8N1, LISTENER_RX, LISTENER_TX, true);
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
  nmeaOut.begin(baudrate, SERIAL_8N1, TALKER_RX, TALKER_TX, true);

#ifdef DEBUG
  debugWrite("Talker initialized...");
#endif
  return status;
}

void mfd_setup_communication()
{

  initializeListener();
  // Due to low output voltage of 0.65V on the TX port it is below the trheshold of 1,5V
  // and receivers wont's see any data. So for know sending NMEA data is disbaled.
  //initializeTalker();
}
