#include <p3t_application.h>
#include <p3t_timer.h>
#include <p3t_point.h>
#include <p3t_box.h>

#include <stdlib.h>
#include <stdio.h>

#include <nds.h>

#define TIMERS_NUMBER      8
#define SECONDS_PER_MINUTE 60

struct _p3t_application {
	p3t_timer  **timers;
	p3t_box    **boxes;
	int          currentTimer;
};

static void
init (p3t_application *self)
{
	int i;

	self->timers = malloc (TIMERS_NUMBER * sizeof (p3t_timer *));
	self->boxes  = malloc (TIMERS_NUMBER * sizeof (p3t_box *));

	/* Create all the needed timers */
	for (i = 0; i < TIMERS_NUMBER; i++) {
		self->timers[i] = p3t_timerNew (i + 1);
	}

	self->boxes[0] = p3t_boxNew (3, 3, 124, 45);
	self->boxes[1] = p3t_boxNew (3, 50, 124, 45);
	self->boxes[2] = p3t_boxNew (3, 97, 124, 45);
	self->boxes[3] = p3t_boxNew (3, 144, 124, 45);
	self->boxes[4] = p3t_boxNew (0, 0, 0, 0);
	self->boxes[5] = p3t_boxNew (0, 0, 0, 0);
	self->boxes[6] = p3t_boxNew (0, 0, 0, 0);
	self->boxes[7] = p3t_boxNew (0, 0, 0, 0);

	/* Make the first timer current */
	self->currentTimer = 0;
}

static void
finalize (p3t_application *self)
{
	int i;

	/* Destroy all timers and boxes */
	for (i = 0; i < TIMERS_NUMBER; i++) {
		p3t_timerDestroy (self->timers[i]);
		p3t_boxDestroy (self->boxes[i]);
	}

	/* Destroy containers */
	free (self->timers);
	free (self->boxes);
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
p3t_applicationDestroy (p3t_application *self)
{
	finalize (self);
	free (self);
}

void
p3t_applicationUpdate (p3t_application *self,
                       int              input)
{
	p3t_timer *timer;
	p3t_point *stylus;
	touchPosition touch;
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

	if (input & KEY_TOUCH) {

		/* Convert a touchPosition to a p3t_point */
		touchRead (&touch);
		stylus = p3t_pointNew (touch.px, touch.py);

		for (i = 0; i < TIMERS_NUMBER; i++) {

			if (p3t_boxContainsPoint (self->boxes[i], stylus)) {

				if (self->currentTimer == i) {

					timer = self->timers[self->currentTimer];

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
				else {
					self->currentTimer = i;
					printf ("[%d %d] Current timer\n",
					        p3t_clockGetSeconds (),
					        i + 1);
				}
			}
		}

		p3t_pointDestroy (stylus);
	}
}
