#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "defs.h"
#include "client.h"
#include "limits.h"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        print_usage(stderr);
        exit(ERR);
    }

    // Parse pid to wanted length
    char pid[WIDTH_PID+1];
    snprintf(pid, WIDTH_PID+1, "%0" MACRO_STRINGIFY(WIDTH_PID) "d", getpid());

    // Parse timeout value
    u_int time_out = parse_unsigned_int(argv[1]);
    if (time_out == UINT_MAX || time_out == 0) {
        fprintf(stderr, "Error: time_out must be a positive integer value.\n");
        print_usage(stderr);
        exit(ERR);
    }

    // Parse num wanted wanted seats
    u_int num_wanted_seats = parse_unsigned_int(argv[2]);
    if (num_wanted_seats == UINT_MAX || num_wanted_seats == 0 || num_wanted_seats > MAX_ROOM_SEATS) {
        fprintf(stderr, "Error: the number of wanted seats must be in the range [1, %d].\n", MAX_ROOM_SEATS);
        print_usage(stderr);
        exit(ERR);
    }

    printf("%u\n", time_out);
    printf("%u\n", num_wanted_seats);

    return 0;
}

void print_usage(FILE* stream) {
    fprintf(stream, "usage: client <time_out> <num_wanted_seats> <pref_seat_list>\n");
}

u_int parse_unsigned_int(char* value) {
    long val = strtol(value, NULL, 10);

    if (val == LONG_MAX || val == LONG_MIN || val < 0) {
        return UINT_MAX;
    }
    else {
        return (u_int) val;
    }
}
