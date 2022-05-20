/* SO-043
Diogo Chambel - 53319
Gonçalo Cardoso - 54415
Pedro Correia - 54570
 */
#include "metime.h"
#include <time.h>

void set_start_time(struct operation *op)
{
	struct timespec start;
	if (clock_gettime(CLOCK_REALTIME, &start) == -1)
	{
		perror("clock gettime");
		exit(EXIT_FAILURE);
	}
	op->start_time = start;
}

void set_rest_time(struct operation *op)
{
	struct timespec rest;
	if (clock_gettime(CLOCK_REALTIME, &rest) == -1)
	{
		perror("clock gettime");
		exit(EXIT_FAILURE);
	}
	op->rest_time = rest;
}

void set_driver_time(struct operation *op)
{
	struct timespec driver;
	if (clock_gettime(CLOCK_REALTIME, &driver) == -1)
	{
		perror("clock gettime");
		exit(EXIT_FAILURE);
	}
	op->driver_time = driver;
}

void set_client_end_time(struct operation *op)
{
	struct timespec end;
	if (clock_gettime(CLOCK_REALTIME, &end) == -1)
	{
		perror("clock gettime");
		exit(EXIT_FAILURE);
	}
	op->client_end_time = end;
}
