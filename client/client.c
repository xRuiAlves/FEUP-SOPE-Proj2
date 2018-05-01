#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "defs.h"
#include "client.h"
#include "limits.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        print_usage(stderr);
        exit(ERR);
    }

    // Parse pid to wanted length
    char pid[WIDTH_PID+1];
    snprintf(pid, WIDTH_PID+1, "%0" MACRO_STRINGIFY(WIDTH_PID) "d", getpid());

    // Parse parameters
    u_int time_out = parse_time_out_val(argv[1]);
    u_int num_wanted_seats = parse_num_wanted_seats(argv[2]);
    u_int* parsed_pref_seat_list = (u_int*) malloc(MAX_ROOM_SEATS);
    u_int num_pref_seats = parse_pref_seat_list(argv[3], parsed_pref_seat_list, num_wanted_seats);

    printf("Timeout value: %u\n", time_out);
    printf("Number of Wanted Seats: %u\n", num_wanted_seats);
    printf("Prefered seats list:\n");
    int i;
    for (i=0 ; i<num_pref_seats ; i++) {
        printf("- %u\n",  parsed_pref_seat_list[i]);
    }

    return 0;
}

u_int parse_time_out_val(char* time_out_str) {
    u_int time_out = parse_unsigned_int(time_out_str);
    if (time_out == UINT_MAX || time_out == 0) {
        fprintf(stderr, "Error: time_out must be a positive integer value.\n");
        print_usage(stderr);
        exit(ERR);  // Parameter error
    } else {
        return time_out;
    }
}

u_int parse_num_wanted_seats(char* num_wanted_seats_str) {
    u_int num_wanted_seats = parse_unsigned_int(num_wanted_seats_str);

    if (num_wanted_seats == UINT_MAX) {
        fprintf(stderr, "Error: the number of wanted seats must be an integer in the range [1, %d].\n", MAX_CLI_SEATS);
        print_usage(stderr);
        exit(ERR);  // Parameter error
    } else  if (num_wanted_seats == 0 || num_wanted_seats > MAX_CLI_SEATS ){
        fprintf(stderr, "Error: the number of wanted seats must be an integer in the range [1, %d].\n", MAX_CLI_SEATS);
        print_usage(stderr);
        exit(MAX);  // Invalid number of wanted seats
    } else {
        return num_wanted_seats;
    }
}

u_int parse_unsigned_int(char* str) {
    errno = 0;
    char* endptr = NULL;
    long val = strtol(str, &endptr, 10);

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN || val < 0)) ||
        (errno != 0 && val == 0) ||
        (str == endptr)) {

        return UINT_MAX;
    }
    else {
        return (u_int) val;
    }
}

int parse_pref_seat_list(char* pref_seat_list, u_int* parsed_pref_seat_list, u_int num_wanted_seats) {
    char* seat;
    char* rest = strdup(pref_seat_list);
    u_int num_pref_seats = 0;

    // Count the number of prefered seats entered by the user
    while( (seat = strtok_r(rest, " ", &rest)) ) {
        num_pref_seats++;
    }

    // Verify number of prefered seats "correctness"
    if (num_pref_seats < num_wanted_seats || num_pref_seats > MAX_ROOM_SEATS) {
        fprintf(stderr, "Error: the number of specified prefered seats identifiers must be an integer in the range [num_wanted_seats, %d].\n", MAX_ROOM_SEATS);
        print_usage(stderr);
        exit(NST);  // Invalid number of prefered seats identifiers
    }

    // Parse the prefered seats list
    parsed_pref_seat_list = (u_int *) realloc(parsed_pref_seat_list, num_pref_seats*sizeof(u_int));
    u_int parsed_seats_counter = 0;
    rest = strdup(pref_seat_list);

    while( (seat = strtok_r(rest, " ", &rest)) ) {
        parsed_pref_seat_list[parsed_seats_counter] = parse_unsigned_int(seat);

        // Verify value correctness
        if (parsed_pref_seat_list[parsed_seats_counter] == UINT_MAX || parsed_pref_seat_list[parsed_seats_counter] == 0) {
            fprintf(stderr, "Error: Invalid prefered seat identifier - \"%s\"\n", seat);
            print_usage(stderr);
            exit(IID);  // Invalid number of prefered seats identifiers
        }

        parsed_seats_counter++;
    }

    int i=0;
    for (i=0 ; i<num_pref_seats ; i++) {
        printf("%d\n", parsed_pref_seat_list[i]);
    }

    return num_pref_seats;
}

void print_usage(FILE* stream) {
    fprintf(stream, "usage: client <time_out> <num_wanted_seats> <pref_seat_list>\n");
}
