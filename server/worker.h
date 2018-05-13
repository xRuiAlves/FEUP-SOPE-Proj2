#ifndef _WORKER_H_
#define _WORKER_H_
#include "defs.h"

#define WORKER_RUNNING      1
#define WORKER_STOP         0

typedef struct {
    unsigned int pid;
    unsigned int num_wanted_seats;
    unsigned int num_pref_seats;
    unsigned int pref_seats[MAX_CLI_SEATS];
} ClientMessage;

void * startWorking(void * args);

void set_worker_status(int status);

#endif  // _WORKER_H_
