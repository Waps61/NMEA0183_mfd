/**
 * @file mfd_communication.h
 */
#ifndef MFD_COMMUNICATION_H
#define MFD_COMMUNICATION_H

#include <HardwareSerial.h>
#include <NMEA0183_data.h>
#include <calc/mfd_calculation.h>
#include <mfd_conf.h>


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

extern int nmeaStatus;                    //*** state variable for the NMEA receiving process, initial state is INVALID
extern int nmeaIndex;                           //*** index variable for the NMEA receiving process, initial state is 0
extern bool nmeaDataReady;                  //*** flag variable for the NMEA receiving process, initial state is false
extern bool recvInProgress;                 //*** flag variable to indicate if the receiving process is in progress, initial state is false
extern bool newData;                        // *** flag variable to indicate if new data is available, initial state is false
extern char nmeaBuffer[NMEA_BUFFER_SIZE + 1]; //*** buffer variable to store the incoming NMEA data, initial state is an array of 0 with size NMEA_BUFFER_SIZE + 1

extern void mfd_setup_communication();

extern int NMEA_startTalking(const char *nmeaBuff);
extern void NMEA_startListening();
extern void NMEA_runSoftGenerator();

#endif // MFD_COMMUNICATION_H
