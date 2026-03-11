# CHANGE LOG

##  V0.8.7 10-03-2026
Moved all static definitions from include files to .c or .cpp files to avoid multiple definition errors. 
This is a common practice in C/C++ programming to ensure that global variables are defined only once and 
can be accessed across multiple files without causing linker errors.\
Moved changelofg from main.ccp comments to changelog.md file\
### V0.8.6 LVGL updated to V9.5.0
Added demo-mode.\
This is useful for testing and demo purposes, without having to wait for real NMEA sentences to be received.
The demo mode can be enabled or disabled via a switch on the config panel. When enabled, the display will show
simulated data that changes over time, to mimic real sensor readings.\
### v0.8.4 07-03-2026:
Moved wind gauges from right to middle position on windpanel for better optical balance.\
Driving the display on 5V using the 5V & GND pin can't handle max brightness. On 52% brightness the
display draws 520mA. Driving the display with the USB-C from the laptop does not show this issue.\
Need to test with the Buck converter what is the right voltage to drive the display at max brightness.\
### V0.8.3 06-03-2026
Memory assertion seems to besolved by changing line 72 in l_conf.h, size of memory available
for `lv_malloc()` in bytes (>= 2kB) #define LV_MEM_SIZE (128 * 1024U) (used to 64 *124u by default).\
Two wind gauges added to the wind panel; one for apparent wind angele between -135 and +135 degrees and
and an apparent wind angle +, showing a zoomed view  between -60 and +60 degrees for higher precision.\
### V0.8.2 05-03-2026
A gauge tile added (this was a feature on the prio list).The gauge is a graphical representation of
the data, and can be used to display the wind angle for example. It is implemented as a separate tile type,
since it has a different layout and functionality than the regular tiles. It is not problem free yet since
it drains the memory.\
### V0.81 04-03-2026
MTW tag added to NMEA_TAG enum and implemented in mfd_calculation.cpp since it was forgotton to be added
in the previous version, and is used in the test data and is a common tag for water temperature.\
Fixed unreliable serial cable leading to unreadable data from the UART leading to long debugging sessions
Implemented filtering of non-printable characters in the serial data, which can cause problems in the processing
of the NMEA data and can lead to crashes or incorrect data being displayed.\
### V0.80 01-03-2026
FAT passed with real NMEA0183 data from the test network, and the display is working as expected.\
2-way communication crashed the program, so I have commented out the talker part for now, and will implement it in a later stage.\
I have added some comments to the code to make it more clear what is going on.\
### v0.6 28-02-2026
Implemented 2-way communication so that incomming NMEA data can be relayed
I re-used my state based function from the Yazz_NMEAtor_ESP32 project to read the
NMEA data from the serial port, and added a function to process the data when it is ready.\
fixed bugs, cleaned code, implemented version info on settings screen,
fixed toggle state for menubar, added spacer for panels when not all tiles are used
implemented Sun-, Dawn and Night modes.\
### V0.5  22-02-2026
fixed bugs, cleaned code, implemented version info on settings screen,
fixed toggle state for menubar, added spacer for panels when not all tiles are used
implemented Sun-, Dawn and Night modes.\
### v0.4 17-02-2026
Fixed toggle state. Cleaned dead code.\
Version info implemented on Setting screen.\
TO DO: Implement Sun-, Dawn and Night modes.\
### v0.3 15-02-2026
fixed bugs, cleaned code, spacer added for panels when not all tiles are used
CMG implemented \
TO DO: \
	* Implement Sun-, Dawn and Night modes\
	* fix toggle state for menubar
### v0.2 14-02-2026
Except from communication with hardwared NMEA0183 the program is functional.\
For testing and demo purposes it sends virtual NMEA0183 messages and these
are processed correctly.\
Persistent storage implemented and working.\
Sun-, Dawn and Night modes not functional.\
### v0.1 31-01-2026
1st working version of the UI without NMEA0183 data input, but with a working demo.\
