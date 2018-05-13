#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "defs.h"
#include "clog.h"

static FILE *fp = NULL;

void open_clog_file() {
    const char* fileName = "clog.txt";
    fp = fopen(fileName,"a");

    if(fp == NULL){
        // Error opening file
        fprintf(stderr, "Error opening client log file.\n");
        exit(CLOG_OPENING_ERROR);
    }

    return 0;
}

void writeinLog(int answer[]) {
    // Open clog file
    open_clog_file();

    // Write all the reserved seats in the log file
    if(answer[0] > 0) {
        int i;
        int n_seats = answer[0];
        for(i=1 ; i<=n_seats ; i++) {
            fprintf(fp,"%0" MACRO_STRINGIFY(WIDTH_PID) "d ", getpid());
            fprintf(fp,"%0" MACRO_STRINGIFY(WIDTH_XX) "d.",i);
            fprintf(fp,"%0" MACRO_STRINGIFY(WIDTH_NN) "d ", n_seats);
            fprintf(fp,"%0" MACRO_STRINGIFY(WIDTH_SEAT) "d\n", answer[i]);
            fflush(fp);
        }
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
        }
    }

    // Close clog file
    fclose(fp);
}

void writeError(char err[]){
    fprintf(fp,"%0" MACRO_STRINGIFY(WIDTH_PID) "d" " %.3s\n",getpid(),err);
    fflush(fp);
}
