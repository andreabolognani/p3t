#ifndef __P3T_TIMER_H__
#define __P3T_TIMER_H__

#include <p3t_clock.h>

typedef struct _p3t_timer p3t_timer;

typedef enum {
	P3T_TIMER_STATE_STOPPED,
	P3T_TIMER_STATE_RUNNING,
	P3T_TIMER_STATE_PAUSED,
	P3T_TIMER_STATE_FINISHED
} p3t_timerState;

p3t_timer*      p3t_timerNew                  (int         number);

int             p3t_timerGetNumber            (p3t_timer  *timer);
p3t_timerState  p3t_timerGetState             (p3t_timer  *timer);

int             p3t_timerGetElapsedSeconds    (p3t_timer  *timer);
char*           p3t_timerGetElapsedTime       (p3t_timer  *timer);
int             p3t_timerGetTargetSeconds     (p3t_timer  *timer);
char*           p3t_timerGetTargetTime        (p3t_timer  *timer);
int             p3t_timerGetRemainingSeconds  (p3t_timer  *timer);
char*           p3t_timerGetRemainingTime     (p3t_timer  *timer);

void            p3t_timerStart                (p3t_timer  *timer);
void            p3t_timerPause                (p3t_timer  *timer);
void            p3t_timerStop                 (p3t_timer  *timer);
void            p3t_timerFinish               (p3t_timer  *timer);

void            p3t_timerIncreaseTargetTime   (p3t_timer  *timer);
void            p3t_timerDecreaseTargetTime   (p3t_timer  *timer);

#endif /* __P3T_TIMER_H__ */
