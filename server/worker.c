#include "worker.h"
#include "seats.h"
#include "defs.h"
#include "synchronization.h"
#include "buffer.h"
#include "parser.h"
#include <limits.h>

#include <unistd.h>
#include <stdio.h>

static Seat seats[MAX_ROOM_SEATS];
static int worker_status = WORKER_RUNNING;

typedef struct {
    unsigned int pid;
    unsigned int num_wanted_seats;
    unsigned int num_pref_seats;
    unsigned int pref_seats[MAX_CLI_SEATS];
} ClientMessage;

static int parse_client_message(char * client_data, ClientMessage * cmessage);

void * startWorking(void * args) {
    printf("Worker created\n");

    char * mydata;
    ClientMessage cmess;
    int parse_status = 0;
    while(worker_status) {
        //Waits until there is data to read
        wait_has_data_sem();
        mydata = read_buffer();
        //Signals that data can be written once again, for it has been read
        signal_can_send_data_sem();
        //Process data
        parse_status = parse_client_message(mydata, &cmess);
        if(parse_status != 0) {
            printf("Parsing of client message failed with status %d\n", parse_status);
            //Handle error response, etc
            continue;
        }

        //Client message has no errors, attempt request
        int i;
        int num_reserved_seats = 0;
        unsigned int reserved_seats[cmess.num_wanted_seats];
        //Requesting lock on seats mutex
        lock_seats_mutex();
        for(i = 0; i < cmess.num_pref_seats; ++i) {
            if(num_reserved_seats == cmess.num_wanted_seats) {
                break;
            }

            if(isSeatFree(seats, cmess.pref_seats[i]) == 1) {
                //Seat is free, book it
                bookSeat(seats, cmess.pref_seats[i], cmess.pid);
                reserved_seats[num_reserved_seats++] = cmess.pref_seats[i];
            }
        }
        if(num_reserved_seats < cmess.num_wanted_seats) {
            //Request failed, unbook seats
            for(i = 0; i < num_reserved_seats; ++i) {
                freeSeat(seats, reserved_seats[i]);
            }
        }
        unlock_seats_mutex();

        if(num_reserved_seats < cmess.num_wanted_seats) {
            printf("At least one wanted seat could not be booked\n");
            //Failure with NAV
            //Write response
        } else {
            printf("All seats were booked successfully!\n");
            //Success
            //Write response
        }
    }

    printf("Worker exiting\n");
    return NULL;
}

void set_worker_status(int status) {
    worker_status = status;
}

static int parse_client_message(char * client_data, ClientMessage * cmessage) {
    char ** split_data;
    size_t split_data_len;
    int error_status = 0;

    if(split_string(client_data, " ", &split_data, &split_data_len) != 0) {
        //If this ocurrs it is not possible to reply to the client!
        error_status = 1;
        //Even if the call failed in the middle of splitting we must deallocate memory to prevent leaks
        goto free_and_exit;
    }

    ////Format of client message is:
    ///"<pid> <num_wanted_seats> <num_pref_seats> <s1> <s2> ... <sN>\n"

    if(split_data_len < 4) {
        //4 is the minimum number of message fields for it to be valid
        error_status = ERR;
        goto free_and_exit;
    }

    unsigned int pid = parse_unsigned_int(split_data[0]);
    if(pid == UINT_MAX) {
        //If this ocurrs it is not possible to reply to the client!
        error_status = 1;
        goto free_and_exit;
    }
    //So that we are able to reply in case of failure for other reasons
    cmessage->pid = pid;
    unsigned int num_wanted_seats = parse_unsigned_int(split_data[1]);
    if(num_wanted_seats == UINT_MAX || num_wanted_seats == 0) {
        error_status = NST;
        goto free_and_exit;
    } else if(num_wanted_seats > MAX_CLI_SEATS) {
        error_status = MAX;
        goto free_and_exit;
    }
    unsigned int num_pref_seats = parse_unsigned_int(split_data[2]);
    if(num_pref_seats == UINT_MAX || num_pref_seats == 0) {
        error_status = ERR;
        goto free_and_exit;
    }

    if(split_data_len - 3 != num_pref_seats) {
        //Seat list does not correspond to passed number of preferred seats
        error_status = ERR;
        goto free_and_exit;
    }

    //Parsing the seat list itself
    int i;
    for(i = 0; i < num_pref_seats; ++i) {
        cmessage->pref_seats[i] = parse_unsigned_int(split_data[i+3]);
        if(cmessage->pref_seats[i] == UINT_MAX || cmessage->pref_seats[i] > MAX_ROOM_SEATS) {
            //TODO: Verify if being bigger than maxroomseats is IID, ERR or NAV
            error_status = IID;
            goto free_and_exit;
        }
    }

    //Setting the remaining values
    cmessage->num_wanted_seats = num_wanted_seats;
    cmessage->num_pref_seats = num_pref_seats;

free_and_exit:
    //Deallocating the split data
    for(i = 0; i < split_data_len; ++i) {
       free(split_data[i]);
    }
    free(split_data);

    return error_status;
}