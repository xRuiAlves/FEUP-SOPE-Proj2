#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "defs.h"
#include "cbook.h"

static int cbook_descriptor;

void open_cbook_file() {
    const char* fileName = "cbook.txt";
    cbook_descriptor = open(fileName, O_CREAT | O_WRONLY | O_APPEND, 0660);

    if(cbook_descriptor < 0){
        // Error opening file
        fprintf(stderr, "Error opening client book log file.\n");
        exit(CBOOK_OPENING_ERROR);
    }
}
void close_cbook_file() {
    // Close cbook file
    close(cbook_descriptor);
}

void writeinBookLog(int seat) {

  // Write all the reserved seats in the log file
  char log_line[WIDTH_SEAT + 2];

  snprintf(log_line, WIDTH_SEAT+2,"%0" MACRO_STRINGIFY(WIDTH_SEAT) "d\n", seat);
  write(cbook_descriptor, log_line, strlen(log_line));
}
