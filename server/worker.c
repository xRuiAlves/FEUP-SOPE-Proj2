#include "worker.h"
#include "seats.h"
#include "defs.h"

#include <unistd.h>
#include <stdio.h>

static Seat seat_arr[MAX_ROOM_SEATS];

void * startWorking(void * args) {
    printf("Worker created\n");
    sleep(1);
    printf("Worker exiting\n");
    return -1;
}