#include "synchronization.h"
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include "buffer.h"
#include <errno.h>
#include "defs.h"
#include "seats.h"

static sem_t has_data_sem;
static sem_t can_send_data_sem;
static pthread_mutex_t seats_mutex[MAX_ROOM_SEATS];
static pthread_mutex_t buffer_mutex;

int init_sync() {
    if(sem_init(&has_data_sem, 0, 0) != 0) {
        return -1;
    }

    if(sem_init(&can_send_data_sem, 0, BUFFER_LENGTH) != 0) {
        return -2;
    }
    
    int i;
    for(i = 0; i < getMaxPossibleSeatID(); ++i) {
        if(pthread_mutex_init(&seats_mutex[i], NULL) != 0) {
            return -3;
        }
    }

    if(pthread_mutex_init(&buffer_mutex, NULL) != 0) {
        return -4;
    }
    
    return 0;
}

int finish_sync() {
    if(sem_destroy(&has_data_sem) != 0) {
        return -1;
    }

    if(sem_destroy(&can_send_data_sem) != 0) {
        return -2;
    }

    //destroying mutex
    int i;
    for(i = 0; i < getMaxPossibleSeatID(); ++i) {
        if(pthread_mutex_destroy(&seats_mutex[i]) != 0) {
            return -3;
        }
    }

    if(pthread_mutex_destroy(&buffer_mutex) != 0) {
        return -4;
    }

    return 0;
}

void wait_has_data_sem() {
    if(sem_wait(&has_data_sem) != 0) {
        fprintf(stderr, "Error in wait for has data semaphore!\n");
    }
}

int try_wait_has_data_sem() {
    if(sem_trywait(&has_data_sem) == 0) {
        return 0;
    } else {
        if(errno == EAGAIN) {
            return 1;
        } else {
            return -1;
        }
    }
}

void signal_has_data_sem() {
    if(sem_post(&has_data_sem) != 0) {
        fprintf(stderr, "Error in signaling has data semaphore!\n");
    }
}

void wait_can_send_data_sem() {
    if(sem_wait(&can_send_data_sem) != 0) {
        fprintf(stderr, "Error in wait for can send data semaphore!\n");
    }
}

void signal_can_send_data_sem() {
    if(sem_post(&can_send_data_sem) != 0) {
        fprintf(stderr, "Error in signaling can send data semaphore!\n");
    }
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

int try_lock_buffer_mutex() {
    int ret_val;
    if((ret_val = pthread_mutex_trylock(&buffer_mutex)) != 0) {
        if(ret_val == EBUSY) {
            return 1;
        } else {
            fprintf(stderr, "Error in locking buffer mutex!\n");
            return -1;
        }
    }
    return 0;
}

void unlock_buffer_mutex() {
    if(pthread_mutex_unlock(&buffer_mutex) != 0) {
        fprintf(stderr, "Error in unlocking buffer mutex!\n");
    }
}