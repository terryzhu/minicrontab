#include "include.h"
#define MAX_LINE_LENGTH 4096
#define ANYTIME 0xff
// number | number "-" number [ "/" number ]
// min hour date month day_of_work
// 0 23-7/2,8 * * * echo "Have a good dream"
void debug_bit(char * buf,int max)
{
    int i;
    for(i=0;i<max;i++)
    {
        printf("%d ",buf[i]);
    }
    printf("\n");
}
void debug_entry(entry * e)
{
    printf("\n\n");
    debug_bit(e->min,MAX_MIN);
    debug_bit(e->hour,MAX_HOUR);
    debug_bit(e->dom,MAX_DOM);
    debug_bit(e->month,MAX_MONTH);
    debug_bit(e->dow,MAX_DOW);
    printf("cmd is :  %s",e->cmd);

}

void set_bit(char * buf,int index)
{
//    printf("%d ",index);
    buf[index] = 1;
}
void set_entry_bit(char * data,int from,int to,int low,int high,int repeat)
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
//     printf("from=%d to=%d low=%d high=%d repeat=%d\n",from,to,low,high,repeat);
    int i;
    int f_g_t = from > to;// from is greater than to
    if (f_g_t)
    {
        // 21-7/2
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
        for(i=from-low;i<to-low+1;i+=repeat)
        {
            set_bit(data,i);
        }
    }
}
void parse_word(char * e_data,char * word,int low,int high)
{
    char * sp_comma;
    char * ranges = strtok_r(word,",",&sp_comma);
    while(ranges!=NULL)
    { 
        printf("\n");

        // handle 23-7/2
        int repeat=-1;
        int from=-1,to=-1;
        char * p_slash = strstr(ranges,"/");
        if (p_slash)
        {
            repeat = atoi(p_slash+1);
            *p_slash = '\0';
        }
        // now the ranges = "23-7"

        // handle 23-7 or 10 or *
        char * hyphen = strstr(ranges,"-");
        if (hyphen)
        {
            // 23-7
            *hyphen = '\0';
            from = atoi(ranges);
            to = atoi(hyphen+1);
        }
        else
        {
            // 10 or *
            // 0 means *, any time
            if (*ranges == '*')
            {
                from = to = ANYTIME;
            }
            else
            {
                from = to = atoi(ranges);
            }
        }

        // set bit in entry
        set_entry_bit(e_data,from,to,low,high,repeat);
        // into next comma
        ranges = strtok_r(NULL, ",",&sp_comma);
    }
}
entry * terry_parse_line(char * line)
{
    if (line[0] == '#')
    {
        return NULL; 
    }

    entry * e = (entry * )calloc(1,sizeof(entry));
    char * sp_space;
    // handle comment
    // minute
    char  * words = strtok_r(line," ",&sp_space);
    parse_word(e->min,words,0,59);

    // hour
    words = strtok_r(NULL, " ",&sp_space);
    parse_word(e->hour,words,0,23);

    // day
    words = strtok_r(NULL, " ",&sp_space);
    parse_word(e->dom,words,1,31);

    // month
    words = strtok_r(NULL, " ",&sp_space);
    parse_word(e->month,words,1,12);

    // day of week
    words = strtok_r(NULL, " ",&sp_space);
    parse_word(e->dow,words,0,6);

    e->cmd = strdup(sp_space);
    return e;
}

int main()
{
    FILE * fp = fopen("cron.cfg","r");
    if (!fp)
    {
        printf("can't open cron.cfg file\n");
        exit(-1);
    }
    char line[MAX_LINE_LENGTH];
    while(fgets(line,MAX_LINE_LENGTH,fp))
    {
        printf("\n\n\n-----------------------------------------\n");
        printf("%s",line);
        entry * e = terry_parse_line(line);
        if(e)
        {
            debug_entry(e);
        }

        printf("\n-----------------------------------------\n");
    }
    fclose(fp);

    return 0;
}
