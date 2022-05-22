/* SO-043
Diogo Chambel - 53319
Gonçalo Cardoso - 54415
Pedro Correia - 54570
*/
#ifndef CONFIGURATION_H_GUARD
#define CONFIGURATION_H_GUARD

#include "main.h"

#define MAXLINHA 1024

/* Função que lê os argumentos da aplicação, nomeadamente o número
 * máximo de operações, o tamanho dos buffers de memória partilhada
 * usados para comunicação, e o número de clientes, de motoristas e de
 * restaurantes. Guarda esta informação nos campos apropriados da
 * estrutura main_data.
 */
void config(char *nomeficheiro, struct main_data *data);

#endif