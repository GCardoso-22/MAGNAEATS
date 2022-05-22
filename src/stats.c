#include "stats.h"
#include <stdio.h>
#include <stdlib.h>

void make_stats(struct main_data *data, char *intruction, char argument)
{
    FILE *stats;

    if (NULL == (stats = fopen("stats.txt" /*data->log_filename*/, "w")))
    {
        perror("stats.txt");
        exit(1);
    }

    fprintf(log, "%s\n", realtime);
    fclose(stats);
}