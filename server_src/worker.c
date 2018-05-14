#include "worker.h"
#include "seats.h"
#include "defs.h"
#include "synchronization.h"
#include "buffer.h"
#include "parser.h"
#include "communication.h"
#include "slog.h"

#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

static Seat seats[MAX_ROOM_SEATS];
static int worker_status = WORKER_RUNNING;
static unsigned int num_workers = 0;

static int parse_client_message(char * client_data, ClientMessage * cmessage);

void * startWorking(void * args) {
    unsigned int myID = ++num_workers;
    writeServerWorkerCreating(myID);

    char * mydata;
    ClientMessage cmess;
    int parse_status = 0;
    int sem_wait_status = 0;
    while(worker_status) {
        //Waits until there is data to read
        sem_wait_status = try_wait_has_data_sem();
        if(sem_wait_status == -1) {
            fprintf(stderr, "Error in call to try wait for has data sem!\n");
            return (void *) -1;
        }
        if(sem_wait_status == 1) {
            //Must wait for semaphore
            //Sleeping for a bit to reduce processing waste
            usleep(NON_BLOCKING_SEM_WAIT_DELAY_MS * 1000);
            //Continue to recheck loop condition
            continue;
        }
        mydata = read_buffer();
        //Signals that data can be written once again, for it has been read
        signal_can_send_data_sem();
        //Process data
        parse_status = parse_client_message(mydata, &cmess);
        if(parse_status != 0) {
            //Handle error response, reply to client if need be, etc
            if(parse_status < 0) {
                //"Replyable" error
                replyToClient_error(cmess.pid, parse_status);
                writetoServerLogError(cmess, myID, parse_status);
            } else {
                //"Unreplyable" error
                fprintf(stderr, "Critical error: Cannot reply to client...\n");
            }

            continue;
        }

    ////From now on we are processing a request so it must always be able to end, regardless if threads are shutting down

        //Client message has no errors, attempt request
        int i;
        int num_reserved_seats = 0;
        int seat_free_status;
        unsigned int reserved_seats[cmess.num_wanted_seats];
        //Requesting lock on seats mutex
        lock_seats_mutex();
        if(cmess.num_wanted_seats > getNrAvailableSeats()) {
            //Room would be full, FUL error
            replyToClient_error(cmess.pid, FUL);
            seat_free_status = -1;
            writetoServerLogError(cmess, myID, FUL);
        } else {
            for(i = 0; i < cmess.num_pref_seats; ++i) {
                if(num_reserved_seats == cmess.num_wanted_seats) {
                    break;
                }

                seat_free_status = isSeatFree(seats, cmess.pref_seats[i]);
                if(seat_free_status == 2) {
                    //Room is full, break
                    break;
                }

                if(seat_free_status == 1) {
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
        }
        unlock_seats_mutex();

        if(seat_free_status == -1) {
            //Error already dealt with
            continue;
        }

        if(seat_free_status == 2) {
            //Failure with FUL
            replyToClient_error(cmess.pid, FUL);
            writetoServerLogError(cmess, myID, FUL);
        } else if(num_reserved_seats < cmess.num_wanted_seats) {
            //Failure with NAV
            replyToClient_error(cmess.pid, NAV);
            writetoServerLogError(cmess, myID, NAV);
        } else {
            //Success
            replyToClient_success(cmess.pid, num_reserved_seats, reserved_seats);
            writetoServerLog(cmess, myID, num_reserved_seats, reserved_seats);
        }
    }

    writeServerWorkerClosing(myID);
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
    ///"<pid> <num_wanted_seats> <s1> <s2> ... <sN>\n"

    if(split_data_len < 3) {
        //3 is the minimum number of message fields for it to be valid
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
        error_status = ERR;
        goto free_and_exit;
    } else if(num_wanted_seats > MAX_CLI_SEATS) {
        error_status = MAX;
    }
    unsigned int num_pref_seats = split_data_len - 2;
    if(num_pref_seats < num_wanted_seats) {
        error_status = NST;
    }

    //Parsing the seat list itself
    int i;
    for(i = 0; i < num_pref_seats; ++i) {
        cmessage->pref_seats[i] = parse_unsigned_int(split_data[i+2]);
        if(cmessage->pref_seats[i] == UINT_MAX  || cmessage->pref_seats[i] == 0 || cmessage->pref_seats[i] > getPossibleMaxID()) {
            error_status = IID;
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
