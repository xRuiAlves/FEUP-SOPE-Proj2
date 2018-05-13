#include "request_message.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

RequestMessage create_request_message(u_int pid, u_int num_wanted_seats, u_int* pref_seat_list, u_int num_pref_seats) {
    RequestMessage msg;

    msg.pid = pid;
    msg.num_wanted_seats = num_wanted_seats;
    msg.pref_seat_list = pref_seat_list;
    msg.num_pref_seats = num_pref_seats;

    return msg;
}

void broadcast_message(RequestMessage msg) {
    // Open the server requests fifo
    int server_fifo = open(SERVER_FIFO_NAME, O_WRONLY | O_APPEND);
    if (server_fifo < 0) {
        fprintf(stderr, "Error: Failed to open server requests fifo for writting.\n");
        exit(SERVER_FIFO_OPENING_ERROR);
    }

    // Prepare message
    char msg_str[MESSAGE_MAX_SIZE];
    char num_str[12];
    int i;

    sprintf(num_str, "%u ", msg.pid);
    strcat(msg_str, num_str);
    sprintf(num_str, "%u", msg.num_wanted_seats);
    strcat(msg_str, num_str);
    for (i=0 ; i<msg.num_pref_seats ; i++) {
        sprintf(num_str, " %u", msg.pref_seat_list[i]);
        strcat(msg_str, num_str);
    }
    strcat(msg_str, "\n");

    // Write message
    int write_res = write(server_fifo, msg_str, strlen(msg_str));
    if (write_res < 0) {
        fprintf(stderr, "Error: Failed to broadcast message to server requests fifo.\n");
        exit(MSG_BROADCAST_ERROR);
    }
}
