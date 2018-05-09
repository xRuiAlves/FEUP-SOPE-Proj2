#ifndef _BUFFER_H_
#define _BUFFER_H_

/**
 * Handles unit buffer operations
 */

#define BUFFER_LENGTH 1

char * read_buffer();
void write_to_buffer(char * data);

#endif  // _BUFFER_H_