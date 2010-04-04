#include <p3t_application.h>
#include <p3t_timer.h>
#include <p3t_point.h>
#include <p3t_box.h>
#include <p3t_pixmap.h>
#include <p3t_widget.h>
#include <p3t_timerWidget.h>
#include <p3t_button.h>

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
	u16                *videoBuffer;
	p3t_timer         **timers;
	p3t_timerWidget   **widgets;
	p3t_timerWidget    *active;
	p3t_button         *actionButton;
	p3t_button         *upButton;
	p3t_button         *downButton;
	applicationState    state;
};

static void
actionButtonActivateCallback (p3t_widget  *widget,
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
                          void        *data)
{
	p3t_timer *timer;

	timer = P3T_TIMER (data);

	p3t_timerIncreaseTargetTime (timer);
}

static void
downButtonActivateCallback (p3t_widget  *widget,
                            void        *data)
{
	p3t_timer *timer;

	timer = P3T_TIMER (data);

	p3t_timerDecreaseTargetTime (timer);
}

static void
buttonPaintCallback (p3t_widget  *widget,
                     void        *data)
{
	p3t_button *self;
	p3t_pixmap *pixmap;
	p3t_box *box;
	u16 *videoBuffer;

	self = P3T_BUTTON (widget);
	videoBuffer = (u16*) data;

	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_OUTLINE,
	                        P3T_PIXMAP_OUTLINE_BUTTON);
	p3t_pixmapDraw (pixmap, P3T_BOX (self), videoBuffer);

	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_BUTTON,
	                        p3t_buttonGetType (self));
	box = p3t_boxNew (1, 1, 42, 42);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap, box, videoBuffer);
	p3t_boxDestroy (box);
}

static void
timerWidgetActivateCallback (p3t_widget  *widget,
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

	switch (application->state) {

		case APPLICATION_STATE_ALL:

			for (i = 0; i < TIMERS_NUMBER; i++) {

				if (i != (p3t_timerGetNumber (timer) - 1)) {

					temp = application->widgets[i];
					p3t_boxSavePosition (P3T_BOX (temp));
					p3t_boxMove (P3T_BOX (temp), 0, SCREEN_HEIGHT);
				}

			}

			if (p3t_boxGetX (P3T_BOX (self)) <= (SCREEN_WIDTH / 2)) {
				x = 168;
				/* The active timerWidget is on the left side */
			}
			else {
				x = 44;  /* The active timerWidget is on the right side */
			}

			/* Action button */
			button = p3t_buttonNew (x, 17, 44, 44, P3T_PIXMAP_BUTTON_ACTION);
			p3t_widgetSetActivateCallback (P3T_WIDGET (button),
			                               &actionButtonActivateCallback,
			                               p3t_timerWidgetGetTimer (self));
			p3t_widgetSetPaintCallback (P3T_WIDGET (button),
										&buttonPaintCallback,
										application->videoBuffer);
			application->actionButton = button;

			/* Up button */
			button = p3t_buttonNew (x, 74, 44, 44, P3T_PIXMAP_BUTTON_UP);
			p3t_widgetSetActivateCallback (P3T_WIDGET (button),
			                               &upButtonActivateCallback,
			                               p3t_timerWidgetGetTimer (self));
			p3t_widgetSetPaintCallback (P3T_WIDGET (button),
										&buttonPaintCallback,
										application->videoBuffer);
			application->upButton = button;

			/* Down button */
			button = p3t_buttonNew (x, 131, 44, 44, P3T_PIXMAP_BUTTON_DOWN);
			p3t_widgetSetActivateCallback (P3T_WIDGET (button),
			                               &downButtonActivateCallback,
			                               p3t_timerWidgetGetTimer (self));
			p3t_widgetSetPaintCallback (P3T_WIDGET (button),
										&buttonPaintCallback,
										application->videoBuffer);
			application->downButton = button;

			application->state = APPLICATION_STATE_ONE;
			application->active = self;
			break;

		case APPLICATION_STATE_ONE:

			for (i = 0; i < TIMERS_NUMBER; i++) {

				temp = application->widgets[i];
				p3t_boxRestorePosition (P3T_BOX (temp));
			}

			if (application->actionButton != NULL) {
				p3t_buttonDestroy (application->actionButton);
				application->actionButton = NULL;
			}
			if (application->upButton != NULL) {
				p3t_buttonDestroy (application->upButton);
				application->upButton = NULL;
			}
			if (application->downButton != NULL) {
				p3t_buttonDestroy (application->downButton);
				application->downButton = NULL;
			}

			application->state = APPLICATION_STATE_ALL;
			application->active = NULL;
			break;
	}
}

