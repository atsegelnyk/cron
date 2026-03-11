#include "cron.h"
#include <stdint.h>
#include <stdlib.h>

Cron *cron_init(uint8_t capacity)
{
    Task *tasks = malloc(sizeof(Task) * capacity);


    Cron *cron = malloc(sizeof(Cron));
    cron->NumTasks = 0;
    cron->TasksCapacity = capacity;
    cron->Tasks = tasks;

    return cron;
}

void cron_destroy(Cron *cron)
{
    if (cron->Tasks) {
        free(cron->Tasks);
    }

    free(cron);
}

CronError cron_run(Cron *cron)
{
    return CRON_OK;
}
