/*
 * Sathish Kumar R
 *
 * Main program to test the scheduler
 * Check schedule.c for more details
 */

#include <stdio.h>
#include <stdlib.h>
#include "schedule.h"

void t1_func(void *ctx)
{
    sq_thread_t *t = ctx;
    printf("tid %d is called with priority: %d\n", t->id, t->priority);
    return;
}

void t_func(void *ctx)
{
    sq_thread_t *t = ctx;
    static num_thread = 0;
    static start_throttle = 0;

    printf("tid %d is called with priority: %d\n", t->id, t->priority);

    if (num_thread > 10 || start_throttle) {
        printf("Number of thread exceeded, start destroy\n");
        schedule_destroy_thread(t);
        num_thread--;
        if (num_thread <= 3)
            start_throttle = 0;
        else 
            start_throttle = 1;

    } else {
        printf("Creating new therad\n");
        t = schedule_create_thread(rand()%(MAX_PRIORITY - 1));
        schedule_start_thread(t, t_func, t);
        num_thread++;
    }

    return;
}

int main()
{
    schedule_init();

    sq_thread_t *t1 = schedule_create_thread(0);
    schedule_start_thread(t1, t_func, t1);

    sq_thread_t *t2 = schedule_create_thread(0);
    schedule_start_thread(t2, t_func, t2);

    sq_thread_t *t3 = schedule_create_thread(0);
    schedule_start_thread(t3, t_func, t3);

    schedule_start();
    schedule_display();

    return 0;
}
