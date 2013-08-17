#ifndef JOB_H
#define JOB_H
#include "include.h"
typedef struct __job
{
    struct tm time; /* job trigger time*/
    char cmd[100];  /* job execute command */
}job;









#endif
