#ifndef _SYNCHRONIZATION_H_
#define _SYNCHRONIZATION_H_

int init_sync();
int finish_sync();

void wait_has_data_sem();
void signal_has_data_sem();
void wait_can_send_data_sem();
void signal_can_send_data_sem();
void lock_seats_mutex();
void unlock_seats_mutex();

#endif  // _SYNCHRONIZATION_H_