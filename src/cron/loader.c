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
        line[strcspn(line, "\n")] = '\0';

        char* line_cpy = strdup(line);
        tasks[tasks_count]._rawLine = line_cpy;

        ParserError parse_status = parse_line_into_task(line_cpy, &tasks[tasks_count]);
        if (parse_status != PARSER_OK) {
            printf("parse failed with code %d\n", parse_status);
            rc = CRON_ERR_PARSE_CONFIG;
            goto cleanup;
        }

        if (tasks_count >= tasks_cap) {
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
                new_tasks = realloc(tasks, tasks_cap * sizeof(Task));
                if (!new_tasks) {
                    rc = CRON_ERR_LOAD_CONFIG;
                    free(new_tasks);
                    free(line);
                    goto cleanup;
                }
            }

            tasks = new_tasks;
        }

        tasks_count++;
    }

    rc = CRON_OK;
    *tasks_out = tasks;
    *out_count = tasks_count;

    cleanup:
        fclose(f);
        return rc;
}
