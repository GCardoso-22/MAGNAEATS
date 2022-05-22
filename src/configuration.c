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

            switch(i){
                
                case 0:
                    data->max_ops = atoi(linha);
                    break;
                case 1:
                    data->buffers_size = atoi(linha);
                    break;
                case 2:
                    data->n_clients = atoi(linha);
                    break;
                case 3:
                    data->n_restaurants = atoi(linha);
                    break;
                case 4:
                    data->n_drivers = atoi(linha);
                    break;
                case 5:
                    data->log_filename = linha;
                    break;
                
                case 6:
                    data->statistics_filename = linha;
                    break;
                case 7:
                    data->alarm_time = atoi(linha);
                    break;
                default:
                    printf("As configurações apenas esperam 8 linhas, todas as linhas posteriores serão ignoradas.\n");
            }
            i++;

            /**
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

            */
        }
        fclose(file);
    }
}