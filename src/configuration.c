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
    char aux[8][25];

    FILE *file;
    file = fopen(nomeFicheiro, "r");
    if ((file) == NULL)
    {
        printf("Ficheiro de configuração nao existe.\n");
        exit(1);
    }
    else
    {

        int i = 0;
        while (fgets(linha, MAXLINHA, file) != NULL)
        {
            strcpy(aux[i], linha);
            i++;
        }        
            
         
        data->max_ops = atoi(aux[0]);
        data->buffers_size = atoi(aux[1]);
        data->n_clients = atoi(aux[2]);
        data->n_restaurants = atoi(aux[3]);
        data->n_drivers = atoi(aux[4]);
        data->log_filename = "log.txt";
        data->statistics_filename = "statistics.txt";
        data->alarm_time = atoi(aux[7]);
           
            

           
        fclose(file);
    }
}