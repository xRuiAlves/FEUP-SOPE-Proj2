#ifndef _CLIENT_H_
#define _CLIENT_H_

/**
  * Prints the correct program usage
  *
  * @param stream Stream in which the usage is printed on
  */
void print_usage(FILE* stream);

/**
  * Parses a string to an unsiged integer value
  *
  * @param value String representing an integer value
  *
  * @return Returns the parsed integer value OR returns UINT_MAX if error occurs
  */
u_int parse_unsigned_int(char* value);

#endif  // _CLIENT_H_
