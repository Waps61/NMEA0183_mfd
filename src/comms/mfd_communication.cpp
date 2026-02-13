#include <Arduino.h>
/*
  Project:  Yazz_NMEAtor_ESP32.cpp, Copyright 2020, Roy Wassili
  Contact:  waps61 @gmail.com
/*
    This is the NMEA comminication unit for receiving and sending NMEA data
    Include the necessary libraries
*/
#include <comms/mfd_communication.h>

/*
   Definitions go here
*/
// *** Conditional Debug & Test Info to Serial Monitor
// *** by commenting out the line(s) below the debugger and or test statements will
// *** be ommitted from the code
// #define DEBUG 1
//#define TEST 1

/*
   If there is some special treatment needed for some NMEA sentences then
   add the their definitions to the NMEA_SPECIALTY definition
   The pre-compiler concatenates string literals by using "" in between
*/

unsigned long tmr1 = 0;

// SoftwareSerial nmeaSerialOut; // // signal need to be inverted for RS-232

// ----- software timer
unsigned long Timer2 = 1000000; // 500000L;                         // 500mS loop ... used when sending data to to Processing
unsigned long Stop2 = 0;

bool on = true;
byte pin = 22;

bool updateDisplay = false;

// bool newData = false;
// unsigned long tmr1 = 0;

/*
 * Setting for Serial interrupt
 */
//*** flag data on the listener port is ready
volatile bool listenerDataReady = false;

//*** ISR to set listerDataReady flag
void listenerReady()
{
  listenerDataReady = true;
}

/*
debugWrite() <--provides basic debug info from other tasks
takes a String as input parameter
*/
void debugWrite(String debugMsg)
{
#ifdef DEBUG
  if (debugMsg.length() > 1)
    Serial.println(debugMsg);
  else
    Serial.print(debugMsg);
#endif
}

