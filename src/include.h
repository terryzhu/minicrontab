#ifndef INCLUDE_H
#define INCLUDE_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>


#ifndef log
#define log(msg) printf("%s\n",msg);
#endif

typedef struct __entry
{
    char * cmd;  /* job execute command */
    struct __entry * prev;
    struct __entry * next;

    /* 
     * job trigger time
     */
    char    min[60];
    char    hour[12];
    char    dom[31];    // day of month
    char    month[12];
    char    dow[7];     // day of week
}entry;

typedef struct __entry_list
{
    entry * head,*tail;
}entry_list;
typedef struct __cron_db
{
    entry_list  elist;
    int         rate;   // rate between real time and clock time
}cron_db;


#endif
