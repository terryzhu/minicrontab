minicrontab
===========

TODO:

* module design

    > ./minicrontab &

fork -> fork -> the running instance -> fork to create scheduler -> fork job 1,2..N

* study process session

* redirect input/output

* how to handle user to modify configuration file

* sleep interval

* other detailed implementation

**Time conversion algorithm:**

* The daemon process has start time
* The schedule process has the now time
* So the clockTime = startTime + ( nowTime - startTime ) * rate
* We can use clockTime to choose which job should be executed now.

study the http://enterprise-storage-os.googlecode.com/files/vixie-cron-4.1.tar.bz2

ISC cron tool code analysis

1. fork processes to setup the correct process structure
2. close input output stderr
3. load the cron data to the cron database(read from files)
4. calculate time and sleep
5. check all the entry in database whether it should be run now
6. add the to be running job into job queue
7. run the job from job queue
8. loop to step 3

data structure:
    
    typedef struct _user {
        struct _user    *next, *prev;   /* links */
        char        *name;
        time_t      mtime;      /* last modtime of crontab */
        entry       *crontab;   /* this person's crontab */
    } user;
    
    typedef struct _cron_db {
        user        *head, *tail;   /* links */
        time_t      mtime;      /* last modtime on spooldir */
    } cron_db;
    
    typedef struct _entry {
        struct _entry   *next;
        struct passwd   *pwd;
        char        **envp;
        char        *cmd;
        int     flags;
    } entry;
    
    typedef struct _job {
        struct _job *next;
        entry       *e;
        user        *u;
    } job;
    static job  *jhead = NULL, *jtail = NULL;

![datagram](http://img.blog.csdn.net/20130818093338718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5oYXBweXBlb3BsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
