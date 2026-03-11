#ifndef CRON_H
#define CRON_H

#include "task/task.h"
#include <stdint.h>

typedef enum {
    CRON_OK = 0,
    CRON_FAILED,
    CRON_ERR_LOAD_CONFIG,
    CRON_ERR_PARSE_CONFIG,
} CronError;

typedef struct Cron Cron;
struct Cron {
    Task *Tasks;
    uint8_t NumTasks;
    uint8_t TasksCapacity;
};


Cron* cron_init(uint8_t capacity);
void cron_destroy(Cron* cron);
CronError cron_run(Cron* cron);

#endif
