#ifndef _ALARM_H_
#define _ALARM_H_

/**
 * SIGALRM handler, client waiting timeout handler
 */
void alarm_handler();

/**
 * Sets up how to handle an alarm signal
 */
void setup_alarm();




#endif  // _ALARM_H_
