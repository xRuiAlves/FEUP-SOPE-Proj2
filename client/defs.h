#ifndef _DEFS_H_
#define _DEFS_H_

// Useful typedefs
typedef unsigned int u_int;
typedef unsigned long u_long;

// Error Macros
#define MAX -1
#define NST -2
#define IID -3
#define ERR -4
#define NAV -5
#define FUL -6
#define FIFO_CREATION_ERROR 1

// Stringification Macros
#define MACRO_STRINGIFY_(X) #X
#define MACRO_STRINGIFY(X) MACRO_STRINGIFY_(X)

// Server Data Macros
#define MAX_ROOM_SEATS  9999
#define MAX_CLI_SEATS   99
#define WIDTH_PID       5
#define WIDTH_FIFO_NAME (WIDTH_PID + 3)
#define WIDTH_XX        2
#define WIDTH_NN        2
#define WIDTH_XXNN      (WIDTH_XX + WIDTH_NN + 1)
#define WIDTH_SEAT      4


#endif  // _DEFS_H_
