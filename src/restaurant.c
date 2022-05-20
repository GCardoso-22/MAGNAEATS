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
#include "synchronization.h"
#include "metime.h"

int execute_restaurant(int rest_id, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    struct operation op;
    while (1)
    {
        restaurant_receive_operation(&op, rest_id, buffers, data, sems);

        if (op.id == -1)
        {
            // ignored
        }
        else if (*data->terminate == 0)
        {
            restaurant_process_operation(&op, rest_id, data, data->restaurant_stats, sems);
            restaurant_forward_operation(&op, buffers, data, sems);
        }
        else if (*data->terminate == 1)
        {
            return *data->restaurant_stats;
        }
    }
}

void restaurant_receive_operation(struct operation *op, int rest_id, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    consume_begin(sems->main_rest);
    if (*data->terminate == 1)
    {
        return;
    }
    else
    {
        read_main_rest_buffer(buffers->main_rest, rest_id, data->buffers_size, op);
        consume_end(sems->main_rest);
        set_rest_time(op);
    }
}

void restaurant_process_operation(struct operation *op, int rest_id, struct main_data *data, int *counter, struct semaphores *sems)
{
    op->receiving_rest = rest_id;
    op->status = 'R';
    (*counter)++;
    data->results[op->id] = *op;
}

void restaurant_forward_operation(struct operation *op, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    produce_begin(sems->rest_driv);
    write_rest_driver_buffer(buffers->rest_driv, data->buffers_size, op);
    produce_end(sems->rest_driv);
}