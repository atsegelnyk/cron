#include "task.h"
#include <stdlib.h>

Task* task_init(void)
{   
    Task* task = malloc(sizeof(Task));
    if (!task) {
        return NULL;
    }
    
    return task;
}

void task_destroy(Task* task)
{
    free(task);
}
