#include <p3t_timer.h>
#include <p3t_clock.h>

#include <stdlib.h>
#include <stdio.h>

#define SECONDS_PER_MINUTE (60)
#define TARGET_SECONDS     (1 * SECONDS_PER_MINUTE)

struct _p3t_timer {
	int            number;
	int            targetSeconds;
	int            startSeconds;
	int            elapsedSeconds;
	p3t_timerState state;
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

p3t_timer*
p3t_timerNew (int number)
{
	p3t_timer *self;

	self = malloc (sizeof (p3t_timer));
	init (self, number);

	return self;
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
p3t_timerGetElapsed (p3t_timer *self)
{
	return (self->elapsedSeconds + lastElapsed (self));
}

char*
p3t_timerGetElapsedAsString (p3t_timer *self)
{
	return secondsAsString (p3t_timerGetElapsed (self));
}

int
p3t_timerGetMinutes (p3t_timer *self)
{
	return (self->targetSeconds / SECONDS_PER_MINUTE);
}

char*
p3t_timerGetMinutesAsString (p3t_timer *self)
{
	return secondsAsString (self->targetSeconds);
}

void
p3t_timerIncreaseMinutes (p3t_timer *self)
{
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
p3t_timerDecreaseMinutes (p3t_timer *self)
{
	/* Stop the timer if it's not stopped already */
	if (self->state != P3T_TIMER_STATE_STOPPED) {
		p3t_timerStop (self);
	}

	/* Go from the middle of a minute to the start of that minute,
	 * and from the beginning of a minute to the beginning of the
	 * previous minute */
	if ((self->targetSeconds % SECONDS_PER_MINUTE) > 0) {
		self->targetSeconds -= (self->targetSeconds);
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

	self->startSeconds = p3t_clockGetSeconds ();

	self->state = P3T_TIMER_STATE_RUNNING;

	printf ("[%d %d] Started (%s / %s)\n",
			p3t_clockGetSeconds (),
			p3t_timerGetNumber (self),
	        p3t_timerGetElapsedAsString (self),
	        p3t_timerGetMinutesAsString (self));
}

void
p3t_timerPause (p3t_timer *self)
{
	if (self->state != P3T_TIMER_STATE_RUNNING)
		return;

	self->elapsedSeconds += lastElapsed (self);

	self->state = P3T_TIMER_STATE_PAUSED;

	printf ("[%d %d] Paused (%s / %s)\n",
			p3t_clockGetSeconds (),
			p3t_timerGetNumber (self),
	        p3t_timerGetElapsedAsString (self),
	        p3t_timerGetMinutesAsString (self));
}

void
p3t_timerStop (p3t_timer *self)
{
	self->elapsedSeconds = 0;

	self->state = P3T_TIMER_STATE_STOPPED;

	printf ("[%d %d] Stopped\n",
			p3t_clockGetSeconds (),
			p3t_timerGetNumber (self));
}

void
p3t_timerFinish (p3t_timer *self)
{
	if (self->state != P3T_TIMER_STATE_RUNNING)
		return;

	self->elapsedSeconds = 0;

	self->state = P3T_TIMER_STATE_FINISHED;

	printf ("[%d %d] Finished (%s)\n",
			p3t_clockGetSeconds (),
			p3t_timerGetNumber (self),
	        p3t_timerGetMinutesAsString (self));
}
