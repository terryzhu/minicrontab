#ifndef JOB_H
#define JOB_H
#include "include.h"

void load_entry(cron_db * db);

void add_entry(entry_list * list,entry * e);


void run_triggered_entry(cron_db * db, time_t clock_time);

void find_entries_to_run(entry_list* list ,cron_db * db, time_t clock_time);

void run_entry(entry * e);



#endif
