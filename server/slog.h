#ifndef _SLOG_H_
#define _SLOG_H_
#include "worker.h"
/**
  * Opens the client log file and exits with error status (2) on failure
  */
void open_slog_file();

/**
  * Writes a server error response in the client log file
  *
  * @param err String that identifies the type of error returned by the server
  */
void writetoServerLogError(ClientMessage cmess,unsigned int pid, int error_status);

/**
  * Writes a server answer in the client log file
  *
  * @param answer Array of integer containing all the reserved seats OR the server
  *        error identifier, in case of failure
  */
void writetoServerLog(ClientMessage cmess,unsigned int pid, int n_reserved_seats, unsigned int reserved_seats[]);

#endif
