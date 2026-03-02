/**
 * @file mfd_communication.h
 */
#ifndef MFD_COMMUNICATION_H
#define MFD_COMMUNICATION_H

#include <HardwareSerial.h>
#include <NMEA0183_data.h>
#include <calc/mfd_calculation.h>
#include <mfd_conf.h>

#define SAMPLERATE 115200

// #define LISTENER_RATE 38400 // Baudrate for the listner, but now set through mfd_subject_baudrate
#define LISTENER_RX 32 // Serial1 Rx port
#define LISTENER_TX 33 // Serial1 TX port
// #define TALKER_RATE 38400   // Baudrate for the talker
#define TALKER_RX 22
#define TALKER_TX 23 // SoftSerial port 2

/**
 * The different states for the NMEA receiving process, used in the state machine in the decodeNMEAInput function
 */
enum NMEAReceiveStatus
{
  INVALID,
  VALID,
  RECEIVING,
  CHECKSUMMING,
  TERMINATING,
  NMEA_READY
};

static int nmeaStatus = INVALID;                    //*** state variable for the NMEA receiving process, initial state is INVALID
static int nmeaIndex = 0;                           //*** index variable for the NMEA receiving process, initial state is 0
static bool nmeaDataReady = false;                  //*** flag variable for the NMEA receiving process, initial state is false
static bool recvInProgress = false;                 //*** flag variable to indicate if the receiving process is in progress, initial state is false
static bool newData = false;                        // *** flag variable to indicate if new data is available, initial state is false
static char nmeaBuffer[NMEA_BUFFER_SIZE + 1] = {0}; //*** buffer variable to store the incoming NMEA data, initial state is an array of 0 with size NMEA_BUFFER_SIZE + 1

extern void mfd_setup_communication();

extern int NMEA_startTalking(const char *nmeaBuff);
extern void NMEA_startListening();
extern void decodeNMEAInput(char cIn);

#endif // MFD_COMMUNICATION_H
