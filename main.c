#include <stdlib.h>

#include "cron.h"

int main(void)
{
    Cron *cron = cron_init();
    if (cron == NULL) {
        exit(1);
    }

    CronError status = cron_run(cron);
    if (status != CRON_OK) {
        printf("cron_run exited with code %d\n", status);
        cron_destroy(cron);
        exit(1);
    }

    printf("cron_run exited with code %d\n", status);
    cron_destroy(cron);
}
