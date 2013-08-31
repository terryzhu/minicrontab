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
#include <assert.h>

#ifndef log
#define log(msg) printf("%s\n",msg);
#endif

#define MAX_MIN 60
#define MAX_HOUR 24
#define MAX_DOM 31
#define MAX_MONTH 12
#define MAX_DOW 7
typedef struct __entry
{
    char * cmd;  /* job execute command */
    struct __entry * prev;
    struct __entry * next;

    /* 
     * job trigger time
     */
    char    min[MAX_MIN];
    char    hour[MAX_HOUR];
    char    dom[MAX_DOM];    // day of month
    char    month[MAX_MONTH];
    char    dow[MAX_DOW];     // day of week
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
