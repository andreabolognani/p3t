/* P3T - Coundown timers for Nintendo DS
 * Copyright (C) 2010-2011  Andrea Bolognani <eof@kiyuko.org>
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

#include <p3t_timer.h>
#include <p3t_clock.h>

#include <stdlib.h>
#include <maxmod9.h>

#include <soundbank.h>

#ifdef DEVELOPMENT_BUILD
#include <stdio.h>
#endif

#define SECONDS_PER_MINUTE  60

#ifdef DEVELOPMENT_BUILD
#define TARGET_SECONDS      (1 * SECONDS_PER_MINUTE)
#else
#define TARGET_SECONDS      (10 * SECONDS_PER_MINUTE)
#endif

struct _p3t_timer {
	int             number;
	int             targetSeconds;
	int             startSeconds;
	int             elapsedSeconds;
	p3t_timerState  state;
	mm_sfxhand      sfx;
};

static int
lastElapsed (p3t_timer *self)
{
	/* If the timer is not running, the elapsed time is
	 * recorded into the timer and there's no need to add
	 * anything to it */
	if (self->state != P3T_TIMER_STATE_RUNNING)
		return 0;

	/* Add the time that has passed since the timer was
	 * last started/restarted */
	return (p3t_clockGetSeconds () - self->startSeconds);
}

static char*
secondsAsString (int elapsed)
{
	char *asString;
	int minutes;
	int seconds;

	/* Assume the time is between 00:00 and 99:99 */
	asString = (char *) malloc (6 * sizeof (char));
	asString[2] = ':';
	asString[5] = '\0';

	seconds = elapsed % SECONDS_PER_MINUTE;
	minutes = (elapsed - seconds) / SECONDS_PER_MINUTE;

	asString[1] = (minutes % 10) + 48;
	asString[0] = ((minutes - (asString[1] - 48)) / 10) + 48;

	asString[4] = (seconds % 10) + 48;
	asString[3] = ((seconds - (asString[4] - 48)) / 10) + 48;

	return asString;
}

static void
init (p3t_timer  *self,
      int         number)
{
	self->number = number;
	self->targetSeconds = TARGET_SECONDS;
	self->state = P3T_TIMER_STATE_STOPPED;
}

static void
finalize (p3t_timer *self)
{
	/* Just to be sure */
	p3t_timerStop (self);
}

p3t_timer*
p3t_timerNew (int number)
{
	p3t_timer *self;

	self = malloc (sizeof (p3t_timer));
	init (self, number);

	return self;
}

void
p3t_timerDestroy (p3t_timer *self)
{
	finalize (self);
	free (self);
}

int
p3t_timerGetNumber (p3t_timer *self)
{
	return self->number;
}

p3t_timerState
p3t_timerGetState (p3t_timer *self)
{
	return self->state;
}

int
p3t_timerGetElapsedSeconds (p3t_timer *self)
{
	return (self->elapsedSeconds + lastElapsed (self));
}

char*
p3t_timerGetElapsedTime (p3t_timer *self)
{
	return secondsAsString (p3t_timerGetElapsedSeconds (self));
}

int
p3t_timerGetTargetSeconds (p3t_timer *self)
{
	return self->targetSeconds;
}

char*
p3t_timerGetTargetTime (p3t_timer *self)
{
	return secondsAsString (p3t_timerGetTargetSeconds (self));
}

int
p3t_timerGetRemainingSeconds (p3t_timer *self)
{
	return (p3t_timerGetTargetSeconds (self) -
	        p3t_timerGetElapsedSeconds (self));
}

char*
p3t_timerGetRemainingTime (p3t_timer *self)
{
	return secondsAsString (p3t_timerGetRemainingSeconds (self));
}

void
p3t_timerIncreaseTargetTime (p3t_timer *self)
{
	/* Set the target time to the remaining time */
	self->targetSeconds = p3t_timerGetRemainingSeconds (self);

	/* Stop the timer if it's not stopped already */
	if (self->state != P3T_TIMER_STATE_STOPPED) {
		p3t_timerStop (self);
	}

	/* Round it down to the nearest minute, then add a minute */
	self->targetSeconds -= (self->targetSeconds % SECONDS_PER_MINUTE);
	self->targetSeconds += SECONDS_PER_MINUTE;

	/* Don't go over 99 minutes */
	if (self->targetSeconds >= (99 * SECONDS_PER_MINUTE)) {
		self->targetSeconds = 99 * SECONDS_PER_MINUTE;
	}
}

void
p3t_timerDecreaseTargetTime (p3t_timer *self)
{
	/* Set the target time to the remaining time */
	self->targetSeconds = p3t_timerGetRemainingSeconds (self);

	/* Stop the timer if it's not stopped already */
	if (self->state != P3T_TIMER_STATE_STOPPED) {
		p3t_timerStop (self);
	}

	/* Go from the middle of a minute to the start of that minute,
	 * and from the beginning of a minute to the beginning of the
	 * previous minute */
	if ((self->targetSeconds % SECONDS_PER_MINUTE) > 0) {
		self->targetSeconds -= (self->targetSeconds % SECONDS_PER_MINUTE);
	}
	else {
		self->targetSeconds -= SECONDS_PER_MINUTE;
	}

	/* Stop decreasing at 0 minutes */
	if (self->targetSeconds <= 0) {
		self->targetSeconds = 0;
	}
}

void
p3t_timerStart (p3t_timer *self)
{
	if (self->state != P3T_TIMER_STATE_STOPPED &&
	    self->state != P3T_TIMER_STATE_PAUSED)
		return;

	/* Don't start if the target time is 0 seconds */
	if (self->targetSeconds <= 0) {
		return;
	}

	self->startSeconds = p3t_clockGetSeconds ();

	self->state = P3T_TIMER_STATE_RUNNING;

#ifdef DEVELOPMENT_BUILD
	printf ("[%d %d] Started (%s)\n",
	        p3t_clockGetSeconds (),
	        p3t_timerGetNumber (self),
	        p3t_timerGetRemainingTime (self));
#endif
}

void
p3t_timerPause (p3t_timer *self)
{
	if (self->state != P3T_TIMER_STATE_RUNNING)
		return;

	self->elapsedSeconds += lastElapsed (self);

	self->state = P3T_TIMER_STATE_PAUSED;

#ifdef DEVELOPMENT_BUILD
	printf ("[%d %d] Paused (%s)\n",
	        p3t_clockGetSeconds (),
	        p3t_timerGetNumber (self),
	        p3t_timerGetRemainingTime (self));
#endif
}

void
p3t_timerStop (p3t_timer *self)
{
	self->elapsedSeconds = 0;

	self->state = P3T_TIMER_STATE_STOPPED;

	mmEffectCancel (self->sfx);
	mmEffectRelease (self->sfx);

#ifdef DEVELOPMENT_BUILD
	printf ("[%d %d] Stopped\n",
	        p3t_clockGetSeconds (),
	        p3t_timerGetNumber (self));
#endif
}

void
p3t_timerFinish (p3t_timer *self)
{
	if (self->state != P3T_TIMER_STATE_RUNNING)
		return;

	self->state = P3T_TIMER_STATE_FINISHED;

	self->elapsedSeconds = self->targetSeconds;

	mmLoadEffect (SFX_FINISH);
	self->sfx = mmEffect (SFX_FINISH);

#ifdef DEVELOPMENT_BUILD
	printf ("[%d %d] Finished (%s)\n",
	        p3t_clockGetSeconds (),
	        p3t_timerGetNumber (self),
	        p3t_timerGetTargetTime (self));
#endif
}
