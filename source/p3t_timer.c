#include <p3t_timer.h>

#define SECONDS_PER_MINUTE 60

static int
lastElapsed (p3t_timer *self)
{
	return (p3t_clockGetSeconds () - self->startSeconds);
}

void
p3t_timerInit (p3t_timer  *self,
               int         number)
{
	self->number = number;
	self->time = 8 * SECONDS_PER_MINUTE;
	self->state = TIMER_STATE_STOPPED;
}

int
p3t_timerGetNumber (p3t_timer *self)
{
	return self->number;
}

int
p3t_timerGetElapsed (p3t_timer *self)
{
	return (self->elapsedSeconds + lastElapsed (self));
}

void
p3t_timerStart (p3t_timer *self)
{
	if (self->state != TIMER_STATE_STOPPED)
		return;

	self->startSeconds = p3t_clockGetSeconds ();
	self->elapsedSeconds = 0;

	self->state = TIMER_STATE_RUNNING;
}

void
p3t_timerPause (p3t_timer *self)
{
	if (self->state != TIMER_STATE_RUNNING)
		return;

	self->elapsedSeconds += lastElapsed (self);

	self->state = TIMER_STATE_PAUSED;
}

void
p3t_timerContinue (p3t_timer *self)
{
	if (self->state != TIMER_STATE_PAUSED)
		return;

	self->startSeconds = p3t_clockGetSeconds ();

	self->state = TIMER_STATE_RUNNING;
}

void
p3t_timerStop (p3t_timer *self)
{
	self->state = TIMER_STATE_STOPPED; 
}

void
p3t_timerFinish (p3t_timer *self)
{
	if (self->state != TIMER_STATE_RUNNING)
		return;

	self->elapsedSeconds += lastElapsed (self);

	self->state = TIMER_STATE_FINISHED;
}
