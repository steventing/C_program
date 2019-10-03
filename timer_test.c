/* Author: Steven Ting  https://github.com/steventing/C_program */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

struct timerhandler_data *t_data;
static void timerhandler(union sigval s);
struct timerhandler_data {
    uint8_t periodic;     /* 0: SetTimeout / 1: SetInterval in JS*/
    timer_t timer_id;
    uint32_t timeout_ms;
};

void start_timer()
{
    t_data = (struct timerhandler_data *)calloc(1, sizeof(struct timerhandler_data));
    struct sigevent se;
    struct itimerspec its;
    timer_t timer_id;

    memset(&se, 0, sizeof(se));
    se.sigev_notify          = SIGEV_THREAD;
    se.sigev_notify_function = timerhandler;
    se.sigev_value.sival_ptr = (void *)t_data;

    if (-1 == timer_create(CLOCK_REALTIME, &se, &timer_id)) {
        timer_delete(timer_id);
        free(t_data);
    }

    printf("timer_create timer_id(%p)\n", timer_id);

    t_data->timer_id = timer_id;
    t_data->periodic = 1;
    t_data->timeout_ms = 3000;

    // Finally, arm/start the timer
    //
    its.it_value.tv_sec     = t_data->timeout_ms / 1000;
    its.it_value.tv_nsec    = 0;
    its.it_interval.tv_sec  = t_data->timeout_ms / 1000;
    its.it_interval.tv_nsec = 0;

    if (0 != timer_settime(timer_id, 0, &its, NULL))
    {
        free(t_data);
        timer_delete(timer_id);
        return;
    }

    printf("set_timer()\n");
    return;
}

static void timerhandler(union sigval s)
{
    struct timerhandler_data *aux;
    aux = (struct timerhandler_data *)s.sival_ptr;

    printf("timerhandler got value timer_id(%p) timeout_ms(%u)\n",
           aux->timer_id,
           aux->timeout_ms
           );

    if (1 != aux->periodic) {
        timer_delete(aux->timer_id);
        printf("timer deleted\n");
        free(aux);
    }

    return;
}

int main()
{
    printf("Start time\n");
    start_timer();
    /*for (;;) {*/
    sleep(10);
    timer_delete(t_data->timer_id);
    printf("After 2 second timer_delete()\n");
    /*fflush(stdout);*/
    pause();
    return 0;
}

