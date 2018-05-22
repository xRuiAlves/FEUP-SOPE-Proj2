#ifndef _BUFFER_H_
#define _BUFFER_H_

/**
 * Handles unit buffer operations
 */

#define BUFFER_LENGTH 1

char * read_buffer();
void write_to_buffer(char * data);
void set_buffer_full();
void set_buffer_empty();
int is_buffer_full();

#endif  // _BUFFER_H_