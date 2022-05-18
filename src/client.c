/* SO-043
Diogo Chambel - 53319
Gonçalo Cardoso - 54415
Pedro Correia - 54570
*/

#include <stdio.h>
#include <stdlib.h>
#include "client.h"

int execute_client(int client_id, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    struct operation op;

    while (1)
    {
        client_get_operation(&op, client_id, buffers, data, sems);

        if (op.id == -1)
        {
            // ignored
        }
        else if (*data->terminate == 0)
        {
            client_process_operation(&op, client_id, data, data->client_stats, sems);
        }
        else if (*data->terminate == 1)
        {
            return *data->client_stats;
        }
    }
}

void client_get_operation(struct operation *op, int client_id, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    consume_begin(sems->driv_cli);
    if (*data->terminate == 1)
    {
        return;
    }
    else
    {
        read_driver_client_buffer(buffers->driv_cli, client_id, data->buffers_size, op);
        consume_end(sems->driv_cli);
    }
}

void client_process_operation(struct operation *op, int client_id, struct main_data *data, int *counter, struct semaphores *sems)
{
    op->receiving_client = client_id;
    op->status = 'C';
    (*counter)++;
    data->results[op->id] = *op;
}
