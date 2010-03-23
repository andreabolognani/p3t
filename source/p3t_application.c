#include <p3t_application.h>
#include <p3t_timer.h>

#include <stdlib.h>
#include <stdio.h>

#include <nds.h>

#define SECONDS_PER_MINUTE 60

struct _p3t_application {
	p3t_timer  *timer;
};

static void
init (p3t_application *self)
{
	self->timer = p3t_timerNew (1);
}

p3t_application*
p3t_applicationNew (void)
{
	p3t_application *self;

	self = malloc (sizeof (p3t_application));
	init (self);

	return self;
}

void
p3t_applicationUpdate (p3t_application *self,
                       int              input)
{
	int elapsed;
	int target;

	elapsed = p3t_timerGetElapsed (self->timer);
	target = p3t_timerGetMinutes (self->timer) * SECONDS_PER_MINUTE;

	if (elapsed >= target) {
		p3t_timerFinish (self->timer);
	}

	if (input & KEY_TOUCH) {

		switch (p3t_timerGetState (self->timer)) {

			case P3T_TIMER_STATE_STOPPED:

				p3t_timerStart (self->timer);
				break;

			case P3T_TIMER_STATE_RUNNING:

				p3t_timerPause (self->timer);
				break;

			case P3T_TIMER_STATE_PAUSED:

				p3t_timerContinue (self->timer);
				break;

			case P3T_TIMER_STATE_FINISHED:

				p3t_timerStop (self->timer);
				break;
		}
	}
}
