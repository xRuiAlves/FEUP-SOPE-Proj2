#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdlib.h>

unsigned int parse_unsigned_int(char* str);
/**
 * Splits a string based on given tokens. Result is returned via the last 2 arguments which should be passed by reference.
 * Is thread safe.
 * Result list can be iterated for *len entries and each must be deallocated, plus the pointer itself (*result_list).
 */
int split_string(const char * src_str, const char * tokens, char *** result_list, size_t * len);

#endif  // _PARSER_H_