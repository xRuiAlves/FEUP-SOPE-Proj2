#include "worker.h"
#include "seats.h"
#include "defs.h"
#include "synchronization.h"
#include "buffer.h"

#include <unistd.h>
#include <stdio.h>

static Seat seat_arr[MAX_ROOM_SEATS];

void * startWorking(void * args) {
    printf("Worker created\n");
    sleep(1);

    char * mydata;

    //Waits until there is data to read
    wait_has_data_sem();    
    mydata = read_buffer();
    //Signals that data can be written once again, for it has been read
    signal_can_send_data_sem();
    //Process data...

    printf("Worker exiting\n");
    return -1;
}