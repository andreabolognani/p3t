#include <p3t_application.h>
#include <p3t_timer.h>
#include <p3t_point.h>
#include <p3t_box.h>
#include <p3t_pixmap.h>
#include <p3t_widget.h>
#include <p3t_timerWidget.h>

#include <stdlib.h>
#include <nds.h>
#include <maxmod9.h>

#include <soundbank.h>
#include <soundbank_bin.h>

#define TIMERS_NUMBER      8
#define SECONDS_PER_MINUTE 60

typedef enum {
	APPLICATION_STATE_ALL,
	APPLICATION_STATE_ONE
} applicationState;

struct _p3t_application {
	u16                *backgroundBuffer;
	u16                *widgetsBuffer;
	p3t_timer         **timers;
	p3t_timerWidget   **widgets;
	applicationState    state;
};

static void
activateCallback (p3t_widget  *widget,
                  void        *data)
{
	p3t_timerWidget *self;
	p3t_timerWidget *temp;
	p3t_application *application;
	p3t_timer *timer;
	int i;

	self = P3T_TIMERWIDGET (widget);
	application = P3T_APPLICATION (data);
	timer = p3t_timerWidgetGetTimer (self);

	switch (application->state) {

		case APPLICATION_STATE_ALL:

			for (i = 0; i < TIMERS_NUMBER; i++) {

				if (i != (p3t_timerGetNumber (timer) - 1)) {

					temp = application->widgets[i];
					p3t_boxSavePosition (P3T_BOX (temp));
					p3t_boxMove (P3T_BOX (temp), 0, SCREEN_HEIGHT);
				}

			}

			application->state = APPLICATION_STATE_ONE;
			break;

		case APPLICATION_STATE_ONE:

			for (i = 0; i < TIMERS_NUMBER; i++) {

				temp = application->widgets[i];
				p3t_boxRestorePosition (P3T_BOX (temp));
			}

			application->state = APPLICATION_STATE_ALL;
			break;
	}

#if 0
	if (timer != NULL) {
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
#endif
}

static void
paintCallback (p3t_widget  *widget,
               void        *data)
{
	p3t_timerWidget *self;
	p3t_application *application;
	p3t_timer *timer;
	p3t_pixmap *pixmap;
	p3t_box *box;
	char *remaining;

	self = P3T_TIMERWIDGET (widget);
	application = P3T_APPLICATION (data);
	timer = p3t_timerWidgetGetTimer (self);
	remaining = p3t_timerGetRemainingTime (timer);

	/* Outline */
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_OUTLINE,
	                        P3T_PIXMAP_OUTLINE_TIMERWIDGET);
	p3t_pixmapDraw (pixmap,
	                P3T_BOX (self),
	                p3t_applicationGetWidgetsBuffer (application));

	/* Timer number */
	box = p3t_boxNew (5, 5, 18, 22);
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_NUMBER,
	                        p3t_timerGetNumber (timer) - 1);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap,
	                box,
	                p3t_applicationGetWidgetsBuffer (application));
	p3t_boxDestroy (box);

	/* Last digit */
	box = p3t_boxNew (100, 5, 18, 34);
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_DIGIT,
	                        remaining[4] - 48);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap,
	                box,
	                p3t_applicationGetWidgetsBuffer (application));
	p3t_boxDestroy (box);

	/* Third digit */
	box = p3t_boxNew (81, 5 ,18, 34);
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_DIGIT,
	                        remaining[3] - 48);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap,
	                box,
	                p3t_applicationGetWidgetsBuffer (application));
	p3t_boxDestroy (box);

	/* Second digit */
	box = p3t_boxNew (55, 5 ,18, 34);
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_DIGIT,
	                        remaining[1] - 48);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap,
	                box,
	                p3t_applicationGetWidgetsBuffer (application));
	p3t_boxDestroy (box);

	/* First digit */
	box = p3t_boxNew (36, 5 ,18, 34);
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_DIGIT,
	                        remaining[0] - 48);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap,
	                box,
	                p3t_applicationGetWidgetsBuffer (application));
	p3t_boxDestroy (box);

	free (remaining);
}

