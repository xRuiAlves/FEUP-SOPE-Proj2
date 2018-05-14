#include "alarm.h"
#include "defs.h"
#include "clog.h"
#include <signal.h>
#include <stdlib.h>

void alarm_handler() {
    int timeout_error[] = {OUT};
    writeinLog(timeout_error);
    exit(0);
}

void setup_alarm() {
    signal(SIGALRM, alarm_handler);
}
