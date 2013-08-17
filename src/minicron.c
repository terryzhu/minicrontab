#include "include.h"
#include "job.h"

/**
 * how to stop the daemon and its children process
 * ps -ef |grep minicron |grep -v minicron.c|cut -c9-15|xargs kill -9
 */
void first_run_and_exit();
void schedule();
int main()
{
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
    struct tm time;
    job test_job={time,"ls"};
    // load_jobs();
    // schedule
    int pid = fork();
    if(!pid)
    {
        log("executing process");
        log(test_job.cmd);
        printf("pid is %d\n",getpid());
        system(test_job.cmd);
        log("command is finished\n-----------------------------\n\n\n");
        exit(0);
    }

    // signal(SIGCHLD,handle_chd);
    // assume 10s crash for test
    sleep(10);
    log("schedule is crashed");
    exit(0);
}
