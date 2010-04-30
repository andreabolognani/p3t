/* P3T - Coundown timers for Nintendo DS
 * Copyright (C) 2010  Andrea Bolognani <eof@kiyuko.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __P3T_TIMER_H__
#define __P3T_TIMER_H__

#include <p3t_clock.h>

#define P3T_TIMER(x) ((p3t_timer*) (x))

typedef struct _p3t_timer p3t_timer;

typedef enum {
	P3T_TIMER_STATE_STOPPED,
	P3T_TIMER_STATE_RUNNING,
	P3T_TIMER_STATE_PAUSED,
	P3T_TIMER_STATE_FINISHED
} p3t_timerState;

p3t_timer*      p3t_timerNew                  (int         number);
void            p3t_timerDestroy              (p3t_timer  *timer);

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
