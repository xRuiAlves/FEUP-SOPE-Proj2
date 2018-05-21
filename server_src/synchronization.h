#ifndef _SYNCHRONIZATION_H_
#define _SYNCHRONIZATION_H_

int init_sync();
int finish_sync();

void wait_has_data_sem();
/**
 * Returns 0 if the wait succeeded, 1 if the caller must wait (normal wait call would block), -1 if an error ocurred
 * (Does not block)
 */
int try_wait_has_data_sem();
void signal_has_data_sem();
void wait_can_send_data_sem();
void signal_can_send_data_sem();
void lock_seats_mutex(unsigned int seatID);
void unlock_seats_mutex(unsigned int seatID);
int try_lock_buffer_mutex();
void unlock_buffer_mutex();

#endif  // _SYNCHRONIZATION_H_