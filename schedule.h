#ifndef __SCHEDULE_H__
#include <unistd.h>
#define MAX_PRIORITY  5

typedef struct sq_thread {
    struct sq_thread *next;
    int id;
    int priority;
    int quantom;
    void (*fn)();
    void *ctx;
} sq_thread_t;

typedef struct sched_queue {
    sq_thread_t *sq[MAX_PRIORITY];
    sq_thread_t *cur_thread;
} sq_t;

extern sq_thread_t * schedule_create_thread(int prio);
extern void schedule_destroy_thread(sq_thread_t *t);
extern int schedule_start_thread(sq_thread_t *t, void (*fn)(), void *context);
extern void schedule_start();
extern int schedule_init();
extern void schedule_display();

#endif /* __SCHEDULE_H__ */
