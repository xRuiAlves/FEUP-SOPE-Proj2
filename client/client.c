#include <stdio.h>
#include <unistd.h>
#include "defs.h"

void print_usage(FILE* stream) {
    fprintf(stream, "usage: client <time_out> <num_wanted_seats> <pref_seat_list>\n");
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        print_usage(stderr);
        return ERR;
    }
    
    char format[4];
    sprintf(format, "%%0%dd\n", WIDTH_PID);
    char pid[6];
    snprintf(pid, 6, format, getpid());
    
    printf("Normalized PID: %s\n", pid);
    
    
    return 0;
}
