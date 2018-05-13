#ifndef _SEATS_H_
#define _SEATS_H_

#include <stdbool.h>

typedef struct {
    bool isTaken;
    int owner_clientid;
} Seat;

void initNrAvailableSeats(unsigned int n_available_seats);

unsigned int getNrAvailableSeats();

unsigned int getPossibleMaxID();

/**
 * Returns 1 if seat is free, 0 if it is taken, 2 if room is full and -1 in case of error
 */
int isSeatFree(Seat *seats, int seatNum);

void bookSeat(Seat *seats, int seatNum, int clientId);

void freeSeat(Seat *seats, int seatNum);

#endif  // _SEATS_H_