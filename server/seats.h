#ifndef _SEATS_H_
#define _SEATS_H_

#include <stdbool.h>

typedef struct {
    bool isTaken;
    int owner_clientid;
} Seat;

int isSeatFree(Seat *seats, int seatNum);

void bookSeat(Seat *seats, int seatNum, int clientId);

void freeSeat(Seat *seats, int seatNum);

#endif  // _SEATS_H_