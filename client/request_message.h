#ifndef _REQUEST_MESSAGE_H_
#define _REQUEST_MESSAGE_H_

#include "defs.h"

typedef struct RequestMessage_struct {
    u_int pid;
    u_int num_wanted_seats;
    u_int* pref_seat_list;
    u_int num_pref_seats;
} RequestMessage;



#endif  // _REQUEST_MESSAGE_H_
