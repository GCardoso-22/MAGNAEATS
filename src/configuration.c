/* SO-043
Diogo Chambel - 53319
Gonçalo Cardoso - 54415
Pedro Correia - 54570
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "configuration.h"

void config(char *nomeFicheiro, struct main_data *data)
{
    FILE *file;
    file = fopen(nomeFicheiro, "r");
    if ((file) != NULL)
    {
        char linha[MAXLINHA];
        char arg[8][MAXLINHA];

        for (int i = 0; fgets(linha, MAXLINHA, file) != NULL; i++)
        {
            strcpy(arg[i], linha);

            data->max_ops = atoi(arg[0]);
            data->buffers_size = atoi(arg[1]);
            data->n_clients = atoi(arg[2]);
            data->n_restaurants = atoi(arg[3]);
            data->n_drivers = atoi(arg[4]);
            // log_filename = atoi(arg[5]);
            // statistics_filename = atoi(arg[6]);
            // alarm_time = atoi(arg[7]);
            fclose(file);
        }
    }
    else
    {
        printf("Ficheiro de configuração nao existe.\n");
        exit(1);
    }
}