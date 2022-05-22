/* SO-043
Diogo Chambel - 53319
Gonçalo Cardoso - 54415
Pedro Correia - 54570
*/
#include "main.h"
#include "memory.h"
#include "process.h"
#include "client.h"
#include "driver.h"
#include "restaurant.h"
#include "synchronization.h"
#include "configuration.h"
#include "metime.h"
#include "log.h"
#include "mesignal.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

struct main_data *data;
struct communication_buffers *buffers;
struct semaphores *sems;

void ctrlC()
{
	stop_execution(data, buffers, sems);
}

int main(int argc, char *argv[])
{
	// init data structures
	struct main_data *data = create_dynamic_memory(sizeof(struct main_data));
	struct communication_buffers *buffers = create_dynamic_memory(sizeof(struct communication_buffers));
	buffers->main_rest = create_dynamic_memory(sizeof(struct rnd_access_buffer));
	buffers->rest_driv = create_dynamic_memory(sizeof(struct circular_buffer));
	buffers->driv_cli = create_dynamic_memory(sizeof(struct rnd_access_buffer));

	// init semaphore data structure
	struct semaphores *sems = create_dynamic_memory(sizeof(struct semaphores));
	sems->main_rest = create_dynamic_memory(sizeof(struct prodcons));
	sems->rest_driv = create_dynamic_memory(sizeof(struct prodcons));
	sems->driv_cli = create_dynamic_memory(sizeof(struct prodcons));

	// execute main code
	main_args(argc, argv, data);
	create_dynamic_memory_buffers(data);
	create_shared_memory_buffers(data, buffers);
	create_semaphores(data, sems);
	launch_processes(buffers, data, sems);
	user_interaction(buffers, data, sems);

	// release memory before terminating
	destroy_dynamic_memory(data);
	destroy_dynamic_memory(buffers->main_rest);
	destroy_dynamic_memory(buffers->rest_driv);
	destroy_dynamic_memory(buffers->driv_cli);
	destroy_dynamic_memory(buffers);
	destroy_dynamic_memory(sems->main_rest);
	destroy_dynamic_memory(sems->rest_driv);
	destroy_dynamic_memory(sems->driv_cli);
	destroy_dynamic_memory(sems);
}

void main_args(int argc, char *argv[], struct main_data *data)
{
	if (argc == 7)
	{
		data->max_ops = atoi(argv[1]);
		data->buffers_size = atoi(argv[2]);
		data->n_clients = atoi(argv[3]);
		data->n_restaurants = atoi(argv[4]);
		data->n_drivers = atoi(argv[5]);
		data->log_filename = argv[5];
		// data->statistics_filename = ipt[6];
		// data->alarm_time = atoi(ipt[7]);
	}
	else
	{
		printf("Insira os dados necessários");
		printf("magnaeats max_ops buffers_size n_restaurants n_drivers n_clients\n");
		exit(0);

		/*
		if (argc != 1)
		{
			config(argv[1], data);
		}
		else
		{
			printf("Insira um ficheiro com as configuaracoes necessarias\n");
			exit(0);
			*/
	}
}

