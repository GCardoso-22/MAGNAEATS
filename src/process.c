/* SO-043
Diogo Chambel - 53319
Gonçalo Cardoso - 54415
Pedro Correia - 54570
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "memory.h"
#include "main.h"
#include "process.h"
#include "restaurant.h"
#include "client.h"
#include "driver.h"

/* Função que inicia um novo processo restaurante através da função fork do SO. O novo
 * processo irá executar a função execute_restaurant respetiva, fazendo exit do retorno.
 * O processo pai devolve o pid do processo criado.
 */
int launch_restaurant(int restaurant_id, struct communication_buffers *buffers, struct main_data *data)
{
    int pid = fork();
    if (pid == -1)
    {
        perror("pid");
        exit(1);
    }

    if (pid == 0)
    {
        exit(execute_restaurant(restaurant_id, buffers, data));
    }
    else
    {
        return pid;
    }
}

int launch_driver(int driver_id, struct communication_buffers *buffers, struct main_data *data)
{

    int pid = fork();
    if (pid == -1)
    {
        perror("pid");
        exit(1);
    }

    if (pid == 0)
    {
        exit(execute_driver(driver_id, buffers, data));
    }
    else
    {
        return pid;
    }
}

int launch_client(int client_id, struct communication_buffers *buffers, struct main_data *data)
{

    int pid = fork();
    if (pid == -1)
    {
        perror("pid");
        exit(1);
    }

    if (pid == 0)
    {
        exit(execute_client(client_id, buffers, data));
    }
    else
    {
        return pid;
    }
}

int wait_process(int process_id)
{
    int status;

    waitpid(process_id, &status, 0);

    if (WIFEXITED(status))
    {
        return WEXITSTATUS(status);
    }
    return -1;
}