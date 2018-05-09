#include "request_message.h"



RequestMessage create_request_message(u_int pid, u_int num_wanted_seats, u_int* pref_seat_list, u_int num_pref_seats) {
    RequestMessage msg;

    msg.pid = pid;
    msg.num_wanted_seats = num_wanted_seats;
    msg.pref_seat_list = pref_seat_list;
    msg.num_pref_seats = num_pref_seats;

    return msg;
}
