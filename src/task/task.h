#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    DEFAULT,
    LIST,
    RANGE
} ScheduleType;


typedef struct Schedule Schedule;
struct Schedule {
    ScheduleType Type;

    int8_t Default;

    uint8_t* List;
    size_t NumListItems;

    uint8_t RangeStart;
    uint8_t RangeEnd;
};

typedef struct Task Task;
struct Task {
    Schedule Minute;
    Schedule Hour;
    Schedule Day;
    Schedule Month;
    Schedule Weekday;

    char** Argv;
    char* raw;
};

static void destroy_task(Task *task) {
    if (!task) return;

    free(task->Argv);
    free(task->raw);

    task->Argv = NULL;
    task->raw = NULL;
}

static void destroy_tasks(Task *tasks, size_t num) {
    if (!tasks) return;

    for (size_t i = 0; i < num; i++) {
        destroy_task(&tasks[i]);
    }

    free(tasks);
}

#endif //TASK_H
