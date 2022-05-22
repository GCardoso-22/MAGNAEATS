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
    char linha[MAXLINHA];
    char ipt[8][MAXLINHA];

    FILE *file;
    file = fopen(nomeFicheiro, "r");
    if ((file) == NULL)
    {
        printf("Ficheiro de configuração nao existe.\n");
        exit(1);
    }
    else
    {
        while (fgets(linha, MAXLINHA, file) != NULL)
        {
            int i = 0;
            strcpy(ipt[i], linha);
            data->max_ops = atoi(ipt[0]);
            data->buffers_size = atoi(ipt[1]);
            data->n_clients = atoi(ipt[2]);
            data->n_restaurants = atoi(ipt[3]);
            data->n_drivers = atoi(ipt[4]);
            data->log_filename = ipt[5];
            data->statistics_filename = ipt[6];
            data->alarm_time = atoi(ipt[7]);
            i++;
        }
        fclose(file);
    }
}