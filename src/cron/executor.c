#include "executor.h"

CronError execute_task(Task task)
{
    CronError rc = CRON_ERR_EXECUTE_TASK;

    pid_t pid = fork();
    switch (pid) {
        case -1:
            return CRON_ERR_FORK_FAILED;
        case 0:
            execvp(task.Argv[0], task.Argv);
            perror("execvp");
            _exit(1);
        default:
            break;
    }

    int status;
    if (waitpid(pid, &status, 0) == -1) {
        return CRON_ERR_EXECUTE_TASK;
    }

    if (WIFEXITED(status)) {
        int code = WEXITSTATUS(status);
        if (code == 0) {
            rc = CRON_OK;
        }
    }

    return rc;
}
