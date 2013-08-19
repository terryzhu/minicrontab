#include "include.h"
#include "entry.h"

/**
 * how to stop the daemon and its children process
 * ps -ef |grep minicron |grep -v minicron.c|cut -c9-15|xargs kill -9
 */
#define CLOSE_IT "ps -ef |grep minicron |grep -v minicron.c|cut -c9-15|xargs kill -9"
void first_run_and_exit();
void schedule();
int main()
{
		log(CLOSE_IT);
    first_run_and_exit();
    log("\n\ndaemon process running");
    printf("pid is %d\n",getpid());
    while(1)
    {
        int pid = fork();
        if(!pid)
        {
            /* child process */
            schedule();
        }
        int status;
        waitpid(pid,&status,0);
        log("scheduler crash detected, recreate it");
    }
    return 0;
}

void first_run_and_exit()
{
    int pid = fork();
    if (pid)
    {
        /* parent process */
        log("The first process is exited");
        exit(0);
    }
    setsid();
}

void handle_chd(int sig)
{
    log("handle_chd");
    wait(NULL);
}

void schedule()
{   
    log("schedule process running");
    printf("pid is %d\n",getpid());
		cron_db db;
		load_entry(&db);
    // schedule
		// for each task in db
		// if the task execution time is now, fork and run it
    int pid = fork();
    if(!pid)
    {
        log("executing process");
        printf("pid is %d\n",getpid());
        // system(xxx);
        log("command is finished\n-----------------------------\n\n\n");
        exit(0);
    }

		// There is a bug how to catch the SIGCHLD when child exited
		// signal(SIGCHLD,handle_chd); does not work
    // assume 10s crash for test
    sleep(10);
    log("schedule is crashed");
    exit(0);
}

