#include <stdio.h>
#include <stdlib.h>

#include "parser/parser.h"
#include "task/task.h"

void print_schedule(Schedule s)
{
    printf("type: %d ", s.Type);

    switch (s.Type) {
    case DEFAULT:
        printf("value: %d\n", s.Default);
        return;
    case LIST:
        printf("value: ");
        for (int i = 0; i < s.NumListItems; i++) {
            printf("%d, ", s.List[i]);
        }
        printf("\n");
        return;
    case RANGE:
        printf("value: %d-%d\n", s.RangeStart, s.RangeEnd);
        return;
    }
}

int main(void)
{
    char line[] = "0-10 0,2,4,6,8 * * * /bin/sh -c yes";
    
    Task *task = task_init();
    int8_t status = parse_line_into_task(line, task);

    if (status != 0) {
        printf("non 0 return code: %d\n", status);
        exit(1);
    }


    printf("minute:\n");
    print_schedule(task->Minute);

    printf("hour:\n");
    print_schedule(task->Hour);

    printf("day:\n");
    print_schedule(task->Day);

    printf("month:\n");
    print_schedule(task->Month);

    printf("weekday:\n");
    print_schedule(task->Weekday);

    printf("command: ");
    for (int i = 0; i < task->Argc; i++) {
        printf("\"%s\", ", task->Argv[i]);
    }
    printf("\n");
}
