#include <p3t_application.h>
#include <p3t_timer.h>
#include <p3t_point.h>
#include <p3t_box.h>
#include <p3t_widget.h>
#include <p3t_timerWidget.h>

#include <stdlib.h>
#include <nds.h>

#include <bgEight.h>
#include <digit0.h>

#define TIMERS_NUMBER      8
#define SECONDS_PER_MINUTE 60

struct _p3t_application {
	u16               *backgroundBuffer;
	u16               *widgetsBuffer;
	p3t_timer        **timers;
	p3t_timerWidget  **widgets;
};

static void
paintBitmapInsideBox (p3t_application  *self,
                      p3t_box          *box,
                      u16              *bitmap,
                      uint32            bitmapLen)
{
	u16 *video;
	int bitmapLineLen;
	int boxX;
	int boxY;
	int boxHeight;
	int y;

	boxX = p3t_boxGetX (box);
	boxY = p3t_boxGetY (box);
	boxHeight = p3t_boxGetHeight (box);

	video = p3t_applicationGetWidgetsBuffer (self);
	bitmapLineLen = bitmapLen / (boxHeight * 2);

	DC_FlushRange (bitmap, bitmapLen);

	for (y = 0; y < boxHeight; y++) {

		dmaCopy ((void*) &bitmap[y * bitmapLineLen],
		         (void*) &video[boxX + ((y + boxY) * SCREEN_WIDTH)],
		         bitmapLen / boxHeight);
	}
}

static void
paintCallback (p3t_widget  *widget,
               void        *data)
{
	p3t_timerWidget *self;
	p3t_application *application;
	p3t_timer *timer;
	p3t_box *box;
	char *remaining;

	self = P3T_TIMERWIDGET (widget);
	application = P3T_APPLICATION (data);
	timer = p3t_timerWidgetGetTimer (self);
	remaining = p3t_timerGetRemainingTime (timer);

	box = p3t_boxNew (100, 5, 18, 34);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));

	paintBitmapInsideBox (application,
	                      box,
	                      (u16*) digit0Bitmap,
	                      digit0BitmapLen);

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

	dmaCopy((void*) bgEightBitmap,
	        (void*) self->backgroundBuffer,
	        bgEightBitmapLen);

	lcdMainOnBottom ();

#ifdef DEVELOPMENT_BUILD
	consoleDemoInit ();
#endif

	/* Create all the needed timers */
	for (i = 0; i < TIMERS_NUMBER; i++) {
		self->timers[i] = p3t_timerNew (i + 1);
	}

	self->widgets[0] = p3t_timerWidgetNew (3, 3, 124, 45);
	p3t_timerWidgetSetTimer (self->widgets[0], self->timers[0]);
	p3t_widgetSetPaintCallback (P3T_WIDGET (self->widgets[0]),
											&paintCallback,
											self);

	/*
	                  p3t_boxNew (3, 3, 124, 45));
	                  p3t_boxNew (3, 50, 124, 45));
	                  p3t_boxNew (3, 97, 124, 45));
	                  p3t_boxNew (3, 144, 124, 45));
	                  p3t_boxNew (129, 3, 124, 45));
	                  p3t_boxNew (129, 50, 124, 45));
	                  p3t_boxNew (129, 97, 124, 45));
	                  p3t_boxNew (129, 144, 124, 45));
	*/
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
