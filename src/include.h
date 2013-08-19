#ifndef INCLUDE_H
#define INCLUDE_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>


#ifndef log
#define log(msg) printf("%s\n",msg);
#endif

typedef struct __entry
{
    struct tm time; /* job trigger time*/
    char cmd[100];  /* job execute command */
}entry;

typedef struct __cron_db
{
	struct entry * head,*tail;
}cron_db;


#endif
