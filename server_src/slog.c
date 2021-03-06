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
    char log_line[50];
    sprintf(log_line, "%02u-OPEN\n", tid);
    write(slog_descriptor, log_line, strlen(log_line));
}

void writeServerWorkerClosing(unsigned int tid) {
    char log_line[50];
    sprintf(log_line, "%02u-CLOSED\n", tid);
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
    char seats_buf[MAX_CLI_SEATS * (WIDTH_SEAT+1)];
    log_line[0] = '\0';
    seats_buf[0] = '\0';

    sprintf(log_line, "%02u"
                      "-"
                      "%0" MACRO_STRINGIFY(WIDTH_PID) "d"
                      "-"
                      "%0" MACRO_STRINGIFY(WIDTH_NT) "d"
                      ": ",
                      tid, cmess.pid, cmess.num_wanted_seats);
    int i;
    for(i=0;i<cmess.num_pref_seats;i++){
      sprintf(num_str,"%0" MACRO_STRINGIFY(WIDTH_SEAT) "d ", cmess.pref_seats[i]);
      strcat(seats_buf, num_str);
    }
    strcat(log_line, seats_buf);
    for (i=strlen(seats_buf) ; i<num_spaces ; i++) {    // Fill with spaces
        strcat(log_line, " ");
    }

    strcat(log_line, "-");

    seats_buf[0] = '\0';
    for(i=0;i<n_reserved_seats;i++){
      sprintf(num_str," %0" MACRO_STRINGIFY(WIDTH_SEAT) "d", reserved_seats[i]);
      strcat(seats_buf, num_str);
      writeinSBookLog(reserved_seats[i]);
    }
    strcat(log_line, seats_buf);
    for (i=strlen(seats_buf) ; i<num_spaces ; i++) {    // Fill with spaces
        strcat(log_line, " ");
    }
    strcat(log_line,"\n");
    write(slog_descriptor, log_line, strlen(log_line));
}

void writetoServerLogError(ClientMessage cmess, unsigned int tid, int error_status){
  char log_line[BUFF_SIZE];
  char num_str[20];
  int num_spaces = MAX_CLI_SEATS * (WIDTH_SEAT+1);
  char seats_buf[MAX_CLI_SEATS * (WIDTH_SEAT+1)];
  log_line[0] = '\0';
  seats_buf[0] = '\0';

  sprintf(log_line, "%02u"
                    "-"
                    "%0" MACRO_STRINGIFY(WIDTH_PID) "d"
                    "-"
                    "%0" MACRO_STRINGIFY(WIDTH_NT) "d"
                    ": ",
                    tid, cmess.pid, cmess.num_wanted_seats);
  int i;
  for(i=0;i<cmess.num_pref_seats;i++){
    sprintf(num_str, "%0" MACRO_STRINGIFY(WIDTH_SEAT) "d ", cmess.pref_seats[i]);
    strcat(seats_buf, num_str);
  }
  strcat(log_line, seats_buf);
  for (i=strlen(seats_buf) ; i<num_spaces ; i++) {    // Fill with spaces
      strcat(log_line, " ");
  }
  strcat(log_line, "- ");

  switch(error_status) {
  case MAX:
      strcat(log_line, "MAX\n");
      break;
  case NST:
      strcat(log_line, "NST\n");
      break;
  case IID:
      strcat(log_line, "IID\n");
      break;
  case ERR:
      strcat(log_line, "ERR\n");
      break;
  case NAV:
      strcat(log_line, "NAV\n");
      break;
  case FUL:
      strcat(log_line, "FUL\n");
      break;
  }
  write(slog_descriptor, log_line, strlen(log_line));
}
