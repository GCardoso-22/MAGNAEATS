/* SO-043
Diogo Chambel - 53319
Gonçalo Cardoso - 54415
Pedro Correia - 54570
*/

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "main.h"
#include "restaurant.h"

int execute_restaurant(int rest_id, struct communication_buffers *buffers, struct main_data *data)
{
    struct operation op;
    while (1)
    {
        restaurant_receive_operation(&op, rest_id, buffers, data);

        if (op.id == -1)
        {
            // ignored
        }
        else if (data->terminate == 0)
        {
            restaurant_process_operation(&op, rest_id, data, data->restaurant_stats);
            restaurant_forward_operation(&op, buffers, data);
        }
        else if (data->terminate == 1)
        {
            return data->restaurant_stats;
        }
    }
}

void restaurant_receive_operation(struct operation *op, int rest_id, struct communication_buffers *buffers, struct main_data *data)
{
    if (data->terminate == 1)
    {
        return;
    }
    else
    {
        read_main_rest_buffer(buffers->main_rest, rest_id, data->buffers_size, op);
    }
}

void restaurant_process_operation(struct operation *op, int rest_id, struct main_data *data, int *counter)
{
    op->receiving_rest = rest_id;
    op->status = 'R';
    *counter++;
    data->results[op->id] = *op;
}

void restaurant_forward_operation(struct operation *op, struct communication_buffers *buffers, struct main_data *data)
{
    write_rest_driver_buffer(buffers->rest_driv, data->buffers_size, op);
}