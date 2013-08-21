#include "entry.h"

void load_entry( cron_db * db)
{
    // read all entry from file
    // also read the time rate
}


void add_entry(entry_list * list,entry * e)
{
    if (list->head == NULL)
    {
        list->head = e;
        e->prev = NULL;
    }
    else
    {
        e->prev = list->tail;
        list->tail->next = e;
    }
    e->next = NULL;
    list->tail = e;
}

void run_triggered_entry(cron_db * db, time_t clock_time)
{
    entry * e;
    entry_list list ={};
    // Mock data
    entry * data  = (entry*)calloc(1,sizeof(entry));
    data->cmd = "ls";
    add_entry(&list,data);
    find_entries_to_run(&list,db,clock_time);
    for ( e = list.head; e != NULL ; e = e->next)
    {
        // for each entry in list, create process and run it
        run_entry(e);
    }
}

/**
 * add all the triggered entry into the list
 */
void find_entries_to_run(entry_list* list ,cron_db * db, time_t clock_time)
{

}

void run_entry(entry * e)
{
    int pid = fork();
    if(!pid)
    {
        /* Child process */
        log("executing process");
        printf("pid is %d\n",getpid());
        // system(e.cmd);
        log("command is finished\n-----------------------------\n\n\n");
        exit(0);
    }
}
