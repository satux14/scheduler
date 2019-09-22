/*
 * Sathish Kumar R
 *
 * Demonstration of a scheduler
 * schedule_create_thread - Create a thread and return
 * schedule_start_thread - Add the thread to queue but we cannot start until
 * user give control to us, so user will have to call schedule_start() to give
 * us control.
 * schedule_start - Take control and start calling each threads function handler.
 * Function handler can create or destroy threads
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schedule.h"

sq_t SQ;

int g_gen_tid;

static int
generate_tid()
{
    // For now just increment the global value
    return ++g_gen_tid;
}

static void
schedule_del_queue(sq_thread_t *t)
{
    sq_thread_t *temp, *prev;
    temp = SQ.sq[t->priority];

    /* chnage head itself */
    if (temp && temp->id == t->id) {
        printf("TID head itself freed now\n");
        SQ.sq[t->priority] = temp->next;
        return;
    }

    while (temp && temp->id != t->id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("TID not found\n");
        return;
    }

    if (temp) {
        prev->next = temp->next;
        printf("TID found and updated\n");
    }
    return;

}

static int
schedule_add_queue(sq_thread_t *t)
{
    sq_thread_t *temp, *prev;
    prev = temp = SQ.sq[t->priority];

    if (!temp) {
        SQ.sq[t->priority] = t;
        printf("First tid %d added to queue\n", t->id);
        return 0;
    }

    while (temp) {
        prev = temp;
        temp = temp->next;
    }
    prev->next = t;
    printf("Added tid %d to queue\n", t->id);
    return 0;
}

sq_thread_t *
schedule_create_thread(int prio)
{
    sq_thread_t *sq;

    sq = malloc(sizeof(sq_thread_t));
    if (sq == NULL) {
        return NULL;
    }
    memset(sq, 0, sizeof(sq_thread_t));

    sq->id = generate_tid();
    sq->priority = prio;

    printf("%s: sq: %p, sq.id: %d, sq.prio: %d\n", __FUNCTION__, sq, sq->id, sq->priority);
    return sq;
}

int
schedule_start_thread(sq_thread_t *t, void (*fn)(), void *context)
{
    printf("Start thread for tid: %d\n", t->id);
    t->ctx = context;
    t->fn = fn;
    schedule_add_queue(t);
}

void
schedule_destroy_thread(sq_thread_t *t)
{
    schedule_del_queue(t);
    free(t);
    return;
}

void
schedule_display()
{
    int prio;
    sq_thread_t *temp;

    for (prio = 0; prio < (MAX_PRIORITY - 1); prio++) {
        temp = SQ.sq[prio];
        while (temp) {
            printf("No: %d, TID: %d\n", prio+1, temp->id);
            temp = temp->next;
        }
    }
}

void
schedule_start()
{
    int prio;
    sq_thread_t *temp, *next;

    while (1) {

        for (prio = 0; prio < (MAX_PRIORITY - 1); prio++) {
            temp = SQ.sq[prio];
            while (temp) {
                next = temp->next;
                temp->fn(temp->ctx);
                //sleep(2);
                temp = next;
            }
        }
    }
}

int
schedule_init()
{
    return 0;
}
