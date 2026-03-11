#ifndef CRON_EXECUTOR_H
#define CRON_EXECUTOR_H

#include <stdlib.h>
#include <unistd.h>

#include "cron.h"

CronError execute_task(Task task);

#endif //CRON_EXECUTOR_H