static void
timerWidgetPaintCallback (p3t_widget  *widget,
                          void        *data)
{
	p3t_timerWidget *self;
	p3t_timer *timer;
	p3t_pixmap *pixmap;
	p3t_box *box;
	u16 *videoBuffer;
	char *remaining;

	self = P3T_TIMERWIDGET (widget);
	videoBuffer = (u16*) data;
	timer = p3t_timerWidgetGetTimer (self);
	remaining = p3t_timerGetRemainingTime (timer);

	/* Outline */
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_OUTLINE,
	                        P3T_PIXMAP_OUTLINE_TIMERWIDGET);
	p3t_pixmapDraw (pixmap, P3T_BOX (self), videoBuffer);

	/* Timer number */
	box = p3t_boxNew (5, 5, 18, 22);
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_NUMBER,
	                        p3t_timerGetNumber (timer) - 1);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap, box, videoBuffer);
	p3t_boxDestroy (box);

	/* Last digit */
	box = p3t_boxNew (100, 5, 18, 34);
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_DIGIT,
	                        remaining[4] - 48);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap, box, videoBuffer);
	p3t_boxDestroy (box);

	/* Third digit */
	box = p3t_boxNew (81, 5 ,18, 34);
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_DIGIT,
	                        remaining[3] - 48);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap, box, videoBuffer);
	p3t_boxDestroy (box);

	/* Second digit */
	box = p3t_boxNew (55, 5 ,18, 34);
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_DIGIT,
	                        remaining[1] - 48);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap, box, videoBuffer);
	p3t_boxDestroy (box);

	/* First digit */
	box = p3t_boxNew (36, 5 ,18, 34);
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_DIGIT,
	                        remaining[0] - 48);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap, box, videoBuffer);
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

	videoSetMode (MODE_5_2D |
	              DISPLAY_BG3_ACTIVE);

	BACKGROUND.control[3] = BG_BMP16_256x256 | BG_BMP_BASE (0);

	BACKGROUND.bg3_rotation.xdy = 0;
	BACKGROUND.bg3_rotation.xdx = 1 << 8;
	BACKGROUND.bg3_rotation.ydy = 1 << 8;
	BACKGROUND.bg3_rotation.ydx = 0;

	self->videoBuffer = (u16*) BG_BMP_RAM (0);

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
		                            &timerWidgetPaintCallback,
		                            self->videoBuffer);
		p3t_widgetSetActivateCallback (P3T_WIDGET (self->widgets[i]),
		                               &timerWidgetActivateCallback,
		                               self);
	}

	self->active = NULL;
	self->actionButton = NULL;
	self->upButton = NULL;
	self->downButton = NULL;

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

static void
paint (p3t_application *self)
{
	p3t_pixmap *background;
	p3t_box *box;
	int i;

	/* Paint background image */
	box = p3t_boxNew (0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	background = p3t_pixmapGet (P3T_PIXMAP_TYPE_BACKGROUND,
	                            P3T_PIXMAP_BACKGROUND_DEFAULT);
	p3t_pixmapDraw (background, box, self->videoBuffer);

	if (self->state == APPLICATION_STATE_ONE) {

		/* Paint the active timerWidget */
		p3t_widgetPaint (P3T_WIDGET (self->active));

		p3t_widgetPaint (P3T_WIDGET (self->actionButton));
		p3t_widgetPaint (P3T_WIDGET (self->upButton));
		p3t_widgetPaint (P3T_WIDGET (self->downButton));
	}
	else {

		for (i = 0; i < TIMERS_NUMBER; i++) {

			/* Paint all the timerWidgets */
			p3t_widgetPaint (P3T_WIDGET (self->widgets[i]));
		}
	}
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

		/* Try to activate the action buttons */
		if (self->state == APPLICATION_STATE_ONE) {
			p3t_widgetTryActivate (P3T_WIDGET (self->actionButton),
			                       stylus);
			p3t_widgetTryActivate (P3T_WIDGET (self->upButton),
			                       stylus);
			p3t_widgetTryActivate (P3T_WIDGET (self->downButton),
			                       stylus);
		}

		p3t_pointDestroy (stylus);
	}

	paint (self);
}
