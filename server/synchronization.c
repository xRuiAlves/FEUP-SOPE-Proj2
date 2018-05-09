#include "synchronization.h"
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include "buffer.h"

static sem_t has_data_sem;
static sem_t can_send_data_sem;
static pthread_mutex_t seats_mutex;

int init_sync() {
    if(sem_init(&has_data_sem, 0, 0) != 0) {
        return -1;
    }

    if(sem_init(&can_send_data_sem, 0, BUFFER_LENGTH) != 0) {
        return -2;
    }
    
    if(pthread_mutex_init(&seats_mutex, NULL) != 0) {
        return -3;
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
    if(pthread_mutex_destroy(&seats_mutex) != 0) {
        return -3;
    }

    return 0;
}

void wait_has_data_sem() {
    if(sem_wait(&has_data_sem) != 0) {
        fprintf(stderr, "Error in wait for has data semaphore!\n");
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

void lock_seats_mutex() {
    if(pthread_mutex_lock(&seats_mutex) != 0 ) {
        fprintf(stderr, "Error in locking seats mutex!\n");
    }
}

void unlock_seats_mutex() {
    if(pthread_mutex_unlock(&seats_mutex) != 0) {
        fprintf(stderr, "Error in unlocking seats mutex!\n");
    }
}