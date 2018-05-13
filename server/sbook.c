#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "defs.h"
#include "sbook.h"

static int sbook_descriptor;

void open_sbook_file() {
    const char* fileName = "sbook.txt";
    sbook_descriptor = open(fileName, O_CREAT | O_WRONLY | O_APPEND, 0660);

    if(sbook_descriptor < 0){
        // Error opening file
        fprintf(stderr, "Error opening client book log file.\n");
        exit(SBOOK_OPENING_ERROR);
    }
}
void close_sbook_file() {
    // Close cbook file
    close(sbook_descriptor);
}

void writeinSBookLog(int seat) {

  // Write all the reserved seats in the log file
  char log_line[WIDTH_SEAT + 2];

  snprintf(log_line, WIDTH_SEAT+2,"%0" MACRO_STRINGIFY(WIDTH_SEAT) "d\n", seat);
  write(sbook_descriptor, log_line, strlen(log_line));
}
