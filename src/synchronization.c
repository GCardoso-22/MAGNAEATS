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
#include "synchronization.h"

sem_t *semaphore_create(char *name, int value)
{
    sem_t *sem;

    char semaphoreName[strlen(name) + 15];
    int uid = getuid();
    sprintf(semaphoreName, "/%s%d", name, uid);

    sem = sem_open(semaphoreName, O_CREAT, 0xFFFFFFFF, value);
    if (sem == SEM_FAILED)
    {
        perror("full");
        exit(6);
    }
    return sem;
}

void semaphore_destroy(char *name, sem_t *semaphore)
{
    char semaphoreName[strlen(name) + 15];
    int uid = getuid();
    sprintf(semaphoreName, "/%s%d", name, uid);

    if (sem_close(semaphore) == -1)
    {
        perror("sem");
    }

    if (sem_unlink(semaphoreName) == -1)
    {
        perror("sem");
    }
}

void produce_begin(struct prodcons *pc)
{
    sem_wait(pc->empty);
    sem_wait(pc->mutex);
}

void produce_end(struct prodcons *pc)
{
    sem_post(pc->full);
    sem_post(pc->mutex);
}

void consume_begin(struct prodcons *pc)
{
    sem_wait(pc->full);
    sem_wait(pc->mutex);
}

void consume_end(struct prodcons *pc)
{
    sem_post(pc->empty);
    sem_post(pc->mutex);
}

void semaphore_mutex_lock(sem_t *sem)
{
    
    sem_wait(sem);
}

void semaphore_mutex_unlock(sem_t *sem)
{
    sem_post(sem);
}
