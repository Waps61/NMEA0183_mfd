/**
 * @file mfd_communication.h
 */
#ifndef MFD_COMMUNICATION_H
#define MFD_COMMUNICATION_H

#include <HardwareSerial.h>
//#include <SPI.h>
//#include <SD.h>
#include <NMEA0183_data.h>
#include <calc/mfd_calculation.h>
#include <mfd_conf.h>

//*** Since the signal from the RS422-TTL converter is inverted
//*** a digital input is used as a software serial port because
//*** it can invert te signal back to its orignal pulse set
// #include <SoftwareSerial.h>
#define SAMPLERATE 115200

// #define LISTENER_RATE 38400 // Baudrate for the listner, but now set through mfd_subject_baudrate
#define LISTENER_RX 32      // Serial1 Rx port
#define LISTENER_TX 33      // Serial1 TX port
//#define TALKER_RATE 38400   // Baudrate for the talker
#define TALKER_RX 22
#define TALKER_TX 23 // SoftSerial port 2

enum NMEAReceiveStatus
{
  INVALID,
  VALID,
  RECEIVING,
  CHECKSUMMING,
  TERMINATING,
  NMEA_READY
};

static int nmeaStatus = INVALID;
static int nmeaIndex = 0;
static bool nmeaDataReady = false;
static bool recvInProgress = false;
static bool newData = false;
static char nmeaBuffer[NMEA_BUFFER_SIZE + 1]= {0};
static char receivedChars[NMEA_BUFFER_SIZE + 1]= {0};
//static String receivedChars = "";
extern void mfd_setup_communication();

extern int NMEA_startTalking(const char *nmeaBuff);
extern void NMEA_startListening();
extern void decodeNMEAInput(char cIn);

#endif // MFD_COMMUNICATION_H
