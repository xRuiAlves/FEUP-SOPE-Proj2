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
#define WIDTH_NT        2

//Delay in request processing simulation
#include <unistd.h>
#define DELAY() usleep(50*1000);

#define MAX_MESSAGE_SIZE 512
#define CLOSED_WRITE_FIFO_WAIT_DELAY_MS     1
#define NON_BLOCKING_SEM_WAIT_DELAY_MS      1

// Stringification Macros
#define MACRO_STRINGIFY_(X) #X
#define MACRO_STRINGIFY(X)  MACRO_STRINGIFY_(X)

//Comms

#define REQUEST_FIFO_NAME           "requests"
#define CLIENT_ANSWER_PREFIX        "ans"

#define WIDTH_FIFO_NAME     (WIDTH_PID + 3)

// Logs Macros
#define BUFF_SIZE               8192
#define SLOG_OPENING_ERROR         2
#define SBOOK_OPENING_ERROR        3

#endif  // _DEFS_H_
