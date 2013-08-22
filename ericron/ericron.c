#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>


#define CONFIG_FILE "/etc/ericrontab"
#define MAX_LINE_LENGTH 4096

struct job {
    int* min;
    int* hour;
    int* day_of_month;
    int* month_of_year;
    int* day_of_week;
    char* script;

    struct job* next;
};

struct job* parse_line( const char* line );

struct job* new_job_obj( void ) {
    struct job* rel = malloc( sizeof( struct job ) );
    if ( NULL == rel ) {
        syslog( LOG_ERR, "out of memory" );
        exit( 7 );
    }
    memset( rel, 0, sizeof( struct job ) );
    return rel;
}

static pid_t scheduler_pid = 0;

void scheduler( void );

void sigchld_handler( int sig_num );

int main( int argc, char** argv ) {

    uid_t uid = geteuid();
    gid_t gid = getegid();
    if ( ! ( 0 == uid || 0 == gid ) ) {
        printf( "ericron must be run with root privelege.\n" );
        exit( 1 );
    } 
    
    if ( 0 != chdir( "/tmp" ) ) {
        printf( "failed to change to \'/tmp\' directory, aborted.\n" );
        exit( 2 );
    }

    close( 0 );
    close( 1 );
    close( 2 );

    // when configure ericron in /etc/inittab with respawn action,
    // it should fork new one and exist, otherwise init will think
    // it crash and start it repeatly.
    if ( 1 != getppid() ) {
        pid_t pid =  fork();
        if ( 0 != pid ) {
            exit( 0 );
        } 

        pid = fork();
        if ( 0 != pid ) {
            exit( 0 );
        }
    }

    signal( SIGCHLD, sigchld_handler );
    sigchld_handler( SIGCHLD );
    while ( 1 ) {
            sleep( 10 );
    }
    return 0;
}

void sigchld_handler( int sig_num ) {
    if ( scheduler_pid != 0 ) 
        waitpid( scheduler_pid, NULL, WNOHANG );

    scheduler_pid = 0;
    pid_t pid = fork();
    if ( pid != 0 ) {
        scheduler_pid = pid;
        return;
    } else {
        scheduler();
    }
    
}

void scheduler( void ) {
    while ( 1 ) {
        sleep( 5 );
        syslog( LOG_INFO, "ericron pid=%d\n", getpid() );
    }
}

struct job* load_config() {
    int size = MAX_LINE_LENGTH + 1;
    char buf[size];
    memset( &buf, 0, size );

    FILE* file = fopen( CONFIG_FILE, "r" );
    if ( NULL == file ) {
        syslog( LOG_ERR, "failed to open file %s", CONFIG_FILE );
        sleep( 5 );
        exit( 1 );
    }
    
    struct job* head = new_job_obj();
    struct job* curr = head;
    while ( 1 ) {
        char* rel = fgets( buf, size, file );
        if ( NULL == rel ) {
            break;
        }

        curr->next = parse_line( rel );
        if ( NULL != curr->next ) {
            curr = curr->next;
        }
    }

    fclose( file );
    return head;
}

struct job* parse_line( const char* line ) {
    return NULL;
}
