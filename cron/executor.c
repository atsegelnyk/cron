#include "executor.h"

CronError execute_task(Task task)
{
    CronError rc = CRON_ERR_EXECUTE_TASK;

    pid_t pid = fork();

    if (pid == 0) {
        execvp(task.Argv[0], task.Argv);

        perror("execvp");
        _exit(1);
    }

    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
        int code = WEXITSTATUS(status);
        if (code == 0) rc = CRON_OK;
    }

    return rc;
}
