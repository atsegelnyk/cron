//
// Created by art on 13.03.2026.
//

#include "task.h"

void destroy_task(Task *task)
{
    if (!task) return;

    free(task->Argv);
    free(task->raw);

    task->Argv = NULL;
    task->raw = NULL;
}

void destroy_tasks(Task *tasks, size_t num)
{
    if (!tasks) return;

    for (size_t i = 0; i < num; i++) {
        destroy_task(&tasks[i]);
    }

    free(tasks);
}