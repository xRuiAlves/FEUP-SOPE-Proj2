#include "communication.h"
#include "defs.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int replyToClient_error(unsigned int pid, int error_status) {
    char fifo_name[WIDTH_FIFO_NAME + 1];
    snprintf(fifo_name, WIDTH_FIFO_NAME+1, CLIENT_ANSWER_PREFIX "%0" MACRO_STRINGIFY(WIDTH_PID) "u", pid);

    int fifo_fd = open(fifo_name, O_WRONLY);

    if(fifo_fd == -1) {
        //Error opening answer fifo, client probably timed out
        fprintf(stderr, "Client %u probably timed out\n", pid);
        return -1;
    }

    char * msg = NULL;

    if(asprintf(&msg, "%d\n", error_status) == -1) {
        fprintf(stderr, "Error in asprintf for replying to client!\n");
        return -2;
    }

    if(write(fifo_fd, msg, strlen(msg)) < 0) {
        fprintf(stderr, "Error in write, in replying to client with id %u\n", pid);
        return -3;
    }

    close(fifo_fd);
    free(msg);
    return 0;
}

int replyToClient_success(unsigned int pid, size_t n_reserved_seats, unsigned int reserved_seats[]) {
    if(n_reserved_seats <= 0) {
        return -1;
    }

    char fifo_name[WIDTH_FIFO_NAME + 1];
    snprintf(fifo_name, WIDTH_FIFO_NAME+1, CLIENT_ANSWER_PREFIX "%0" MACRO_STRINGIFY(WIDTH_PID) "u", pid);

    int fifo_fd = open(fifo_name, O_WRONLY);

    if(fifo_fd == -1) {
        //Error opening answer fifo, client probably timed out
        fprintf(stderr, "Client %u probably timed out\n", pid);
        return -2;
    }

    char msg[4096];
    msg[0] = '\0';
    char single_num[5];

    snprintf(single_num, 5, "%u", reserved_seats[0]);
    strncat(msg, single_num, 5);

    int i;
    for(i = 1; i < n_reserved_seats; ++i) {
        strcat(msg, " ");
        snprintf(single_num, 5, "%u", reserved_seats[0]);
        strncat(msg, single_num, 5);
    }

    if(write(fifo_fd, msg, strlen(msg)) < 0) {
        fprintf(stderr, "Error in write, in replying to client with id %u\n", pid);
        return -3;
    }

    close(fifo_fd);
    return 0;
}