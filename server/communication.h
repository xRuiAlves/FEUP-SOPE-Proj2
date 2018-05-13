#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include <stdlib.h>

int replyToClient_error(unsigned int pid, int error_status);

int replyToClient_success(unsigned int pid, size_t n_reserved_seats, unsigned int reserved_seats[]);

#endif  // _COMMUNICATION_H_