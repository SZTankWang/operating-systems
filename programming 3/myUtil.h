#ifndef MYUTIL_H_
#define MYUTIL_H_

#include "info.h"

int detach(INFO * shm, int mkrID);

double timing(struct timeval before,struct timeval after, int mkrID);

#endif 