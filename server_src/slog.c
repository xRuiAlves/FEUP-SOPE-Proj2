#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "slog.h"
#include "defs.h"
#include "sbook.h"

static int slog_descriptor;

void open_slog_file() {
    const char* fileName = "slog.txt";
    slog_descriptor = open(fileName, O_CREAT | O_WRONLY | O_APPEND, 0660);

    if(slog_descriptor < 0){
        // Error opening file
        fprintf(stderr, "Error opening server log file.\n");
        exit(SLOG_OPENING_ERROR);
    }
}

void close_slog_file() {
    close(slog_descriptor);
}

void writeServerWorkerCreating(unsigned int tid) {
    char log_line[100];
    sprintf(log_line, "%02u-OPEN\n", tid);
    write(slog_descriptor, log_line, strlen(log_line));
}

void writeServerWorkerClosing(unsigned int tid) {
    char log_line[100];
    snprintf(log_line, sizeof(log_line), "%02u-CLOSED\n", tid);
    write(slog_descriptor, log_line, strlen(log_line));
}

void writeServerClosing() {
    const char* closingMsg = "SERVER CLOSED\n";
    write(slog_descriptor, closingMsg, strlen(closingMsg));
}

void writetoServerLog(ClientMessage cmess, unsigned int tid, int n_reserved_seats, unsigned int reserved_seats[]) {
    char log_line[BUFF_SIZE];
    char num_str[20];
    int num_spaces = MAX_CLI_SEATS * (WIDTH_SEAT+1);
    char seats_buf[2* MAX_CLI_SEATS * (WIDTH_SEAT+1)];
    log_line[0] = '\0';
    seats_buf[0] = '\0';

    snprintf(log_line, BUFF_SIZE, "%02u"
                      "-"
                      "%0" MACRO_STRINGIFY(WIDTH_PID) "u"
                      "-"
                      "%0" MACRO_STRINGIFY(WIDTH_NT) "u"
                      ": ",
                      tid, cmess.pid, cmess.num_wanted_seats);
    int i;

    for(i=0;i<cmess.num_pref_seats;i++){
      snprintf(num_str, sizeof(num_str), "%0" MACRO_STRINGIFY(WIDTH_SEAT) "u ", cmess.pref_seats[i]);
      strncat(seats_buf, num_str, WIDTH_SEAT+1);
    }
    strncat(log_line, seats_buf, MAX_CLI_SEATS * (WIDTH_SEAT+1));

    for (i=strlen(seats_buf) ; i<num_spaces ; i++) {    // Fill with spaces
        strncat(log_line, " ", 1);
    }
    strncat(log_line, "-", 1);

    seats_buf[0] = '\0';
    for(i=0;i<n_reserved_seats;i++){
      snprintf(num_str, sizeof(num_str), " %0" MACRO_STRINGIFY(WIDTH_SEAT) "d", cmess.pref_seats[i]);
      strncat(seats_buf, num_str, WIDTH_SEAT+1);
      writeinSBookLog(reserved_seats[i]);
    }
    strncat(log_line, seats_buf, MAX_CLI_SEATS * (WIDTH_SEAT+1));
    for (i=strlen(seats_buf) ; i<num_spaces ; i++) {    // Fill with spaces
        strncat(log_line, " ", 1);
    }
    strncat(log_line, "\n", 1);
    write(slog_descriptor, log_line, strlen(log_line));
}

void writetoServerLogError(ClientMessage cmess, unsigned int tid, int error_status){
    if(cmess.num_wanted_seats > MAX_CLI_SEATS) {
        //Truncating the log file write
        cmess.num_wanted_seats = MAX_CLI_SEATS;
    }

    char log_line[BUFF_SIZE];
    char num_str[20];
    int num_spaces = MAX_CLI_SEATS * (WIDTH_SEAT+1);
    char seats_buf[2 * MAX_CLI_SEATS * (WIDTH_SEAT+1)];
    log_line[0] = '\0';
    seats_buf[0] = '\0';

    snprintf(log_line, BUFF_SIZE, "%02u"
                      "-"
                      "%0" MACRO_STRINGIFY(WIDTH_PID) "u"
                      "-"
                      "%0" MACRO_STRINGIFY(WIDTH_NT) "u"
                      ": ",
                      tid, cmess.pid, cmess.num_wanted_seats);

    int i;
    for(i=0;i<cmess.num_pref_seats;i++){
      snprintf(num_str, sizeof(num_str), "%0" MACRO_STRINGIFY(WIDTH_SEAT) "u ", cmess.pref_seats[i]);
      strncat(seats_buf, num_str, WIDTH_SEAT+1);
    }
    strncat(log_line, seats_buf, MAX_CLI_SEATS * (WIDTH_SEAT+1));

    for (i=strlen(seats_buf) ; i<num_spaces ; i++) {    // Fill with spaces
        strncat(log_line, " ", 1);
    }
    strncat(log_line, "- ", 2);

    switch(error_status) {
    case MAX:
        strncat(log_line, "MAX\n", 4);
        break;
    case NST:
        strncat(log_line, "NST\n", 4);
        break;
    case IID:
        strncat(log_line, "IID\n", 4);
        break;
    case ERR:
        strncat(log_line, "ERR\n", 4);
        break;
    case NAV:
        strncat(log_line, "NAV\n", 4);
        break;
    case FUL:
        strncat(log_line, "FUL\n", 4);
        break;
    }
    write(slog_descriptor, log_line, strlen(log_line));
}
