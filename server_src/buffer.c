#include "buffer.h"
#include <stdlib.h>

static char * buffer[BUFFER_LENGTH] = {NULL};
static int is_full = 0;

char * read_buffer() {
    is_full = 0;
    return buffer[0];
}

void write_to_buffer(char * data) {
    buffer[0] = data;
    is_full = 1;
}

int is_buffer_full() {
    return is_full;
}