static void
init (p3t_application *self)
{
	int i;

	self->timers = malloc (TIMERS_NUMBER * sizeof (p3t_timer *));
	self->widgets = malloc (TIMERS_NUMBER * sizeof (p3t_timerWidget *));

	powerOn (POWER_ALL_2D);

	vramSetBankA (VRAM_A_MAIN_BG_0x06000000);
	vramSetBankB (VRAM_B_MAIN_BG_0x06020000);

	videoSetMode (MODE_5_2D |
	              DISPLAY_BG2_ACTIVE |
	              DISPLAY_BG3_ACTIVE);

	BACKGROUND.control[3] = BG_BMP16_256x256 | BG_BMP_BASE (0);
	BACKGROUND.control[2] = BG_BMP16_256x256 | BG_BMP_BASE (8);

	BACKGROUND.bg3_rotation.xdy = 0;
	BACKGROUND.bg3_rotation.xdx = 1 << 8;
	BACKGROUND.bg3_rotation.ydy = 1 << 8;
	BACKGROUND.bg3_rotation.ydx = 0;
	BACKGROUND.bg2_rotation.xdy = 0;
	BACKGROUND.bg2_rotation.xdx = 1 << 8;
	BACKGROUND.bg2_rotation.ydy = 1 << 8;
	BACKGROUND.bg2_rotation.ydx = 0;

	self->backgroundBuffer = (u16*) BG_BMP_RAM (0);
	self->widgetsBuffer = (u16*) BG_BMP_RAM (8);

#ifdef DEVELOPMENT_BUILD
	consoleDemoInit ();
#endif

	lcdMainOnBottom ();

	mmInitDefaultMem ((mm_addr) soundbank_bin);

	/* Create all the widgets */
	self->widgets[0] = p3t_timerWidgetNew (3, 3, 124, 45);
	self->widgets[1] = p3t_timerWidgetNew (3, 50, 124, 45);
	self->widgets[2] = p3t_timerWidgetNew (3, 97, 124, 45);
	self->widgets[3] = p3t_timerWidgetNew (3, 144, 124, 45);
	self->widgets[4] = p3t_timerWidgetNew (129, 3, 124, 45);
	self->widgets[5] = p3t_timerWidgetNew (129, 50, 124, 45);
	self->widgets[6] = p3t_timerWidgetNew (129, 97, 124, 45);
	self->widgets[7] = p3t_timerWidgetNew (129, 144, 124, 45);

	/* Create all the timers and assign each to its widget */
	for (i = 0; i < TIMERS_NUMBER; i++) {
		self->timers[i] = p3t_timerNew (i + 1);
		p3t_timerWidgetSetTimer (self->widgets[i], self->timers[i]);

		p3t_widgetSetPaintCallback (P3T_WIDGET (self->widgets[i]),
		                            &paintCallback,
		                            self);
		p3t_widgetSetActivateCallback (P3T_WIDGET (self->widgets[i]),
		                               &activateCallback,
		                               self);
	}

	self->state = APPLICATION_STATE_ALL;
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
	p3t_pixmap *background;
	p3t_box *screen;
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

	screen = p3t_boxNew (0, 0, 256, 192);
	background = p3t_pixmapGet (P3T_PIXMAP_TYPE_BACKGROUND,
	                            P3T_PIXMAP_BACKGROUND_DEFAULT);
	p3t_pixmapDraw (background,
	                screen,
	                self->widgetsBuffer);

	for (i = 0; i < TIMERS_NUMBER; i++) {
		p3t_widgetPaint (P3T_WIDGET (self->widgets[i]));
	}
}

u16*
p3t_applicationGetBackgroundBuffer (p3t_application *self)
{
	return self->backgroundBuffer;
}

u16*
p3t_applicationGetWidgetsBuffer (p3t_application *self)
{
	return self->widgetsBuffer;
}
