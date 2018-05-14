#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "defs.h"
#include "clog.h"
#include "cbook.h"

static int clog_descriptor;

void open_clog_file() {
    const char* fileName = "clog.txt";
    clog_descriptor = open(fileName, O_CREAT | O_WRONLY | O_APPEND, 0660);

    if(clog_descriptor < 0){
        // Error opening file
        fprintf(stderr, "Error opening client log file.\n");
        exit(CLOG_OPENING_ERROR);
    }
}

void writeinLog(int answer[]) {
    // Open clog file
    open_clog_file();

    // Write all the reserved seats in the log file
    if(answer[0] > 0) {
        int i;
        int n_seats = answer[0];
        char log_line[CLOG_MSG_MAX_SIZE];

        open_cbook_file();

        for(i=1 ; i<=n_seats ; i++) {
            log_line[0] = '\0';
            snprintf(log_line, CLOG_MSG_MAX_SIZE,
                              "%0" MACRO_STRINGIFY(WIDTH_PID) "d "
                              "%0" MACRO_STRINGIFY(WIDTH_XX) "d."
                              "%0" MACRO_STRINGIFY(WIDTH_NN) "d "
                              "%0" MACRO_STRINGIFY(WIDTH_SEAT) "d\n",
                              getpid(), i, n_seats, answer[i]);
            write(clog_descriptor, log_line, strlen(log_line));
            writeinBookLog(answer[i]);
        }

        close_cbook_file();
    }

    // Write error message in the log file
    else {
        int errorNo = answer[0];
        switch(errorNo) {
        case MAX:
            writeError("MAX");
            break;
        case NST:
            writeError("NST");;
            break;
        case IID:
            writeError("IID");
            break;
        case ERR:
            writeError("ERR");
            break;
        case NAV:
            writeError("NAV");
            break;
        case FUL:
            writeError("FUL");
            break;
        case OUT:
            writeError("OUT");
            break;
        }
    }

    // Close clog file
    close(clog_descriptor);
}

void writeError(char err[]){
    char log_line[CLOG_MSG_MAX_SIZE];
    sprintf(log_line, "%0" MACRO_STRINGIFY(WIDTH_PID) "d" " %.3s\n", getpid(), err);
    write(clog_descriptor, log_line, strlen(log_line));
}
