#include <p3t_application.h>
#include <p3t_timer.h>

#include <stdlib.h>
#include <stdio.h>

#include <nds.h>

#define TIMERS_NUMBER      8
#define SECONDS_PER_MINUTE 60

struct _p3t_application {
	p3t_timer  **timers;
	int          currentTimer;
};

static void
init (p3t_application *self)
{
	int i;

	self->timers = malloc (TIMERS_NUMBER * sizeof (p3t_timer *));

	/* Create all the needed timers */
	for (i = 0; i < TIMERS_NUMBER; i++) {
		self->timers[i] = p3t_timerNew (i + 1);
	}

	/* Make the first timer current */
	self->currentTimer = 0;
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
	p3t_timer *timer;
	int elapsed;
	int target;
	int i;

	for (i = 0; i < TIMERS_NUMBER; i++) {

		timer = self->timers[i];

		elapsed = p3t_timerGetElapsedSeconds (timer);
		target = p3t_timerGetTargetSeconds (timer);

		if (elapsed >= target) {
			p3t_timerFinish (timer);
		}
	}

	timer = self->timers[self->currentTimer];

	if (input & KEY_UP) {

		self->currentTimer++;

		/* Wrap to the first timer */
		if (self->currentTimer >= TIMERS_NUMBER) {
			self->currentTimer = 0;
		}
	}

	if (input & KEY_DOWN) {
		self->currentTimer--;

		/* Wrap to the last timer */
		if (self->currentTimer < 0) {
			self->currentTimer = TIMERS_NUMBER - 1;
		}
	}

	if (input & KEY_TOUCH) {

		switch (p3t_timerGetState (timer)) {

			case P3T_TIMER_STATE_STOPPED:
			case P3T_TIMER_STATE_PAUSED:

				p3t_timerStart (timer);
				break;

			case P3T_TIMER_STATE_RUNNING:

				p3t_timerPause (timer);
				break;

			case P3T_TIMER_STATE_FINISHED:

				p3t_timerStop (timer);
				break;
		}
	}
}
