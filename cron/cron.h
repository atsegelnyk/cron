#ifndef DEFAULT_CRONTAB_PATH
#define DEFAULT_CRONTAB_PATH "crontab"

#endif // DEFAULT_CRONTAB_PATH

#ifndef CRON_H
#define CRON_H

#include "task/task.h"
#include <stdint.h>

typedef enum {
    CRON_OK,
    CRON_FAILED,
    CRON_ERR_READ_CRONTAB,
    CRON_ERR_LOAD_CONFIG,
    CRON_ERR_PARSE_CONFIG,
    CRON_ERR_EXECUTE_TASK,
} CronError;

typedef struct Cron Cron;

struct Cron {
    char *CrontabPath;

    Task *Tasks;
    size_t NumTasks;
};


Cron *cron_init(void);

void cron_destroy(Cron *cron);

CronError cron_run(Cron *cron);

#endif //CRON_H
