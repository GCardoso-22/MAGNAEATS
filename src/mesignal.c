#include "mesignal.h"
#include "main.h"
#include "synchronization.h"
#include <signal.h>

struct main_data *data;
struct communication_buffers *buffers;
struct semaphores *sems;

void signal_alarm()
{
    signal(SIGALRM, signal_alarm);
    alarm(data->alarm_time);
}
void ctrlC()
{
    signal(SIGINT, ctrlC);
    stop_execution(data, buffers, sems);
}