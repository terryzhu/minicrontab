#ifndef JOB_H
#define JOB_H
#include "include.h"
typedef struct __entry
{
    struct tm time; /* job trigger time*/
    char cmd[100];  /* job execute command */
}entry;

void load_entry(cron_db * db);









#endif
