#include "cron.h"

#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>


#include "loader.h"
#include "executor.h"


CronError cron_iter(Cron *cron);
CronError cron_set_tasks(Cron *cron);

CronError process_task(Task task, time_t now);

bool task_matches_time(Task task, time_t now);
bool schedule_matches_time_cond(Schedule schedule, int t);

bool default_schedule_matches_time_cond(Schedule schedule, int t);
bool list_schedule_matches_time_cond(Schedule schedule, int t);
bool range_schedule_matches_time_cond(Schedule schedule, int t);


Cron *cron_init(void)
{
    Cron *cron = malloc(sizeof(Cron));
    cron->NumTasks = 0;
    cron->Tasks = NULL;
    cron->CrontabPath = DEFAULT_CRONTAB_PATH;

    return cron;
}

void cron_destroy(Cron *cron)
{
    if (cron->Tasks) {
        free(cron->Tasks);
    }

    free(cron);
}

CronError cron_run(Cron *cron)
{
    CronError rc = cron_iter(cron);
    if (rc != CRON_OK)
        return rc;

    for (;;) {
        sleep(60);
        rc = cron_iter(cron);
        if (rc != CRON_OK)
            return rc;
    }
}

CronError cron_iter(Cron *cron)
{
    CronError rc = cron_set_tasks(cron);
    if (rc != CRON_OK) {
        printf("cron main: crontab load failed with code: %d\n", rc);
        return rc;
    }

    if (cron->NumTasks == 0) {
        printf("cron main: no cron tasks found\n");
        return CRON_OK;
    }

    printf("cron main: execute %lu tasks\n", cron->NumTasks);
    const time_t now = time(NULL);

    for (size_t i = 0; i < cron->NumTasks; i++) {
        CronError task_rc = process_task(cron->Tasks[i], now);
        if (task_rc != CRON_OK) {
            printf("execute task %lu failed with code: %d\n", i, task_rc);
            continue;
        }

        printf("execute task %lu: success\n", i);
    }

    return rc;
}

CronError cron_set_tasks(Cron *cron)
{
    Task* new_tasks;
    size_t num_tasks;
    CronError rc = load_crontab(cron->CrontabPath, &new_tasks, &num_tasks);
    if (rc != CRON_OK) {
        return rc;
    }

    destroy_tasks(cron->Tasks, cron->NumTasks);

    cron->NumTasks = num_tasks;
    cron->Tasks = new_tasks;

    return CRON_OK;
}

CronError process_task(const Task task, const time_t now)
{
    if (!task_matches_time(task, now)) {
        printf("task sched does not match current time, skipping\n");
        return CRON_ERR_EXECUTION_SKIPPED;
    }

    return execute_task(task);
}


bool task_matches_time(const Task task, const time_t now)
{
    const struct tm *t = localtime(&now);

    bool matches_time;
    matches_time = schedule_matches_time_cond(task.Minute, t->tm_min);
    if (!matches_time) {
        return false;
    }

    matches_time = schedule_matches_time_cond(task.Hour, t->tm_hour);
    if (!matches_time) {
        return false;
    }

    matches_time = schedule_matches_time_cond(task.Day, t->tm_mday);
    if (!matches_time) {
        return false;
    }

    matches_time = schedule_matches_time_cond(task.Month, t->tm_mon + 1);
    if (!matches_time) {
        return false;
    }

    matches_time = schedule_matches_time_cond(task.Weekday, t->tm_wday);
    if (!matches_time) {
        return false;
    }

    return matches_time;
}

bool schedule_matches_time_cond(const Schedule schedule, const int t)
{
    switch (schedule.Type) {
        case DEFAULT:
            return default_schedule_matches_time_cond(schedule, t);
        case LIST:
            return list_schedule_matches_time_cond(schedule, t);
        case RANGE:
            return range_schedule_matches_time_cond(schedule, t);
    }

    return false;
}

bool default_schedule_matches_time_cond(const Schedule schedule, const int t)
{
    if (schedule.Default == -1) {
        return true;
    }

    if (schedule.Default == t) {
        return true;
    }

    return false;
}

bool list_schedule_matches_time_cond(const Schedule schedule, const int t)
{
    bool r = false;
    for (size_t i = 0; i < schedule.NumListItems; i++) {
        if (schedule.List[i] == t) {
            r = true;
            break;
        }
    }

    return r;
}

bool range_schedule_matches_time_cond(const Schedule schedule, const int t)
{
    return t >= schedule.RangeStart && t <= schedule.RangeEnd;
}
