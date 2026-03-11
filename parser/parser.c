#include "parser.h"
#include "task/task.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


int8_t split_tokens(char *line, char *delim, char ***out_tokens, uint8_t *out_count);
int8_t parse_tokens_into_task(char **tokens, uint8_t count, Task *task);
int8_t parse_schedule(char *token, Schedule *schedule);

int8_t parse_list(char *token, uint8_t **list_items, uint8_t *items_count);
int8_t parse_range(char *token, uint8_t *start_range, uint8_t *end_range);


int8_t convert_value(char *token);

int8_t parse_line_into_task(char *line, Task *task) 
{
    char **tokens = NULL;
    uint8_t count = 0;

    int8_t rc = -1;

    if (split_tokens(line, " ", &tokens, &count) != 0) {
        goto cleanup;
    }

    if (count < 6) {
        goto cleanup;        
    }

    if (parse_tokens_into_task(tokens, count, task) != 0) {
        goto cleanup;
    }

    rc = 0;
    goto cleanup;

    cleanup:
        free(tokens);
        return rc;
}


int8_t parse_tokens_into_task(char **tokens, uint8_t count, Task *task)
{
    Schedule minuteSchedule;
    if (parse_schedule(tokens[0], &minuteSchedule) != 0) {
        return -1;
    }

    Schedule hourSchedule;
    if (parse_schedule(tokens[1], &hourSchedule) !=  0) {
        return -1;
    }

    Schedule daySchedule;
    if (parse_schedule(tokens[2], &daySchedule) !=  0) {
        return -1;
    }

    Schedule monthSchedule;
    if (parse_schedule(tokens[3], &monthSchedule) !=  0) {
        return -1;
    }

    Schedule weekdaySchedule;
    if (parse_schedule(tokens[4], &weekdaySchedule) !=  0) {
        return -1;
    }

    task->Minute = minuteSchedule;
    task->Hour = hourSchedule;
    task->Day = daySchedule;
    task->Month = monthSchedule;
    task->Weekday = weekdaySchedule;

    uint8_t commandTokenCount = count-5;
    char** commandTokens = malloc(sizeof(char*)*commandTokenCount);
    memcpy(commandTokens, &tokens[5], sizeof(char*)*commandTokenCount);

    task->Argc = (int8_t)commandTokenCount;
    task->Argv = commandTokens;

    return 0;
}

int8_t parse_schedule(char *token, Schedule *schedule)
{
    if (strchr(token, ',')) {
        uint8_t *list_items;
        uint8_t items_count;

        if (parse_list(token, &list_items, &items_count) != 0) {
            return -1;
        }

        schedule->Type = LIST;
        schedule->List = list_items;
        schedule->NumListItems = items_count;
        
        return 0;
    }

    if (strchr(token, '-')) {
        uint8_t range_start;
        uint8_t range_end;

        if (parse_range(token, &range_start, &range_end) != 0) {
            return -1;
        }

        schedule->Type = RANGE;
        schedule->RangeStart = range_start;
        schedule->RangeEnd = range_end;
        
        return 0;
    }

    int8_t default_value = convert_value(token);
    if (default_value == -2) {
        return -1;
    }

    schedule->Type = DEFAULT;
    schedule->Default = default_value;

    return 0;
}

int8_t parse_list(char *token, uint8_t **list_items, uint8_t *items_count)
{
    char **list_tokens = NULL;
    uint8_t count = 0;
    int8_t rc = -1;

    if (split_tokens(token, ",", &list_tokens, &count) != 0) {
        goto cleanup;
    }

    *items_count = count;

    if (count == 0) {
        goto cleanup;
    }

    *list_items = malloc(sizeof(**list_items) * count);
    if (*list_items == NULL) {
        goto cleanup;
    }

    for (uint8_t i = 0; i < count; i++) {
        int8_t value = convert_value(list_tokens[i]);
        if (value == -2) {
            free(*list_items);
            *list_items = NULL;
            *items_count = 0;
            goto cleanup;
        }

        (*list_items)[i] = (uint8_t)value;
    }     

    rc = 0;

cleanup:
    free(list_tokens);
    return rc;
}

int8_t parse_range(char *token, uint8_t *start_range, uint8_t *end_range)
{
    char **list_tokens = NULL;
    uint8_t count = 0;
    int8_t rc = -1;

    if (split_tokens(token, "-", &list_tokens, &count) != 0) {
        goto cleanup;
    }

    if (count != 2) {
        goto cleanup;
    }

    int8_t start_value = convert_value(list_tokens[0]);
    if (start_value < 0) {
        goto cleanup;
    }

    int8_t end_value = convert_value(list_tokens[1]);
    if (end_value < 0) {
        goto cleanup;
    }

    *start_range = (uint8_t)start_value;
    *end_range = (uint8_t)end_value;

    rc = 0;

    cleanup:
        free(list_tokens);
        return rc;
}

int8_t convert_value(char *token)
{
    if (strcmp(token, "*") == 0) {
        return -1;
    }

    char *end;
    errno = 0;

    int64_t v = strtol(token, &end, 10);

    if (errno || *end != '\0') {
        return -2;
    }

    return (int8_t)v;
}

int8_t split_tokens(char *line, char *delim, char ***out_tokens, uint8_t *out_count)
{
    char *stack_tokens[8];
    char **tokens = stack_tokens;

    uint8_t cap = 8;
    uint8_t count = 0;

    char *tok = strtok(line, delim);

    while (tok) {

        if (count == cap) {
            cap *= 2;

            char **new_tokens;

            if (tokens == stack_tokens) {
                new_tokens = malloc(cap * sizeof(char *));
                if (!new_tokens) {
                    return -1;
                }

                memcpy(new_tokens, stack_tokens, count * sizeof(char *));
            } else {
                new_tokens = realloc(tokens, cap * sizeof(char *));
                if (!new_tokens) {
                    return -1;
                }
            }

            tokens = new_tokens;
        }

        tokens[count++] = tok;
        tok = strtok(NULL, delim);
    }

    *out_count = count;

    if (tokens == stack_tokens) {
        *out_tokens = malloc(count * sizeof(char *));
        memcpy(*out_tokens, stack_tokens, count * sizeof(char *));
    } else {
        *out_tokens = tokens;
    }

    return 0;
}