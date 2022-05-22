/* SO-043
Diogo Chambel - 53319
Gonçalo Cardoso - 54415
Pedro Correia - 54570
*/
#ifndef METIME_H_GUARD
#define METIME_H_GUARD

#include "main.h"
#include "memory.h"

void set_start_time(struct operation *op);

void set_rest_time(struct operation *op);

void set_driver_time(struct operation *op);

void set_client_end_time(struct operation *op);

#endif