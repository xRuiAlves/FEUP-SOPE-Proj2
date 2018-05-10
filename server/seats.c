#include "defs.h"
#include "seats.h"
#include <stdio.h>

int isSeatFree(Seat *seats, int seatNum) {
    if(seatNum > MAX_ROOM_SEATS) {
        return -1;
    }

    int result = !seats[seatNum-1].isTaken;

    DELAY();

    return result;
}

void bookSeat(Seat *seats, int seatNum, int clientId) {
    if(seatNum > MAX_ROOM_SEATS || seats[seatNum-1].isTaken) {
        printf("Error in booking seat number %d for client number %d\n", seatNum, clientId);
        return;
    }

    seats[seatNum-1].isTaken = true;
    seats[seatNum-1].owner_clientid = clientId;

    DELAY();
}

void freeSeat(Seat *seats, int seatNum) {
    if(seatNum > MAX_ROOM_SEATS || !seats[seatNum-1].isTaken) {
        printf("Error in freeing seat number %d\n", seatNum);
        return;
    }

    seats[seatNum-1].isTaken = false;
    seats[seatNum-1].owner_clientid = 0;

    DELAY();
}