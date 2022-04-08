/* SO-043
Diogo Chambel - 53319
Gonçalo Cardoso - 54415
Pedro Correia - 54570
*/

#include <stdio.h>
#include <stdlib.h>

#include "../include/client.h"

int execute_client(int client_id, struct communication_buffers *buffers, struct main_data *data)
{
    struct operation op;

    while (1)
    {
        client_get_operation(&op, client_id, buffers, data);

        if (op.id == -1)
        {
            // do nothing
        }
        else if (data->terminate == 0)
        {
            client_process_operation(&op, client_id, data, data->client_stats);
        }
        else if (data->terminate == 1)
        {
            return data->client_stats;
        }
    }
}

void client_get_operation(struct operation *op, int client_id, struct communication_buffers *buffers, struct main_data *data)
{
    if (data->terminate == 1)
    {
        return;
    }
    read_driver_client_buffer(buffers->driv_cli, client_id, data->buffers_size, op);
}

void client_process_operation(struct operation *op, int client_id, struct main_data *data, int *counter)
{
    op->receiving_client = client_id;
    op->status = 'C';
    *counter++;
    data->results[op->id] = *op;
}
