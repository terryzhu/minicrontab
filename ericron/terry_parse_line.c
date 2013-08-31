#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <assert.h>

#define MAX_LINE_LENGTH 4096
#define ANYTIME 0xff

#define MAX_MIN 60
#define MAX_HOUR 24
#define MAX_DOM 31
#define MAX_MONTH 12
#define MAX_DOW 7

typedef struct __job {
    int min[MAX_MIN];//0-59
    int hour[MAX_HOUR];//0-23
    int day_of_month[MAX_DOM];//1-31
    int month_of_year[MAX_MONTH];//1-12
    int day_of_week[MAX_DOW]; // 0-6
    char* script;

    struct __job* next;
} job;

free_job(job * j)
{
    if(j->script)free(j->script);
    free(j);
}

void debug_bit(int * buf,int max)
{
    int i;
    for(i=0;i<max;i++)
    {
        printf("%d ",buf[i]);
    }
    printf("\n");
}
void debug_job(job * e)
{
    printf("\n\n");
    debug_bit(e->min,MAX_MIN);
    debug_bit(e->hour,MAX_HOUR);
    debug_bit(e->day_of_month,MAX_DOM);
    debug_bit(e->month_of_year,MAX_MONTH);
    debug_bit(e->day_of_week,MAX_DOW);
    printf("script is :  %s",e->script);

}

void set_bit(int * buf,int index)
{
    buf[index] = 1;
}
void set_job_bit(int * data,int from,int to,int low,int high,int repeat)
{
    if (from ==ANYTIME && to == ANYTIME)
    {
        from = low;
        to = high;
    }
    if (repeat==-1)
    {
        repeat = 1;
    }
    int i;
    int f_g_t = from > to;// from is greater than to
    if (f_g_t)
    {
        // such as 21-7/2
        for(i=from-low;i<high-low+1;i+=repeat)
        {
            // fill 21,23
            set_bit(data,i);
        }
        i = i- (high-low+1);
        for(;i<to-low+1;i+=repeat)
        {
            // fill 1,2,3,5,7
            set_bit(data,i);
        }
    }
    else
    {
        // such as 7-21
        for(i=from-low;i<to-low+1;i+=repeat)
        {
            set_bit(data,i);
        }
    }
}
void parse_word(int * e_data,char * word,int low,int high)
{
    char * sp_comma;
    char * ranges = strtok_r(word,",",&sp_comma);
    while(ranges!=NULL)
    { 

        // handle A-B/C or N (single number or *)
        // from=A,to=B,repeat=C
        int repeat=-1;
        int from=-1,to=-1;
        char * p_slash = strstr(ranges,"/");
        if (p_slash)
        {
            repeat = atoi(p_slash+1);
            *p_slash = '\0';
        }
        // now the ranges = "A-B"

        // handle A-B or N or *
        char * hyphen = strstr(ranges,"-");
        if (hyphen)
        {
            // A-B
            *hyphen = '\0';
            from = atoi(ranges);
            to = atoi(hyphen+1);
        }
        else
        {
            // N or *
            //  * means any time
            if (*ranges == '*')
            {
                from = to = ANYTIME;
            }
            else
            {
                from = to = atoi(ranges);
            }
        }

        // set bit in job
        set_job_bit(e_data,from,to,low,high,repeat);
        // into next comma
        ranges = strtok_r(NULL, ",",&sp_comma);
    }
}
job * terry_parse_line(const char * line)
{
    // handle comment
    if (line[0] == '#')
    {
        return NULL; 
    }

    // because strtok_r() will change the original data
    // so the original data should be copied
    char * dup_line = strdup(line);
    job * e = (job * )calloc(1,sizeof(job));
    char * sp_space;
    // minute
    char  * words = strtok_r(dup_line," ",&sp_space);
    parse_word(e->min,words,0,59);

    // hour
    words = strtok_r(NULL, " ",&sp_space);
    parse_word(e->hour,words,0,23);

    // day
    words = strtok_r(NULL, " ",&sp_space);
    parse_word(e->day_of_month,words,1,31);

    // month
    words = strtok_r(NULL, " ",&sp_space);
    parse_word(e->month_of_year,words,1,12);

    // day of week
    words = strtok_r(NULL, " ",&sp_space);
    parse_word(e->day_of_week,words,0,6);

    e->script = strdup(sp_space);

    free(dup_line);
    return e;
}

int main()
{
    // test by valgrind
    // All heap blocks were freed -- no leaks are possible
    int i;
    job * j;
    char * input;

    input = "#This is comment";
    j = terry_parse_line(input);
    assert(j == NULL);
    
    input = "* * * * * /home/lfzhou/vhost/vhost";
    j = terry_parse_line(input);
    for(i=0;i<MAX_MIN;i++) assert(j->min[i] == 1);
    for(i=0;i<MAX_HOUR;i++) assert(j->hour[i] == 1);
    for(i=0;i<MAX_DOM;i++) assert(j->day_of_month[i] == 1);
    for(i=0;i<MAX_MONTH;i++) assert(j->month_of_year[i] == 1);
    for(i=0;i<MAX_DOW;i++) assert(j->day_of_week[i] == 1);
    assert(strcmp("/home/lfzhou/vhost/vhost",j->script)==0);
    free_job(j);

    input = "1 2 3 4 5 echo 222";
    j = terry_parse_line(input);
    assert(j->min[1] == 1);
    assert(j->hour[2] == 1);
    assert(j->day_of_month[2] == 1);// the start index of day of month 1 (january) is 0
    assert(j->month_of_year[3] == 1);
    assert(j->day_of_week[5] == 1);
    free_job(j);

    input = "0 21-7/2,8 * * * echo 111";
    j = terry_parse_line(input);
    // hour  is 21,23,1,3,5,7
    assert(j->min[0]==1);
    assert(j->hour[21] && j->hour[23] && j->hour[1] && j->hour[3] && j->hour[5] && j->hour[7] && j->hour[8]);
    assert(strcmp("echo 111",j->script)==0);
    free_job(j);

    input = "* * 2-10/3 * * echo 222";
    j = terry_parse_line(input);
    // day of month is 2,5,8
    // index is 1,4,7
    assert(j->day_of_month[1] && j->day_of_month[4] && j->day_of_month[7]);
    free_job(j);

    return 0;
}
