#ifndef __P3T_TIMER_H__
#define __P3T_TIMER_H__

#include <p3t_clock.h>

typedef struct _p3t_timer p3t_timer;

typedef enum {
	TIMER_STATE_STOPPED,
	TIMER_STATE_RUNNING,
	TIMER_STATE_PAUSED,
	TIMER_STATE_FINISHED
} p3t_timerState;

struct _p3t_timer {
	int            number;
	int            time;
	int            startSeconds;
	int            elapsedSeconds;
	p3t_timerState state;
};

void  p3t_timerInit        (p3t_timer  *timer,
                            int         number);

int   p3t_timerGetNumber   (p3t_timer  *timer);
int   p3t_timerGetElapsed  (p3t_timer  *timer);

void  p3t_timerStart       (p3t_timer  *timer);
void  p3t_timerPause       (p3t_timer  *timer);
void  p3t_timerContinue    (p3t_timer  *timer);
void  p3t_timerStop        (p3t_timer  *timer);
void  p3t_timerFinish      (p3t_timer  *timer);

void  p3t_timerTimeUp      (p3t_timer  *timer);
void  p3t_timerTimeDown    (p3t_timer  *timer);

#endif /* __P3T_TIMER_H__ */
