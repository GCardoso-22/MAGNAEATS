/* SO-043
Diogo Chambel - 53319
Gonçalo Cardoso - 54415
Pedro Correia - 54570
*/
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void make_log(struct main_data *data, char *intruction, char argument)
{
    FILE *log;
    struct timespec clock;

    if (clock_gettime(CLOCK_REALTIME, &clock) == -1)
    {
        perror("clock gettime");
        exit(EXIT_FAILURE);
    }

    if (NULL == (log = fopen(data->log_filename, "a+")))
    {
        perror(data->log_filename);
        exit(1);
    }
    char realtime[30];
    struct tm *timeinfo;
    timeinfo = localtime(&clock.tv_sec);

    if (strcmp(intruction, "status") == 0)
    {
        sprintf(realtime, "%d-%d-%d %d:%d:%d.%ld %s %d", timeinfo->tm_mday,
                timeinfo->tm_mon + 1, timeinfo->tm_year + 1900,
                timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, clock.tv_nsec, intruction, argument);
    }
    else
    {
        sprintf(realtime, "%d-%d-%d %d:%d:%d.%ld %s", timeinfo->tm_mday,
                timeinfo->tm_mon + 1, timeinfo->tm_year + 1900,
                timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, clock.tv_nsec, intruction);
    }

    fprintf(log, "%s\n", realtime);
    fclose(log);
}