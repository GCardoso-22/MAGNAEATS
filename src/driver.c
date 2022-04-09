/* SO-043
Diogo Chambel - 53319
Gonçalo Cardoso - 54415
Pedro Correia - 54570
*/

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "main.h"
#include "driver.h"

int execute_driver(int driver_id, struct communication_buffers *buffers, struct main_data *data)
{
    struct operation op;
    while (1)
    {
        driver_receive_operation(&op, buffers, data);

        if (op.id == -1)
        {
            // ignored
        }
        else if (data->terminate == 0)
        {
            driver_process_operation(&op, driver_id, data, data->driver_stats);
            driver_send_answer(&op, buffers, data);
        }
        else if (data->terminate == 1)
        {
            return data->driver_stats;
        }
    }
}

void driver_receive_operation(struct operation *op, struct communication_buffers *buffers, struct main_data *data)
{
    if (data->terminate == 1)
    {
        return;
    }
    else
    {
        read_rest_driver_buffer(buffers->rest_driv, data->buffers_size, op);
    }
}

void driver_process_operation(struct operation *op, int driver_id, struct main_data *data, int *counter)
{
    op->receiving_driver = driver_id;
    op->status = 'D';
    *counter++;
    data->results[op->id] = *op;
}

void driver_send_answer(struct operation *op, struct communication_buffers *buffers, struct main_data *data)
{
    write_driver_client_buffer(buffers->driv_cli, data->buffers_size, op);
}
