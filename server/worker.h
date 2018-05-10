#ifndef _WORKER_H_
#define _WORKER_H_

#define WORKER_RUNNING      1
#define WORKER_STOP         0

void * startWorking(void * args);

void set_worker_status(int status);

#endif  // _WORKER_H_