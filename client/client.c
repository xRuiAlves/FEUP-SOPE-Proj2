#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "defs.h"

void print_usage(FILE* stream) {
    fprintf(stream, "usage: client <time_out> <num_wanted_seats> <pref_seat_list>\n");
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        print_usage(stderr);
        return ERR;
    }
    
    char pid[WIDTH_PID+1];
    snprintf(pid, WIDTH_PID+1, "%0" MACRO_STRINGIFY(WIDTH_PID) "d", getpid());
    
    printf("Normalized PID: %s\n", pid);
    
    
    return 0;
}
