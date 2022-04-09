#include "main.h"
#include "memory.h"
#include "process.h"
#include "client.h"
#include "driver.h"
#include "restaurant.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	// init data structures
	struct main_data *data = create_dynamic_memory(sizeof(struct main_data));
	struct communication_buffers *buffers = create_dynamic_memory(sizeof(struct communication_buffers));
	buffers->main_rest = create_dynamic_memory(sizeof(struct rnd_access_buffer));
	buffers->rest_driv = create_dynamic_memory(sizeof(struct circular_buffer));
	buffers->driv_cli = create_dynamic_memory(sizeof(struct rnd_access_buffer));

	// execute main code
	main_args(argc, argv, data);
	create_dynamic_memory_buffers(data);
	create_shared_memory_buffers(data, buffers);
	launch_processes(buffers, data);
	user_interaction(buffers, data);

	// release memory before terminating
	destroy_dynamic_memory(data);
	destroy_dynamic_memory(buffers->main_rest);
	destroy_dynamic_memory(buffers->rest_driv);
	destroy_dynamic_memory(buffers->driv_cli);
	destroy_dynamic_memory(buffers);
}

void main_args(int argc, char *argv[], struct main_data *data)
{
	// TODO
}

void create_dynamic_memory_buffers(struct main_data *data)
{
	data->client_pids = create_dynamic_memory(sizeof(int));
	data->driver_pids = create_dynamic_memory(sizeof(int));
	data->restaurant_pids = create_dynamic_memory(sizeof(int));

	data->client_stats = create_dynamic_memory(sizeof(int));
	data->driver_stats = create_dynamic_memory(sizeof(int));
	data->restaurant_pids = create_dynamic_memory(sizeof(int));
}

void create_shared_memory_buffers(struct main_data *data, struct communication_buffers *buffers)
{
	buffers->driv_cli->buffer = create_shared_memory(STR_SHM_DRIVER_CLIENT_BUFFER, data->buffers_size * sizeof(struct operation));
	buffers->driv_cli->ptrs = create_shared_memory(STR_SHM_DRIVER_CLIENT_PTR, data->buffers_size * sizeof(int));

	buffers->main_rest->buffer = create_shared_memory(STR_SHM_MAIN_REST_BUFFER, data->buffers_size * sizeof(struct operation));
	buffers->main_rest->ptrs = create_shared_memory(STR_SHM_MAIN_REST_PTR, data->buffers_size * sizeof(int));

	buffers->rest_driv->buffer = create_shared_memory(STR_SHM_REST_DRIVER_BUFFER, data->buffers_size * sizeof(struct operation));
	buffers->rest_driv->ptrs = create_shared_memory(STR_SHM_REST_DRIVER_PTR, data->buffers_size * sizeof(int));

	data->results = create_shared_memory(STR_SHM_RESULTS, data->buffers_size * sizeof(struct operation));
	data->terminate = create_shared_memory(STR_SHM_TERMINATE, sizeof(int *));
}

void launch_processes(struct communication_buffers *buffers, struct main_data *data)
{
	// TODO
}

void user_interaction(struct communication_buffers *buffers, struct main_data *data)
{
	printf("Modo de funcionamento:\n");
	printf("	request <cliente> <restaurante> <prato> - fazer uma encomenda.\n");
	printf("	status <id> - consultar o estado do pedido especificado pelo id.\n");
	printf("	stop - terminar a execução do MagnaEats.\n");
	printf("	help - informações de ajuda sobre as várias opções.\n");
	// falta acabar
}

void create_request(int *op_counter, struct communication_buffers *buffers, struct main_data *data)
{
	// TODO
}

void read_status(struct main_data *data)
{ // todo
}

void stop_execution(struct main_data *data, struct communication_buffers *buffers)
{
	*data->terminate = 1;
	wait_processes(data);
	write_statistics(data);
	destroy_memory_buffers(data, buffers);
}

void wait_processes(struct main_data *data)
{
	// TODO
}

void write_statistics(struct main_data *data)
{
	// TODO
}

void destroy_memory_buffers(struct main_data *data, struct communication_buffers *buffers)
{
	destroy_dynamic_memory(data->client_pids);
	destroy_dynamic_memory(data->driver_pids);
	destroy_dynamic_memory(data->restaurant_pids);

	destroy_dynamic_memory(data->client_stats);
	destroy_dynamic_memory(data->driver_stats);
	destroy_dynamic_memory(data->restaurant_pids);

	destroy_shared_memory(STR_SHM_DRIVER_CLIENT_BUFFER, buffers->driv_cli->buffer, data->buffers_size * sizeof(struct operation));
	destroy_shared_memory(STR_SHM_DRIVER_CLIENT_PTR, buffers->driv_cli->ptrs, data->buffers_size * sizeof(int));

	destroy_shared_memory(STR_SHM_MAIN_REST_BUFFER, buffers->main_rest->buffer, data->buffers_size * sizeof(struct operation));
	destroy_shared_memory(STR_SHM_MAIN_REST_PTR, buffers->main_rest->ptrs, data->buffers_size * sizeof(int));

	destroy_shared_memory(STR_SHM_REST_DRIVER_BUFFER, buffers->rest_driv->buffer, data->buffers_size * sizeof(struct operation));
	destroy_shared_memory(STR_SHM_REST_DRIVER_PTR, buffers->rest_driv->buffer, data->buffers_size * sizeof(int));

	destroy_shared_memory(STR_SHM_RESULTS, data->results, data->buffers_size * sizeof(struct operation));
	destroy_shared_memory(STR_SHM_TERMINATE, data->terminate, sizeof(int *));
}