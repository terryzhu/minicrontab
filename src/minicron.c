#include "include.h"
#include "entry.h"

/**
 * how to stop the daemon and its children process
 * ps -ef |grep minicron |grep -v minicron.c|cut -c9-15|xargs kill -9
 */
#define CLOSE_IT "ps -ef |grep minicron |grep -v minicron.c|cut -c9-15|xargs kill -9"
void first_run_and_exit();
void schedule();
time_t calulate_clock_time(time_t start_time,int rate);
int main()
{
    log(CLOSE_IT);
    first_run_and_exit();
    log("\n\ndaemon process running");
    printf("pid is %d\n",getpid());
    time_t start_time=time(NULL);
    while(1)
    {
        int pid = fork();
        if(!pid)
        {
            /* child process */
            schedule(start_time);
        }
        int status;
        waitpid(pid,&status,0);
        log("scheduler crash detected, recreate it");
        printf("%d %d\n",WIFEXITED(status),WEXITSTATUS(status));
        sleep(100);
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

void schedule(time_t start_time)
{   
    log("schedule process running");
    printf("pid is %d\n",getpid());
    time_t clock_time;
    cron_db db;
    load_entry(&db);
    // schedule
    // There is a bug how to catch the SIGCHLD when child exited
    // signal(SIGCHLD,handle_chd); //does not work, I didn't know how to handle <defunct> process
    while (1)
    {
        clock_time = calulate_clock_time(start_time,db.rate);
        run_triggered_entry(&db,clock_time);
        log ("schedule is sleeping");
        sleep(20);
    }
}

time_t calulate_clock_time(time_t start_time,int rate)
{
    // clockTime = startTime + ( nowTime - startTime ) * rate
    return start_time + ( time(NULL) - start_time ) *rate ;
}
