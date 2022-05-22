#include "stats.h"
#include <stdio.h>
#include <stdlib.h>

void make_stats(struct main_data *data)
{
    FILE *stats;

    if (NULL == (stats = fopen(data->statistics_filename, "w")))
    {
        perror(data->statistics_filename);
        exit(1);
    }

    fputs("Process Statistics:\n", stats);
    for (int i = 0; i < data->n_restaurants; i++)
    {
        fprintf(stats, "Restaurant %d prepared %d requests!\n", i, data->restaurant_stats[i]);
    }
    for (int i = 0; i < data->n_drivers; i++)
    {
        fprintf(stats, "Driver %d delivered %d requests!\n", i, data->driver_stats[i]);
    }
    for (int i = 0; i < data->n_clients; i++)
    {
        fprintf(stats, "Client %d received %d requests!\n", i, data->client_stats[i]);
    }
    fputs("\nRequest Statistics:", stats);
    for (int i = 0; i < data->results[i]; i++)
    {
        fprintf(stats, "Request : %d\n", data->results[i].id);
        fprintf(stats, "Status: %d\n", data->results[i].status);
        fprintf(stats, "Restaurant id %d:\n", data->results[i].requested_rest);
        fprintf(stats, "Driver id: %d", data->results[i].receiving_driver);
        fprintf(stats, "Client id: %d", data->results[i].receiving_client);

        fprintf(stats, "Created: %d", data->results[i].start_time);
        fprintf(stats, "Restaurant time: %d", data->results[i].rest_time);
        fprintf(stats, "Driver time: %d", data->results[i].driver_time);
        fprintf(stats, "Client time: %d", data->results[i].client_end_time);
        fprintf(stats, "Total Time: %d", 10);
    }
    fclose(stats);
}