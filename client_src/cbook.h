#ifndef _CBOOK_H_
#define _CBOOK_H_

/**
  * Opens the client book log file and exits with error status (2) on failure
  */
void open_cbook_file();

/**
  * Closes the client book log file
  */

void close_cbook_file();

/**
  * Writes the seats booked in the client book log file
  *
  * @param seat the seat that was booked
  */
void writeinBookLog(int seat);
#endif
