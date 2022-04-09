#include "main.h"
#include "memory.h"
#include "process.h"
#include "client.h"
#include "driver.h"
#include "restaurant.h"
#include <string.h>
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
	if (argc == 6)
	{
		data->max_ops = atoi(argv[1]);
		data->buffers_size = atoi(argv[2]);
		data->n_clients = atoi(argv[3]);
		data->n_restaurants = atoi(argv[4]);
		data->n_drivers = atoi(argv[5]);
	}
	else
	{
		printf("Insira os dados necessários");
		printf("magnaeats max_ops buffers_size n_restaurants n_drivers n_clients");
		exit(0);
	}
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
	// clients
	for (int c = 0; c < data->n_clients; c++)
	{
		data->client_pids[c] = launch_client(c, buffers, data);
	}
	// drivers
	for (int d = 0; d < data->n_drivers; d++)
	{
		data->driver_pids[d] = launch_driver(d, buffers, data);
	}
	// restaurants
	for (int r = 0; r < data->n_restaurants; r++)
	{
		data->restaurant_pids[r] = launch_restaurant(r, buffers, data);
	}
}

void user_interaction(struct communication_buffers *buffers, struct main_data *data)
{
	char cmd[8];
	int *opCounter = 0;
	printf("Modo de funcionamento:\n");
	printf("	request <cliente> <restaurante> <prato> - fazer uma encomenda.\n");
	printf("	status <id> - consultar o estado do pedido especificado pelo id.\n");
	printf("	stop - terminar a execução do MagnaEats.\n");
	printf("	help - informações de ajuda sobre as várias opções.\n");
	// falta acabar
	while (1)
	{
		printf("Introduza uma opção:\n");
		scanf("%s", cmd);
		if (strcasecmp(cmd, "request") == 0)
		{
			create_request(opCounter, buffers, data);
		}
		else if (strcasecmp(cmd, "status") == 0)
		{
			read_status(data);
		}
		else if (strcasecmp(cmd, "stop") == 0)
		{
			stop_execution(data, buffers);
		}
		else if (strcasecmp(cmd, "help") == 0)
		{
			printf("Modo de funcionamento:\n");
			printf("	request <cliente> <restaurante> <prato> - fazer uma encomenda.\n");
			printf("	status <id> - consultar o estado do pedido especificado pelo id.\n");
			printf("	stop - terminar a execução do MagnaEats.\n");
			printf("	help - informações de ajuda sobre as várias opções.\n");
		}
		else
		{
			printf("Comando invalido, introduza \"help\" para ver as opçẽs disponiveis.\n");
		}
	}
}

void create_request(int *op_counter, struct communication_buffers *buffers, struct main_data *data)
{
	if (*op_counter < data->max_ops)
	{
		struct operation *newOp;
		newOp->id = *op_counter;

		write_main_rest_buffer(buffers->main_rest, data->buffers_size, newOp);
		printf("Pedido #%d concluido!\n", *op_counter);
		printf("O pedido #%d foi criado!\n", *op_counter);
		*op_counter++;
	}
	else
	{
		printf("O número máximo de pedidos foi alcançado!\n");
	}
}

void read_status(struct main_data *data)
{
	int op;
	scanf("%d", op);
	for (int i = 0; i <= data->max_ops; i++)
	{
		if (op == i)
		{
			printf("Op %d com estado %c foi ", i, data->results[i].status);
			printf("pedida pelo cliente %d, ", data->results[i].requesting_client);
			printf("requesitada pelo restaurante %d, ", data->results[i].requested_rest);
			printf("com o nome do prato pedido %d!", data->results[i].requested_dish);
			printf("foi recebido pelo restaurante %d, ", data->results[i].receiving_rest);
			printf("entregue pelo motorista %d, ", data->results[i].receiving_driver);
			printf("e recebido pelo cliente %d!\n", data->results[i].receiving_client);
			break;
		}
		else if (i == data->max_ops)
		{
			printf("Op %ls ainda não é valida\n", op);
			break;
		}
	}
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
	// clients
	for (int c = 0; c < data->n_clients; c++)
	{
		data->client_stats[c] = wait_process(data->client_pids[c]);
	}
	// drivers
	for (int d = 0; d < data->n_drivers; d++)
	{
		data->driver_stats[d] = wait_process(data->driver_pids[d]);
	}
	// restaurants
	for (int r = 0; r < data->n_restaurants; r++)
	{
		data->restaurant_stats[r] = wait_process(data->restaurant_stats[r]);
	}
}

void write_statistics(struct main_data *data)
{
	printf("Processar estatísticas:\n");
	// clients
	for (int c = 0; c < data->n_restaurants; c++)
	{
		printf("Restaurante %d encaminhou %d pedidos!\n", c, data->restaurant_stats[c]);
	}
	// drivers
	for (int d = 0; d < data->n_drivers; d++)
	{
		printf("Mororista %d respondeu %d pedidos!\n", d, data->driver_stats[d]);
	}
	// restaurants
	for (int r = 0; r < data->n_clients; r++)
	{
		printf("O Cliente %d recebeu %d pedidos!\n", r, data->client_stats[r]);
	}
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