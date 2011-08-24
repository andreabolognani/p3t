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

#include <p3t_application.h>
#include <p3t_clock.h>
#include <p3t_timer.h>
#include <p3t_point.h>
#include <p3t_box.h>
#include <p3t_pixmap.h>
#include <p3t_timerWidget.h>
#include <p3t_button.h>

#include <stdlib.h>
#include <nds.h>

#define TIMERS_NUMBER      8
#define SECONDS_PER_MINUTE 60
#define BACKLIGHT_TIMEOUT  30

typedef enum {
	APPLICATION_STATE_ALL,
	APPLICATION_STATE_ONE
} applicationState;

struct _p3t_applicationPrivate {
	p3t_timer         **timers;
	p3t_timerWidget   **widgets;
	p3t_timerWidget    *active;
	p3t_button         *actionButton;
	p3t_button         *upButton;
	p3t_button         *downButton;
	applicationState    state;
	applicationState    lastState;
	int                 idle;
	int                 lastActivity;
};

static void
paintCallback (p3t_widget  *widget,
               void        *data)
{
	p3t_application *self;
	p3t_applicationPrivate *priv;
	p3t_pixmap *background;
	p3t_box *box;
	int i;

	self = P3T_APPLICATION (widget);
	priv = self->priv;

	/* It doesn't make much sense to paint stuff to the
	 * screen while the backlight is off, does it? */
	if (priv->idle) {
		return;
	}

	/* There's no need to paint the background every single
	 * vblank, just draw it when the mode changes */
	if (priv->state != priv->lastState) {

		/* Paint background image */
		box = p3t_boxNew (0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		background = p3t_pixmapGet (P3T_PIXMAP_TYPE_BACKGROUND,
									P3T_PIXMAP_BACKGROUND_DEFAULT);
		p3t_pixmapDraw (background, box);

		priv->lastState = priv->state;
	}

	if (priv->state == APPLICATION_STATE_ONE) {

		/* Paint the active timerWidget */
		p3t_widgetPaint (P3T_WIDGET (priv->active));

		p3t_widgetPaint (P3T_WIDGET (priv->actionButton));
		p3t_widgetPaint (P3T_WIDGET (priv->upButton));
		p3t_widgetPaint (P3T_WIDGET (priv->downButton));
	}
	else {

		for (i = 0; i < TIMERS_NUMBER; i++) {

			/* Paint all the timerWidgets */
			p3t_widgetPaint (P3T_WIDGET (priv->widgets[i]));
		}
	}
}

static void
activateCallback (p3t_widget  *widget,
                  p3t_point   *stylus,
                  void        *data)
{
	p3t_application *self;
	p3t_applicationPrivate *priv;
	int i;

	self = P3T_APPLICATION (widget);
	priv = self->priv;

	/* Update the idle counter */
	priv->lastActivity = p3t_clockGetSeconds ();

	if (priv->idle) {

		/* Turn the backlight on */
		priv->idle = 0;
		powerOn (PM_BACKLIGHT_BOTTOM);

#ifdef DEVELOPMENT_BUILD
		printf ("[%d] Backlight ON\n",
		        p3t_clockGetSeconds ());
#endif

		return;
	}

	for (i = 0; i < TIMERS_NUMBER; i++) {
		p3t_widgetTryActivate (P3T_WIDGET (priv->widgets[i]),
							   stylus);
	}

	/* Try to activate the action buttons */
	if (priv->state == APPLICATION_STATE_ONE) {
		p3t_widgetTryActivate (P3T_WIDGET (priv->actionButton),
							   stylus);
		p3t_widgetTryActivate (P3T_WIDGET (priv->upButton),
							   stylus);
		p3t_widgetTryActivate (P3T_WIDGET (priv->downButton),
							   stylus);
	}
}

static void
actionButtonActivateCallback (p3t_widget  *widget,
                              p3t_point   *stylus,
                              void        *data)
{
	p3t_timer *timer;

	timer = P3T_TIMER (data);

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

static void
upButtonActivateCallback (p3t_widget  *widget,
                          p3t_point   *stylus,
                          void        *data)
{
	p3t_timer *timer;

	timer = P3T_TIMER (data);

	p3t_timerIncreaseTargetTime (timer);
}

static void
downButtonActivateCallback (p3t_widget  *widget,
                            p3t_point   *stylus,
                            void        *data)
{
	p3t_timer *timer;

	timer = P3T_TIMER (data);

	p3t_timerDecreaseTargetTime (timer);
}

static void
timerWidgetActivateCallback (p3t_widget  *widget,
                             p3t_point   *stylus,
                             void        *data)
{
	p3t_timerWidget *self;
	p3t_timerWidget *temp;
	p3t_application *application;
	p3t_button *button;
	p3t_timer *timer;
	int x;
	int i;

	self = P3T_TIMERWIDGET (widget);
	application = P3T_APPLICATION (data);
	timer = p3t_timerWidgetGetTimer (self);

	switch (application->priv->state) {

		case APPLICATION_STATE_ALL:

			for (i = 0; i < TIMERS_NUMBER; i++) {

				if (i != (p3t_timerGetNumber (timer) - 1)) {

					temp = application->priv->widgets[i];
					p3t_boxSavePosition (P3T_BOX (temp));
					p3t_boxMove (P3T_BOX (temp), 0, SCREEN_HEIGHT);
				}

			}

			if (p3t_boxGetX (P3T_BOX (self)) <= (SCREEN_WIDTH / 2)) {
				x = 169;
				/* The active timerWidget is on the left side */
			}
			else {
				x = 43;  /* The active timerWidget is on the right side */
			}

			/* Action button */
			button = p3t_buttonNew (x, 9, 44, 44, P3T_PIXMAP_BUTTON_ACTION);
			p3t_widgetSetActivateCallback (P3T_WIDGET (button),
			                               &actionButtonActivateCallback,
			                               p3t_timerWidgetGetTimer (self));
			application->priv->actionButton = button;

			/* Up button */
			button = p3t_buttonNew (x, 74, 44, 44, P3T_PIXMAP_BUTTON_UP);
			p3t_widgetSetActivateCallback (P3T_WIDGET (button),
			                               &upButtonActivateCallback,
			                               p3t_timerWidgetGetTimer (self));
			application->priv->upButton = button;

			/* Down button */
			button = p3t_buttonNew (x, 139, 44, 44, P3T_PIXMAP_BUTTON_DOWN);
			p3t_widgetSetActivateCallback (P3T_WIDGET (button),
			                               &downButtonActivateCallback,
			                               p3t_timerWidgetGetTimer (self));
			application->priv->downButton = button;

			application->priv->state = APPLICATION_STATE_ONE;
			application->priv->active = self;
			break;

		case APPLICATION_STATE_ONE:

			for (i = 0; i < TIMERS_NUMBER; i++) {

				temp = application->priv->widgets[i];
				p3t_boxRestorePosition (P3T_BOX (temp));
			}

			if (application->priv->actionButton != NULL) {
				p3t_buttonDestroy (application->priv->actionButton);
				application->priv->actionButton = NULL;
			}
			if (application->priv->upButton != NULL) {
				p3t_buttonDestroy (application->priv->upButton);
				application->priv->upButton = NULL;
			}
			if (application->priv->downButton != NULL) {
				p3t_buttonDestroy (application->priv->downButton);
				application->priv->downButton = NULL;
			}

			application->priv->state = APPLICATION_STATE_ALL;
			application->priv->active = NULL;
			break;
	}
}

void
_p3t_applicationInit (p3t_application *self)
{
	p3t_applicationPrivate *priv;
	int i;

	_p3t_widgetInit (P3T_WIDGET (self), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	priv = malloc (sizeof (p3t_applicationPrivate));

	priv->timers = malloc (TIMERS_NUMBER * sizeof (p3t_timer *));
	priv->widgets = malloc (TIMERS_NUMBER * sizeof (p3t_timerWidget *));

	/* Create all the widgets */
	priv->widgets[0] = p3t_timerWidgetNew (5, 5, 122, 44);
	priv->widgets[1] = p3t_timerWidgetNew (5, 51, 122, 44);
	priv->widgets[2] = p3t_timerWidgetNew (5, 97, 122, 44);
	priv->widgets[3] = p3t_timerWidgetNew (5, 143, 122, 44);
	priv->widgets[4] = p3t_timerWidgetNew (129, 5, 122, 44);
	priv->widgets[5] = p3t_timerWidgetNew (129, 51, 122, 44);
	priv->widgets[6] = p3t_timerWidgetNew (129, 97, 122, 44);
	priv->widgets[7] = p3t_timerWidgetNew (129, 143, 122, 44);

	/* Create all the timers and assign each to its widget */
	for (i = 0; i < TIMERS_NUMBER; i++) {

		priv->timers[i] = p3t_timerNew (i + 1);
		p3t_timerWidgetSetTimer (priv->widgets[i], priv->timers[i]);
		p3t_widgetSetActivateCallback (P3T_WIDGET (priv->widgets[i]),
		                               &timerWidgetActivateCallback,
		                               self);
	}

	priv->active = NULL;
	priv->actionButton = NULL;
	priv->upButton = NULL;
	priv->downButton = NULL;

	priv->state = APPLICATION_STATE_ALL;
	priv->lastState = APPLICATION_STATE_ONE;

	priv->idle = 0;
	priv->lastActivity = p3t_clockGetSeconds ();

	self->priv = priv;

	p3t_widgetSetPaintCallback (P3T_WIDGET (self),
	                            &paintCallback,
	                            NULL);
	p3t_widgetSetActivateCallback (P3T_WIDGET (self),
	                               &activateCallback,
	                               NULL);
}

void
_p3t_applicationFinalize (p3t_application *self)
{
	p3t_applicationPrivate *priv;
	int i;

	priv = self->priv;

	/* Destroy all timers and widgets */
	for (i = 0; i < TIMERS_NUMBER; i++) {
		p3t_timerWidgetDestroy (priv->widgets[i]);
	}

	if (priv->actionButton != NULL) {
		p3t_buttonDestroy (priv->actionButton);
	}
	if (priv->upButton != NULL) {
		p3t_buttonDestroy (priv->upButton);
	}
	if (priv->downButton != NULL) {
		p3t_buttonDestroy (priv->downButton);
	}

	/* Destroy containers */
	free (priv->timers);
	free (priv->widgets);

	free (priv);

	_p3t_widgetFinalize (P3T_WIDGET (self));
}

p3t_application*
p3t_applicationNew (void)
{
	p3t_application *self;

	self = malloc (sizeof (p3t_application));
	_p3t_applicationInit (self);

	return self;
}

void
p3t_applicationDestroy (p3t_application *self)
{
	_p3t_applicationFinalize (self);
	free (self);
}

void
p3t_applicationUpdate (p3t_application *self)
{
	p3t_applicationPrivate *priv;
	p3t_timer *timer;
	int elapsed;
	int target;
	int i;

	priv = self->priv;

	if ((p3t_clockGetSeconds () - priv->lastActivity) >= BACKLIGHT_TIMEOUT) {

		/* Turn backlight off */
		priv->idle = 1;
		powerOff (PM_BACKLIGHT_BOTTOM);

#ifdef DEVELOPMENT_BUILD
		printf ("[%d] Backlight OFF\n",
		        p3t_clockGetSeconds ());
#endif
	}

	for (i = 0; i < TIMERS_NUMBER; i++) {

		timer = priv->timers[i];

		elapsed = p3t_timerGetElapsedSeconds (timer);
		target = p3t_timerGetTargetSeconds (timer);

		if (elapsed >= target) {
			p3t_timerFinish (timer);
		}
	}
}
