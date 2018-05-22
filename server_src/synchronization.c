#include "synchronization.h"
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include "buffer.h"
#include <errno.h>
#include "defs.h"
#include "seats.h"

static pthread_mutex_t seats_mutex[MAX_ROOM_SEATS];
static pthread_mutex_t buffer_mutex;
static pthread_cond_t t_cond = PTHREAD_COND_INITIALIZER;

int init_sync() {
    int i;
    for(i = 0; i < getMaxPossibleSeatID(); ++i) {
        if(pthread_mutex_init(&seats_mutex[i], NULL) != 0) {
            return -1;
        }
    }

    if(pthread_mutex_init(&buffer_mutex, NULL) != 0) {
        return -2;
    }
    
    return 0;
}

int finish_sync() {
    //destroying mutex
    int i;
    for(i = 0; i < getMaxPossibleSeatID(); ++i) {
        if(pthread_mutex_destroy(&seats_mutex[i]) != 0) {
            return -1;
        }
    }

    int mutex_destroy_return;
    if((mutex_destroy_return = pthread_mutex_destroy(&buffer_mutex)) != 0) {
        if (mutex_destroy_return == EBUSY) {
            pthread_mutex_unlock(&buffer_mutex);
            pthread_mutex_destroy(&buffer_mutex);
        } else {
            return -2;
        }
    }

    return 0;
}

void lock_seats_mutex(unsigned int seatID) {
    if(pthread_mutex_lock(&seats_mutex[seatID]) != 0 ) {
        fprintf(stderr, "Error in locking seats mutex for seat ID %u!\n", seatID);
    }
}

void unlock_seats_mutex(unsigned int seatID) {
    if(pthread_mutex_unlock(&seats_mutex[seatID]) != 0) {
        fprintf(stderr, "Error in unlocking seats mutex for seat ID %u!\n", seatID);
    }
}

void wait_until_buffer_empty() {
    pthread_mutex_lock(&buffer_mutex);
}

void signal_buffer_full() {
    pthread_cond_signal(&t_cond);
    if(pthread_mutex_unlock(&buffer_mutex) != 0) {
        fprintf(stderr, "Error in unlocking buffer mutex!\n");
    }
}

int wait_until_buffer_full() {
    
    int ret_val;
    if((ret_val = pthread_mutex_trylock(&buffer_mutex)) != 0) {
        if(ret_val == EBUSY) {
            return 1;
        } else {
            fprintf(stderr, "Error in locking buffer mutex!\n");
            return -1;
        }
    }
    
    while(!is_buffer_full()) {
        pthread_cond_wait(&t_cond, &buffer_mutex);
    }

    return 0;
}

void signal_buffer_empty() {
    pthread_cond_signal(&t_cond);
    if(pthread_mutex_unlock(&buffer_mutex) != 0) {
        fprintf(stderr, "Error in unlocking buffer mutex!\n");
    }
}