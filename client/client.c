#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include "defs.h"
#include "client.h"
#include "request_message.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        print_usage(stderr);
        exit(ERR);
    }

    // Parse parameters
    u_int time_out = parse_time_out_val(argv[1]);
    u_int num_wanted_seats = parse_num_wanted_seats(argv[2]);
    u_int* parsed_pref_seat_list = (u_int*) malloc(MAX_ROOM_SEATS);
    u_int num_pref_seats = parse_pref_seat_list(argv[3], parsed_pref_seat_list, num_wanted_seats);

    // Parse pid to wanted length and create FIFO
    char pid[WIDTH_PID+1];
    char fifo_name[WIDTH_FIFO_NAME] = "ans";
    snprintf(pid, WIDTH_PID+1, "%0" MACRO_STRINGIFY(WIDTH_PID) "d", getpid());
    strncat(fifo_name, pid, WIDTH_PID);

    if (mkfifo(fifo_name, 0660) != 0) {
        fprintf(stderr, "Error: failed to create fifo.\n");
        exit(FIFO_CREATION_ERROR);
    }

    // Create request message
    RequestMessage msg = create_request_message(getpid(), num_wanted_seats, parsed_pref_seat_list, num_pref_seats);

    printf("Timeout value: %u\n", time_out);
    printf("Number of Wanted Seats: %u\n", msg.num_wanted_seats);
    printf("PID: %u\n", msg.pid);
    printf("Number of prefered seats: %u\n", msg.num_pref_seats);
    printf("Prefered Seats:\n");
    int i;
    for (i=0 ; i<msg.num_pref_seats ; i++) {
        printf("\t%u\n",  msg.pref_seat_list[i]);
    }

    return 0;
}

u_int parse_time_out_val(char* time_out_str) {
    u_int time_out = parse_unsigned_int(time_out_str);
    if (time_out == UINT_MAX || time_out == 0) {
        fprintf(stderr, "Error: The timeout value must be a positive integer value.\n");
        print_usage(stderr);
        exit(ERR);  // Parameter error
    } else {
        return time_out;
    }
}

u_int parse_num_wanted_seats(char* num_wanted_seats_str) {
    u_int num_wanted_seats = parse_unsigned_int(num_wanted_seats_str);

    if (num_wanted_seats == UINT_MAX) {
        fprintf(stderr, "Error: The number of wanted seats must be a positive integer value.\n");
        print_usage(stderr);
        exit(ERR);  // Parameter error
    } else {
        return num_wanted_seats;
    }
}

u_int parse_pref_seat_list(char* pref_seat_list, u_int* parsed_pref_seat_list, u_int num_wanted_seats) {
    char* seat;
    char* rest = strdup(pref_seat_list);
    u_int num_pref_seats = 0;

    // Count the number of prefered seats entered by the user
    while( (seat = strtok_r(rest, " ", &rest)) ) {
        num_pref_seats++;
    }

    // Parse the prefered seats list
    parsed_pref_seat_list = (u_int *) realloc(parsed_pref_seat_list, num_pref_seats*sizeof(u_int));
    u_int parsed_seats_counter = 0;
    rest = strdup(pref_seat_list);

    while( (seat = strtok_r(rest, " ", &rest)) ) {
        parsed_pref_seat_list[parsed_seats_counter] = parse_unsigned_int(seat);

        // Verify if value is a number
        if (parsed_pref_seat_list[parsed_seats_counter] == UINT_MAX) {
            fprintf(stderr, "Error: The seat identifiers must be positive integer values.\n");
            print_usage(stderr);
            exit(ERR);  // Invalid number of prefered seats identifiers
        }

        parsed_seats_counter++;
    }

    return num_pref_seats;
}

u_int parse_unsigned_int(char* str) {
    errno = 0;
    char* endptr = NULL;
    long val = strtol(str, &endptr, 10);

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) ||
        (val < 0) ||
        (errno != 0 && val == 0) ||
        (str == endptr)) {

        return UINT_MAX;
    }
    else {
        return (u_int) val;
    }
}

void print_usage(FILE* stream) {
    fprintf(stream, "usage: client <time_out> <num_wanted_seats> <pref_seat_list>\n");
}
