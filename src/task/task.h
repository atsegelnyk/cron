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

void destroy_tasks(Task *tasks, size_t num);

#endif //TASK_H
