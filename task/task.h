#ifndef TASK_H
#define TASK_H

#include <stdint.h>

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
    uint8_t NumListItems;

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

    int8_t Argc;
    char** Argv;
};

Task* task_init(void);
void task_destroy(Task* task);

#endif //TASK_H
