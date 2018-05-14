#ifndef _CLOG_H_
#define _CLOG_H_

/**
  * Opens the client log file and exits with error status (2) on failure
  */
void open_clog_file();

/**
  * Writes a server error response in the client log file
  *
  * @param err String that identifies the type of error returned by the server
  */
void writeError(char err[]);

/**
  * Writes a server answer in the client log file
  *
  * @param answer Array of integer containing all the reserved seats OR the server
  *        error identifier, in case of failure
  */
void writeinLog(int answer[]);

#endif
