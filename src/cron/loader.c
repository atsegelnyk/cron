#include "loader.h"

#include <stdlib.h>
#include <string.h>

CronError read_crontab_file(const char *path, char ***lines_out, size_t *n_lines_out);

CronError load_crontab(const char *path, Task **tasks_out, size_t *out_count)
{
    char **lines = NULL;
    size_t lines_num = 0;
    CronError read_status = read_crontab_file(path, &lines, &lines_num);
    if (read_status != CRON_OK) return read_status;

    size_t tasks_count = 0;
    Task *tasks = malloc(sizeof(Task) * lines_num);

    for (size_t i = 0; i < lines_num; i++) {
        tasks[tasks_count].raw = lines[i];

        ParserError parse_status = parse_line_into_task(lines[i], &tasks[tasks_count]);
        if (parse_status == PARSER_EMPTY_LINE) continue;
        if (parse_status != PARSER_OK) {
            printf("parse failed with code %d\n", parse_status);
            free(tasks);
            free(lines);
            return CRON_ERR_READ_CRONTAB;
        }

        tasks_count++;
    }

    *tasks_out = tasks;
    *out_count = tasks_count;

    return CRON_OK;
}

CronError read_crontab_file(const char *path, char ***lines_out, size_t *n_lines_out)
{
    CronError rc = CRON_ERR_READ_CRONTAB;

    FILE *f = fopen(path, "r");
    if (!f) {
        perror(path);
        goto cleanup;
    }

    size_t lines_count = 0;
    size_t lines_capacity = 16;
    char **lines = malloc(lines_capacity * sizeof(char *));
    if (!lines) {
        goto cleanup;
    }

    for (;;) {
        char *line = NULL;
        size_t cap = 0;
        if (getline(&line, &cap, f) == -1) break;

        line[strcspn(line, "\n")] = '\0';

        if (lines_capacity == lines_count) {
            lines_capacity *= 2;
            char **new_lines = realloc(lines, sizeof(char*) * lines_capacity);
            if (!new_lines) {
                free(lines);
                goto cleanup;
            }

            lines = new_lines;
        }

        lines[lines_count] = line;
        lines_count++;
    }

    rc = CRON_OK;
    *lines_out = lines;
    *n_lines_out = lines_count;

    cleanup:
        fclose(f);
        return rc;
}
