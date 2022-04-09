/* SO-043
Diogo Chambel - 53319
Gonçalo Cardoso - 54415
Pedro Correia - 54570
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/memory.h"

void *create_shared_memory(char *name, int size)
{
	int sm, ret;
	int *ptr;

	int uid;

	char processName[strlen(name) + 15];
	uid = getuid();
	sprintf(processName, "/%s%d", name, uid);

	sm = shm_open(processName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

	if (sm == -1)
	{
		perror(name);
		exit(1);
	}
	ret = ftruncate(sm, size);
	if (ret == -1)
	{
		perror(name);
		exit(2);
	}

	ptr = (int *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, sm, 0);
	if (ptr == MAP_FAILED)
	{
		perror(("/%s-mmap", name));
		exit(3);
	}
	return ptr;
}

void *create_dynamic_memory(int size)
{
	void *dm = malloc(size);
	if (dm == NULL)
	{
		perror("dm");
		exit(1);
	}

	dm = memset(dm, 0, size);
}

void destroy_shared_memory(char *name, void *ptr, int size)
{
	int ret;

	int uid;
	char processName[strlen(name) + 15];
	uid = getuid();
	sprintf(processName, "/%s%d", name, uid);

	ret = munmap(ptr, size);
	if (ret == -1)
	{
		perror(processName);
		exit(7);
	}
	ret = shm_unlink(processName);
	if (ret == -1)
	{
		perror(processName);
		exit(8);
	}
}

void destroy_dynamic_memory(void *ptr)
{
	free(ptr);
}

void write_main_rest_buffer(struct rnd_access_buffer *buffer, int buffer_size, struct operation *op)
{
	int n;
	for (n = 0; n < buffer_size; n++)
	{
		if (buffer->ptrs[n] == 0)
		{
			buffer->buffer[n] = *op;
			buffer->ptrs[n] = 1;
			break;
		}
	}
}

void write_rest_driver_buffer(struct circular_buffer *buffer, int buffer_size, struct operation *op)
{
	buffer->ptrs->in = 0;
	buffer->ptrs->out = 0;

	while (((buffer->ptrs->in + 1) % buffer_size) == buffer->ptrs->out)
	{
		buffer->buffer[buffer->ptrs->in] = *op;
		buffer->ptrs->in = (buffer->ptrs->in + 1) % buffer_size;
	}
}
void write_driver_client_buffer(struct rnd_access_buffer *buffer, int buffer_size, struct operation *op)
{
	int n;
	for (n = 0; n < buffer_size; n++)
	{
		if (buffer->ptrs[n] == 0)
		{
			buffer->buffer[n] = *op;
			buffer->ptrs[n] = 1;
			break;
		}
	}
}

void read_main_rest_buffer(struct rnd_access_buffer *buffer, int rest_id, int buffer_size, struct operation *op)
{
	int n;
	for (n = 0; n < buffer_size; n++)
	{
		if (buffer->ptrs[n] == 1)
		{
			*op = buffer->buffer[n];
			buffer->ptrs[n] = 0;
			break;
		}
	}
	if (n == buffer_size - 1)
	{
		op->id = -1;
	}
}
void read_rest_driver_buffer(struct circular_buffer *buffer, int buffer_size, struct operation *op)
{
	while (buffer->ptrs->in == buffer->ptrs->out)
	{
		*op = buffer->buffer[buffer->ptrs->out];
		buffer->ptrs->out = (buffer->ptrs->out + 1) % buffer_size;
	}
	if (buffer->ptrs->in == buffer_size - 1)
	{
		op->id = -1;
	}
}

void read_driver_client_buffer(struct rnd_access_buffer *buffer, int client_id, int buffer_size, struct operation *op)
{
	int n;
	for (n = 0; n < buffer_size; n++)
	{
		if (buffer->ptrs[n] == 1)
		{
			*op = buffer->buffer[n];
			buffer->ptrs[n] = 0;
			break;
		}
	}
	if (n == buffer_size - 1)
	{
		op->id = -1;
	}
}