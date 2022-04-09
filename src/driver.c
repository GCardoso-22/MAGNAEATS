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
}

void driver_receive_operation(struct operation *op, struct communication_buffers *buffers, struct main_data *data)
{
}

void driver_process_operation(struct operation *op, int driver_id, struct main_data *data, int *counter)
{
}

void driver_send_answer(struct operation *op, struct communication_buffers *buffers, struct main_data *data)
{
}
