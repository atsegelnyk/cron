#include "loader.h"

#include <stdlib.h>
#include <string.h>

CronError load_crontab(const char *path, Task **tasks_out, size_t *out_count)
{
    CronError rc;

    Task stack_tasks[16];
    Task *tasks = stack_tasks;

    size_t tasks_cap = 16;
    size_t tasks_count = 0;

    FILE *f = fopen(path, "r");
    if (!f) {
        perror(path);
        rc = CRON_ERR_READ_CRONTAB;
        goto cleanup;
    }

    char *line = NULL;
    size_t cap = 0;

    while (getline(&line, &cap, f) != -1) {
        Task *task = task_init();
        ParserError parse_status = parse_line_into_task(line, task);
        if (parse_status != PARSER_OK) {
            printf("parse failed with code %d\n", parse_status);
            rc = CRON_ERR_PARSE_CONFIG;
            goto cleanup;
        }

        if (tasks_cap >= tasks_count) {
            tasks_cap *= 2;
            Task *new_tasks;

            if (tasks == stack_tasks) {
                new_tasks = malloc(cap * sizeof(Task));
                if (!new_tasks) {
                    rc = CRON_ERR_LOAD_CONFIG;
                    free(new_tasks);
                    free(line);
                    goto cleanup;
                }

                memcpy(new_tasks, stack_tasks, tasks_count * sizeof(Task));
            } else {
                new_tasks = realloc(tasks, cap * sizeof(Task));
                if (!new_tasks) {
                    rc = CRON_ERR_LOAD_CONFIG;
                    free(new_tasks);
                    free(line);
                    goto cleanup;
                }
            }

            tasks = new_tasks;
        }

        tasks[tasks_count++] = *task;
    }

    rc = CRON_OK;
    *tasks_out = tasks;
    *out_count = tasks_count;

    cleanup:
        fclose(f);
        return rc;
}
