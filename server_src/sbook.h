#ifndef _SBOOK_H_
#define _SBOOK_H_

/**
  * Opens the server book log file and exits with error status (2) on failure
  */
void open_sbook_file();

/**
  * Closes the server book log file
  */
void close_sbook_file();

/**
  * Writes the seats booked in the server book log file
  *
  * @param seat the seat that was booked
  */
void writeinSBookLog(int seat);
#endif
