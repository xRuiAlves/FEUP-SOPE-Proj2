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
#define OUT -7
#define FIFO_CREATION_ERROR         1
#define CLOG_OPENING_ERROR          2
#define CLIENT_FIFO_OPENING_ERROR   3
#define SERVER_FIFO_OPENING_ERROR   4
#define MSG_BROADCAST_ERROR         5
#define CBOOK_OPENING_ERROR         6
#define READ_SERVER_ANS_ERROR       7
#define PARSE_SERVER_ANS_ERROR      8

// Stringification Macros
#define MACRO_STRINGIFY_(X) #X
#define MACRO_STRINGIFY(X)  MACRO_STRINGIFY_(X)

// Server Data Macros
#define SERVER_FIFO_NAME    "requests"
#define MAX_ROOM_SEATS      9999
#define MAX_CLI_SEATS       99
#define WIDTH_PID           5
#define WIDTH_FIFO_NAME     (WIDTH_PID + 3)
#define WIDTH_XX            2
#define WIDTH_NN            2
#define WIDTH_XXNN          (WIDTH_XX + WIDTH_NN + 1)
#define WIDTH_SEAT          4
#define MESSAGE_MAX_SIZE    4096
#define CLOG_MSG_MAX_SIZE   (WIDTH_PID + WIDTH_XXNN + WIDTH_SEAT + 3)


#endif  // _DEFS_H_
