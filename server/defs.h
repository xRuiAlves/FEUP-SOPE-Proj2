#ifndef _DEFS_H_
#define _DEFS_H_

// Error Macros
#define MAX -1 /*Number of desired seats is bigger than MAX_CLI_SEATS*/
#define NST -2 /*Invalid number of desired seats*/
#define IID -3 /*Invalid desired seat identifier*/
#define ERR -4 /*Other parameter error*/
#define NAV -5 /*At least one of the desired seats cannot be placed, seat not available*/
#define FUL -6 /*Room full*/

// Server Data Macros
#define MAX_ROOM_SEATS  9999
#define MAX_CLI_SEATS   99
#define WIDTH_PID       5
#define WIDTH_XXNN      5
#define WIDTH_SEAT      4

//Delay in request processing simulation
#include <unistd.h>
#define DELAY() usleep(200*1000);

#define MAX_MESSAGE_SIZE 512
#define CLOSED_WRITE_FIFO_WAIT_DELAY_MS 200

#endif  // _DEFS_H_