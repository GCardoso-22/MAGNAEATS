/* SO-043
Diogo Chambel - 53319
Gonçalo Cardoso - 54415
Pedro Correia - 54570
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "memory.h"
#include "main.h"
#include "process.h"

int launch_restaurant(int restaurant_id, struct communication_buffers *buffers, struct main_data *data)
{
    restaurant_id = fork();
}

int launch_driver(int driver_id, struct communication_buffers *buffers, struct main_data *data)
{
}

int launch_client(int client_id, struct communication_buffers *buffers, struct main_data *data)
{
}

int wait_process(int process_id)
{
}