/*
   Class definitions go here
*/
/*
Cleasr the inputbuffer by reading until empty, since Serial.flush does not this anymore
*/
void clearNMEAInputBuffer()
{

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

/** reads the softseroal port pin 10 and ckeks for valid nmea data starting with
 * character '$' only (~ and ! can be skipped as start charcter)
 */
void NMEA_startListening()
{
  static bool recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '$'; // we can skip ~ and ! starters since we don't process them here
  // char aisMarker = '!';
  // char rsvMarker = '~';
  char endMarker = '\n';
  char rc;
  newData = false;
  while (Serial1.available() > 0 && newData == false)
  {
    rc = Serial1.read();
    if (recvInProgress == true)
    {
      if (rc != endMarker)
      {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= NMEA_BUFFER_SIZE)
        {
          ndx = NMEA_BUFFER_SIZE - 1;
        }
      }
      else
      {
        receivedChars[ndx] = '\0'; // terminate the string

        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker)
    {
      receivedChars[ndx] = rc;
      ndx++;
      recvInProgress = true;
    }
  }
  NMEA_startTalking(receivedChars);
}

// Need to look into my older process neme data routine as a replacement for the one above
/*Decode the incomming character and test if it is valid NMEA data.If true than put it the NMEA buffer and call NMEAParser object
        to process incomming and complete MNEA sentence
            * /
//     void decodeNMEAInput(char cIn)
// {
//   switch (cIn)
//   {
//   case '~':
//     // reserved by NMEA
//   case '!':
//     // for AIS info
//   case '$':
//     // for general NMEA info
//     nmeaStatus = RECEIVING;
//     nmeaIndex = 0;
//     break;
//   case '*':
//     if (nmeaStatus == RECEIVING)
//     {
//       nmeaStatus = CHECKSUMMING;
//     }
//     break;
//   case '\n':
//   case '\r':
//     // in old v1.5 version, NMEA Data may not be checksummed!
//     if (nmeaStatus == RECEIVING || nmeaStatus == CHECKSUMMING)
//     {
//       nmeaDataReady = true;
//       nmeaStatus = TERMINATING;
//     }
//     else
//       nmeaStatus = INVALID;

//     break;
//   }
//   switch (nmeaStatus)
//   {
//   case INVALID:
//     // do nothing
//     nmeaIndex = 0;
//     nmeaDataReady = false;
//     break;
//   case RECEIVING:
//   case CHECKSUMMING:
//     nmeaBuffer[nmeaIndex] = cIn;
//     nmeaIndex++;
//     break;
//   case TERMINATING:

//     nmeaStatus = INVALID;
//     if (nmeaDataReady)
//     {
//       nmeaDataReady = false;

//       // Clear the remaining buffer content with '\0'
//       for (int y = nmeaIndex + 1; y < NMEA_BUFFER_SIZE + 1; y++)
//       {
//         nmeaBuffer[y] = '\0';
//       }
// #ifdef DEBUG
//       debugWrite(nmeaBuffer);
// #endif
//       NmeaParser.parseNMEASentence(nmeaBuffer);

//       // clear the NMEAbuffer with 0
//       memset(nmeaBuffer, 0, NMEA_BUFFER_SIZE + 1);
//       nmeaIndex = 0;
//     }

//     break;
//   }
// }



/*
 * Initializes UART 2 for incomming NMEA0183 data from the boat network
 */
bool initializeListener()
{
  bool status = true;
  Serial1.begin(LISTENER_RATE, SERIAL_8N1, LISTENER_RX, LISTENER_TX, true);
  clearNMEAInputBuffer();
#ifdef DEBUG
  debugWrite("Listener initialized...");
#endif
  return status;
}

/*
 * Initializes UART 2 for incomming NMEA0183 data from the boat network
 */
bool initializeTalker()
{
  bool status = true;
  Serial2.begin(LISTENER_RATE, SERIAL_8N1, TALKER_RX, TALKER_TX, true);
  clearNMEAInputBuffer();
#ifdef DEBUG
  debugWrite("Listener initialized...");
#endif
  return status;
}

// /*
//   Decode the incomming character and test if it is valid NMEA data.
//   If true than put it the NMEA buffer and call NMEAParser object
//   to process incomming and complete MNEA sentence
// */
// void decodeNMEAInput(char cIn)
// {
//   switch (cIn)
//   {
//   case '~':
//     // reserved by NMEA
//   case '!':
//     // for AIS info
//   case '$':
//     // for general NMEA info
//     nmeaStatus = RECEIVING;
//     nmeaIndex = 0;
//     break;
//   case '*':
//     if (nmeaStatus == RECEIVING)
//     {
//       nmeaStatus = CHECKSUMMING;
//     }
//     break;
//   case '\n':
//   case '\r':
//     // in old v1.5 version, NMEA Data may not be checksummed!
//     if (nmeaStatus == RECEIVING || nmeaStatus == CHECKSUMMING)
//     {
//       nmeaDataReady = true;
//       nmeaStatus = TERMINATING;
//     }
//     else
//       nmeaStatus = INVALID;
//     break;
//   }
//   switch (nmeaStatus)
//   {
//   case INVALID:
//     // do nothing
//     nmeaIndex = 0;
//     nmeaDataReady = false;
//     break;
//   case RECEIVING:
//   case CHECKSUMMING:
//     NMEA_DATA_STORE[nmeaIndex] = cIn;
//     nmeaIndex++;
//     break;
//   case TERMINATING:
//     nmeaStatus = INVALID;
//     if (nmeaDataReady)
//     {
//       nmeaDataReady = false;
//       // Clear the remaining buffer content with '\0'
//       for (int y = nmeaIndex + 1; y < NMEA_BUFFER_SIZE + 1; y++)
//       {
//         NMEA_DATA_STORE[y] = '\0';
//       }
// #ifdef DEBUG
//       debugWrite(NMEA_DATA_STORE);
// #endif
//       NmeaParser.parseNMEASentence(NMEA_DATA_STORE);
//       // clear the NMEA_DATA_STORE with 0
//       memset(NMEA_DATA_STORE, 0, NMEA_BUFFER_SIZE + 1);
//       nmeaIndex = 0;
//     }
//     break;
//   }
// }

// /*
//  * Start listeneing for incomming NNMEA sentences
//  */
// void NMEA_tartListening()
// {
// #ifdef DEBUG
//   debugWrite("Listening....");
// #endif

//   while (Serial1.available() > 0 && nmeaStatus != TERMINATING)
//   {
//     decodeNMEAInput(Serial1.read());
//   }
// }



void mfd_setup_communication()
{

  initializeListener();
  initializeTalker();
}
