#ifndef _PARSER_H_
#define _PARSER_H_

#include "defs.h"
#include "stdlib.h"

/**
  * Parses a string to an unsiged integer value
  *
  * @param value String representing an integer value
  *
  * @return Returns the parsed integer value OR returns UINT_MAX if error occurs
  */
u_int parse_unsigned_int(char* str);

/**
  * Parses a string to an integer value
  *
  * @param value String representing an integer value
  *
  * @return Returns the parsed integer value OR returns UINT_MAX if error occurs
  */
int parse_int(char* str);

int split_string(const char * src_str, const char * tokens, char *** result_list, size_t * len);


#endif  // _PARSER_H_
