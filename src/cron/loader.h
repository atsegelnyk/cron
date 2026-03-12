#ifndef CRON_LOADER_H
#define CRON_LOADER_H

#include <stdio.h>

#include "task.h"
#include "parser.h"
#include "cron.h"

CronError load_crontab(const char *path, Task **tasks, size_t *task_count);

#endif //CRON_LOADER_H
