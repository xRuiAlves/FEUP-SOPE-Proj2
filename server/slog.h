#ifndef _SLOG_H_
#define _SLOG_H_
#include "worker.h"
/**
  * Opens the server log file and exits with error status on failure
  */
void open_slog_file();

/**
  * Closes the server slog log file
  */
void close_slog_file();

/**
  * Writes a server log message indicating thread ticket seller was created
  *
  * @param id Thread id
  */
void writeServerWorkerCreating(unsigned int tid);

/**
  * Writes a server log message indicating thread ticket seller was closed
  *
  * @param id Thread id
  */
void writeServerWorkerClosing(unsigned int tid);

/**
  * Writes a server error log message in the server log file
  *
  * @param cmess Client message
  * @param tid Thread id
  * @param error_status Error that occured
  */
void writetoServerLogError(ClientMessage cmess, unsigned int tid, int error_status);

/**
  * Writes a server error log message in the server log file
  *
  * @param cmess Client message
  * @param tid Thread id
  * @param n_reserved_seats Number of reserved Seats
  * @param reserved_seats Array containing all the reserved seats
  */
void writetoServerLog(ClientMessage cmess, unsigned int tid, int n_reserved_seats, unsigned int reserved_seats[]);

#endif
