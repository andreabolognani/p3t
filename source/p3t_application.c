#include <p3t_application.h>
#include <p3t_timer.h>
#include <p3t_point.h>
#include <p3t_box.h>
#include <p3t_widget.h>
#include <p3t_timerWidget.h>

#include <stdlib.h>
#include <stdio.h>

#include <nds.h>

#define TIMERS_NUMBER      8
#define SECONDS_PER_MINUTE 60

struct _p3t_application {
	p3t_timer        **timers;
	p3t_timerWidget  **widgets;
};

static void
init (p3t_application *self)
{
	int i;

	self->timers = malloc (TIMERS_NUMBER * sizeof (p3t_timer *));
	self->widgets = malloc (TIMERS_NUMBER * sizeof (p3t_timerWidget *));

	/* Create all the needed timers */
	for (i = 0; i < TIMERS_NUMBER; i++) {
		self->timers[i] = p3t_timerNew (i + 1);
		self->widgets[i] = p3t_timerWidgetNew ();
		p3t_timerWidgetSetTimer (self->widgets[i], self->timers[i]);
	}

	p3t_widgetSetBox (P3T_WIDGET (self->widgets[0]),
	                  p3t_boxNew (3, 3, 124, 45));
	p3t_widgetSetBox (P3T_WIDGET (self->widgets[1]),
	                  p3t_boxNew (3, 50, 124, 45));
	p3t_widgetSetBox (P3T_WIDGET (self->widgets[2]),
	                  p3t_boxNew (3, 97, 124, 45));
	p3t_widgetSetBox (P3T_WIDGET (self->widgets[3]),
	                  p3t_boxNew (3, 144, 124, 45));
	p3t_widgetSetBox (P3T_WIDGET (self->widgets[4]),
	                  p3t_boxNew (129, 3, 124, 45));
	p3t_widgetSetBox (P3T_WIDGET (self->widgets[5]),
	                  p3t_boxNew (129, 50, 124, 45));
	p3t_widgetSetBox (P3T_WIDGET (self->widgets[6]),
	                  p3t_boxNew (129, 97, 124, 45));
	p3t_widgetSetBox (P3T_WIDGET (self->widgets[7]),
	                  p3t_boxNew (129, 144, 124, 45));
}

static void
finalize (p3t_application *self)
{
	int i;

	/* Destroy all timers and widgets */
	for (i = 0; i < TIMERS_NUMBER; i++) {
		p3t_timerWidgetDestroy (self->widgets[i]);
	}

	/* Destroy containers */
	free (self->timers);
	free (self->widgets);
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
			p3t_widgetTryActivate (P3T_WIDGET (self->widgets[i]),
			                       stylus);
		}

		p3t_pointDestroy (stylus);
	}
}
