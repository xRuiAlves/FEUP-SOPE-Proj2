#ifndef _REQUEST_MESSAGE_H_
#define _REQUEST_MESSAGE_H_

#include "defs.h"

/**
 *  @brief Message with a request from the client to the server
 */
typedef struct RequestMessage_struct {
    u_int pid;                  ///< Requesting Client's PID
    u_int num_wanted_seats;     ///< Number of seats wanted by the client
    u_int* pref_seat_list;      ///< List of the client's prefered seats identifiers
    u_int num_pref_seats;       ///< Number of the client's prefered seats identifiers
} RequestMessage;


/**
  * Creates a RequestMessage object with the specified parameters
  *
  * @param pid Requesting Client's PID
  * @param num_wanted_seats Number of seats wanted by the client
  * @param pref_seat_list List of the client's prefered seats identifiers
  * @param num_pref_seats Number of the client's prefered seats identifiers
  *
  * @return Returns a RequestMessage object with the specified parameters
  */
RequestMessage create_request_message(u_int pid, u_int num_wanted_seats, u_int* pref_seat_list, u_int num_pref_seats);

/**
  * Sends a request message to the server. Exits with error status on server
  * fifo opening failure (4) or on message broadcasting failure (5)
  *
  * @param msg Request message
  */
void broadcast_message(RequestMessage msg);


#endif  // _REQUEST_MESSAGE_H_