void create_dynamic_memory_buffers(struct main_data *data)
{
	data->client_pids = create_dynamic_memory(sizeof(int));
	data->driver_pids = create_dynamic_memory(sizeof(int));
	data->restaurant_pids = create_dynamic_memory(sizeof(int));

	data->client_stats = create_dynamic_memory(sizeof(int));
	data->driver_stats = create_dynamic_memory(sizeof(int));
	data->restaurant_stats = create_dynamic_memory(sizeof(int));
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

void launch_processes(struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
	// clients
	for (int c = 0; c < data->n_clients; c++)
	{
		data->client_pids[c] = launch_client(c, buffers, data, sems);
	}
	// drivers
	for (int d = 0; d < data->n_drivers; d++)
	{
		data->driver_pids[d] = launch_driver(d, buffers, data, sems);
	}
	// restaurants
	for (int r = 0; r < data->n_restaurants; r++)
	{
		data->restaurant_pids[r] = launch_restaurant(r, buffers, data, sems);
	}
}

void user_interaction(struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
	char cmd[8];
	int opCounter = 0;
	printf("\nModo de funcionamento:\n");
	printf("	request <cliente> <restaurante> <prato> - fazer uma encomenda.\n");
	printf("	status <id> - consultar o estado do pedido especificado pelo id.\n");
	printf("	stop - terminar a execução do MagnaEats.\n");
	printf("	help - informações de ajuda sobre as várias opções.\n");
	// falta acabar
	while (1)
	{
		signal(SIGINT, ctrlC);
		printf("Introduza uma opção:\n");
		scanf("%s", cmd);
		if (strcasecmp(cmd, "request") == 0)
		{
			// printf("banger pedido\n");
			create_request(&opCounter, buffers, data, sems);
		}
		else if (strcasecmp(cmd, "status") == 0)
		{
			read_status(data, sems);
		}
		else if (strcasecmp(cmd, "stop") == 0)
		{
			// printf("Parei (menti)\n");
			stop_execution(data, buffers, sems);
			break;
		}
		else if (strcasecmp(cmd, "help") == 0)
		{
			printf("\nModo de funcionamento:\n");
			printf("	request <cliente> <restaurante> <prato> - fazer uma encomenda.\n");
			printf("	status <id> - consultar o estado do pedido especificado pelo id.\n");
			printf("	stop - terminar a execução do MagnaEats.\n");
			printf("	help - informações de ajuda sobre as várias opções.\n");
			make_log(data, cmd, 0);
		}
		else
		{
			printf("Comando invalido, introduza \"help\" para ver as opçẽs disponiveis.\n");
		}
	}
}

void create_request(int *op_counter, struct communication_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
	// printf("cringe\n");
	// printf("%d\n", *op_counter);
	if (*op_counter < data->max_ops)
	{
		int requestingClient = 0;
		int requestedRest = 0;
		char requestedDish[20];
		struct operation op;

		// printf("scan buffo\n");
		scanf("%d %d %20s", &requestingClient, &requestedRest, requestedDish);
		// printf("dps can\n");
		set_start_time(&op);
		op.id = *op_counter;
		op.requesting_client = requestingClient;
		op.requested_rest = requestedRest;
		op.requested_dish = requestedDish;
		// printf("%d\n", op.id);
		// printf("%d\n", op.requesting_client);
		// printf("%d\n", op.requested_rest);
		// printf("%s\n", op.requested_dish);

		produce_begin(sems->main_rest);
		write_main_rest_buffer(buffers->main_rest, data->buffers_size, &op);
		produce_end(sems->main_rest);

		printf("O pedido #%d foi criado!\n", *op_counter);
		(*op_counter)++;
		make_log(data, "request", 0);
	}
	else
	{
		printf("O número máximo de pedidos foi alcançado!\n");
	}
}

void read_status(struct main_data *data, struct semaphores *sems)
{
	int id;
	scanf("%d", &id);
	semaphore_mutex_lock(sems->results_mutex);
	int check = 0;

	for (int i = 0; i < data->max_ops; i++)
	{
		if (id == data->results[i].id)
		{
			printf("Pedido #%d com estado %c requisitado pelo cliente %d ao restaurante %d com o prato %s,ainda não foi recebido no restaurante!\n",
				   id, data->results[id].status, data->results[id].requesting_client, data->results[id].requested_rest, data->results[id].requested_dish);
			check = 1;
			make_log(data, "status", i);
		}
	}
	if (check != 1)
	{
		printf("O pedido %d ainda não é válido", id);
		semaphore_mutex_unlock(sems->results_mutex);
	}
}

void stop_execution(struct main_data *data, struct communication_buffers *buffers, struct semaphores *sems)
{
	// printf("yo\n");
	*(data->terminate) = 1;
	// printf("yo\n");
	wakeup_processes(data, sems);
	// printf("yo\n");
	wait_processes(data);
	// printf("yo1\n");
	write_statistics(data);
	// printf("yo2");
	destroy_semaphores(sems);
	// printf("yo3\n");
	destroy_memory_buffers(data, buffers);
	// printf("yo4\n");
	make_log(data, "stop", 0);
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
		data->restaurant_stats[r] = wait_process(data->restaurant_pids[r]);
	}
}

