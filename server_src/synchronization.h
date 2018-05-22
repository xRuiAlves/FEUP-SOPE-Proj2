#ifndef _SYNCHRONIZATION_H_
#define _SYNCHRONIZATION_H_

int init_sync();
int finish_sync();

void lock_seats_mutex(unsigned int seatID);
void unlock_seats_mutex(unsigned int seatID);
void wait_until_buffer_empty();
void signal_buffer_full();
int wait_until_buffer_full();
void signal_buffer_empty();

#endif  // _SYNCHRONIZATION_H_