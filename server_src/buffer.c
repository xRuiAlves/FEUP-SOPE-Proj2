#include "buffer.h"
#include <stdlib.h>

static char * buffer[BUFFER_LENGTH] = {NULL};

char * read_buffer() {
    return buffer[0];
}

void write_to_buffer(char * data) {
    buffer[0] = data;
}