void write_statistics(struct main_data *data)
{
	printf("Terminando o MAGNAEATS! Imprimindo estatísticas:\n");
	// clients
	for (int c = 0; c < data->n_restaurants; c++)
	{
		printf("Restaurante %d preparou %d pedidos!\n", c, data->restaurant_stats[c]);
	}
	// drivers
	for (int d = 0; d < data->n_drivers; d++)
	{
		printf("Motorista %d entregou %d pedidos!\n", d, data->driver_stats[d]);
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
	destroy_dynamic_memory(data->restaurant_stats);

	destroy_shared_memory(STR_SHM_DRIVER_CLIENT_BUFFER, buffers->driv_cli->buffer, data->buffers_size * sizeof(struct operation));
	destroy_shared_memory(STR_SHM_DRIVER_CLIENT_PTR, buffers->driv_cli->ptrs, data->buffers_size * sizeof(int));

	destroy_shared_memory(STR_SHM_MAIN_REST_BUFFER, buffers->main_rest->buffer, data->buffers_size * sizeof(struct operation));
	destroy_shared_memory(STR_SHM_MAIN_REST_PTR, buffers->main_rest->ptrs, data->buffers_size * sizeof(int));

	destroy_shared_memory(STR_SHM_REST_DRIVER_BUFFER, buffers->rest_driv->buffer, data->buffers_size * sizeof(struct operation));
	destroy_shared_memory(STR_SHM_REST_DRIVER_PTR, buffers->rest_driv->buffer, data->buffers_size * sizeof(int));

	destroy_shared_memory(STR_SHM_RESULTS, data->results, data->buffers_size * sizeof(struct operation));
	destroy_shared_memory(STR_SHM_TERMINATE, data->terminate, sizeof(int *));
}

void create_semaphores(struct main_data *data, struct semaphores *sems)
{
	sems->main_rest->full = semaphore_create(STR_SEM_MAIN_REST_FULL, 0);
	sems->main_rest->empty = semaphore_create(STR_SEM_MAIN_REST_EMPTY, data->buffers_size);
	sems->main_rest->mutex = semaphore_create(STR_SEM_MAIN_REST_MUTEX, 1);

	sems->rest_driv->full = semaphore_create(STR_SEM_REST_DRIV_FULL, 0);
	sems->rest_driv->empty = semaphore_create(STR_SEM_REST_DRIV_EMPTY, data->buffers_size);
	sems->rest_driv->mutex = semaphore_create(STR_SEM_REST_DRIV_MUTEX, 1);

	sems->driv_cli->full = semaphore_create(STR_SEM_DRIV_CLI_FULL, 0);
	sems->driv_cli->empty = semaphore_create(STR_SEM_DRIV_CLI_EMPTY, data->buffers_size);
	sems->driv_cli->mutex = semaphore_create(STR_SEM_DRIV_CLI_MUTEX, 1);

	sems->results_mutex = semaphore_create(STR_SEM_RESULTS_MUTEX, 1);
}

void wakeup_processes(struct main_data *data, struct semaphores *sems)
{
	for (int i = 0; i < data->max_ops; i++)
	{
		produce_end(sems->main_rest);
		produce_end(sems->rest_driv);
		produce_end(sems->driv_cli);
		semaphore_mutex_unlock(sems->results_mutex);
	}
}

void destroy_semaphores(struct semaphores *sems)
{
	semaphore_destroy(STR_SEM_MAIN_REST_FULL, sems->main_rest->full);
	semaphore_destroy(STR_SEM_MAIN_REST_EMPTY, sems->main_rest->empty);
	semaphore_destroy(STR_SEM_MAIN_REST_MUTEX, sems->main_rest->mutex);

	semaphore_destroy(STR_SEM_REST_DRIV_FULL, sems->rest_driv->full);
	semaphore_destroy(STR_SEM_REST_DRIV_EMPTY, sems->rest_driv->empty);
	semaphore_destroy(STR_SEM_REST_DRIV_MUTEX, sems->rest_driv->mutex);

	semaphore_destroy(STR_SEM_DRIV_CLI_FULL, sems->driv_cli->full);
	semaphore_destroy(STR_SEM_DRIV_CLI_EMPTY, sems->driv_cli->empty);
	semaphore_destroy(STR_SEM_DRIV_CLI_MUTEX, sems->driv_cli->mutex);

	semaphore_destroy(STR_SEM_RESULTS_MUTEX, sems->results_mutex);
}