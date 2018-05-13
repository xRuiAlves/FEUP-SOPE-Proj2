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

void writetoServerLog(ClientMessage cmess,unsigned int pid, int n_reserved_seats, unsigned int reserved_seats[]) {
    // Open slog file
    open_slog_file();
    open_sbook_file();
    char log_line[BUFF_SIZE];
    log_line[0] = '\0';
    sprintf(log_line, "69"
                      "-"
                      "%0" MACRO_STRINGIFY(WIDTH_PID) "d "
                      "-"
                      "%0" MACRO_STRINGIFY(WIDTH_NT) "d "
                      ": ",
                      getpid(), cmess.num_wanted_seats);
    int i;
    for(i=0;i<cmess.num_pref_seats;i++){
      sprintf(log_line,"%0" MACRO_STRINGIFY(WIDTH_SEAT) "d ", cmess.pref_seats[i]);
    }
    sprintf(log_line, "- ");
    for(i=0;i<n_reserved_seats;i++){
      sprintf(log_line,"%0" MACRO_STRINGIFY(WIDTH_SEAT) "d ", reserved_seats[i]);
      writeinSBookLog(reserved_seats[i]);
    }
    sprintf(log_line,"\n");
    write(slog_descriptor, log_line, strlen(log_line));


    // Close slog file
    close(slog_descriptor);
    close_sbook_file();
}

void writetoServerLogError(ClientMessage cmess,unsigned int pid, int error_status){
  // Open slog file
  open_slog_file();
  char log_line[BUFF_SIZE];
  log_line[0] = '\0';
  sprintf(log_line, "69"
                    "-"
                    "%0" MACRO_STRINGIFY(WIDTH_PID) "d "
                    "-"
                    "%0" MACRO_STRINGIFY(WIDTH_NT) "d "
                    ": ",
                    getpid(), cmess.num_wanted_seats);
  int i;
  for(i=0;i<cmess.num_pref_seats;i++){
    sprintf(log_line,"%0" MACRO_STRINGIFY(WIDTH_SEAT) "d ", cmess.pref_seats[i]);
  }
  sprintf(log_line, "- ");
  switch(error_status) {
  case MAX:
      sprintf(log_line, "MAX\n");
      break;
  case NST:
      sprintf(log_line, "NST\n");
      break;
  case IID:
      sprintf(log_line, "IID\n");
      break;
  case ERR:
      sprintf(log_line, "ERR\n");
      break;
  case NAV:
      sprintf(log_line, "NAV\n");
      break;
  case FUL:
      sprintf(log_line, "FUL\n");
      break;
  case OUT:
      sprintf(log_line, "OUT\n");
      break;
  }
  write(slog_descriptor, log_line, strlen(log_line));
}
