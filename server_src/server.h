#ifndef _SERVER_H_
#define _SERVER_H_

void print_usage(FILE * stream, char * progname);

int listen_for_requests(int open_time_s);
void close_server();
void setup_alarm(unsigned int time_out);

#endif  // _SERVER_H_