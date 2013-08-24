#include "include.h"
#define MAX_LINE_LENGTH 4096
// number | number "-" number [ "/" number ]
// 0 23-7/2,8 * * * echo "Have a good dream"
entry * parse_line(char * line)
{
    int i = 0;
    char * sp_space;
    // handle comment
    if (line[i] == '#')
    {
        return NULL; 
    }
    // first level, split by space
    char  * words = strtok_r(line," ",&sp_space);
    int count = 0;// max is 5
    while(words!=NULL && count++ < 5)
    {
        // second level, split by comma
        char * sp_comma;
        char * ranges = strtok_r(words,",",&sp_comma);
        while(ranges!=NULL)
        { 
            printf("\n{%s}\n",ranges);


            // handle 23-7/2
            int repeat=-1;
            int from=-1,to=-1;
            char * p_slash = strstr(ranges,"/");
            if (p_slash)
            {
                repeat = atoi(p_slash+1);
                printf("repeat in %d\n",repeat);
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
                printf("from=%d,to=%d\n",from,to);
            }
            else
            {
                // 10 or *
                // 0 means *, any time
                from = to = atoi(ranges);
                printf("from_to=%d\n",from);
            }

            // TODO
            // set bit in entry

            // into next comma
            ranges = strtok_r(NULL, ",",&sp_comma);
        }
        // into next words
        words = strtok_r(NULL, " ",&sp_space);
    }
    printf("\n");
    return NULL;
}

int main()
{
    FILE * fp = fopen("cron.cfg","r");
    if (!fp)
    {
        printf("can't open cfg file\n");
        exit(-1);
    }
    char line[MAX_LINE_LENGTH];
    while(fgets(line,MAX_LINE_LENGTH,fp))
    {
        printf("%s",line);
        parse_line(line);
    }

    return 0;
}
