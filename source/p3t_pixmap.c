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

#include <p3t_pixmap.h>

#include <stdlib.h>
#include <nds.h>

#include <digit0.h>
#include <digit1.h>
#include <digit2.h>
#include <digit3.h>
#include <digit4.h>
#include <digit5.h>
#include <digit6.h>
#include <digit7.h>
#include <digit8.h>
#include <digit9.h>

#include <number1.h>
#include <number2.h>
#include <number3.h>
#include <number4.h>
#include <number5.h>
#include <number6.h>
#include <number7.h>
#include <number8.h>

#include <buttonUp.h>
#include <buttonDown.h>
#include <buttonAction.h>

#include <outlineTimerWidget.h>
#include <outlineButton.h>

#include <backgroundDefault.h>

#define TYPES_NUMBER 5

#define DIGITS_NUMBER      10
#define NUMBERS_NUMBER     8
#define BUTTONS_NUMBER     3
#define OUTLINES_NUMBER    2
#define BACKGROUNDS_NUMBER 1

#define DIGIT_WIDTH  18
#define DIGIT_HEIGHT 34

#define NUMBER_WIDTH  22
#define NUMBER_HEIGHT 26

#define BUTTON_WIDTH  40
#define BUTTON_HEIGHT 40

#define BACKGROUND_WIDTH  256
#define BACKGROUND_HEIGHT 192

struct _p3t_pixmapPrivate {
	p3t_pixmapType   type;
	u16             *data;
};

static p3t_pixmap  ***pixmaps;

static int ready = 0;

/* Mapping functions */
static void  digitInfo       (int    identifier,
                              u16  **data,
                              int   *width,
                              int   *height);
static void  numberInfo      (int    identifier,
                              u16  **data,
                              int   *width,
                              int   *height);
static void  buttonInfo      (int    identifier,
                              u16  **data,
                              int   *width,
                              int   *height);
static void  outlineInfo     (int    identifier,
                              u16  **data,
                              int   *width,
                              int   *height);
static void  backgroundInfo  (int    identifier,
                              u16  **data,
                              int   *width,
                              int   *height);

static void
prepare (void)
{
	int i;

	pixmaps = (p3t_pixmap***) malloc (TYPES_NUMBER * sizeof (p3t_pixmap**));

	/* Digits */
	pixmaps[P3T_PIXMAP_TYPE_DIGIT] = (p3t_pixmap**) malloc (DIGITS_NUMBER * sizeof (p3t_pixmap*));

	for (i = 0; i < DIGITS_NUMBER; i++) {
		pixmaps[P3T_PIXMAP_TYPE_DIGIT][i] = NULL;
	}

	/* Numbers */
	pixmaps[P3T_PIXMAP_TYPE_NUMBER] = (p3t_pixmap**) malloc (NUMBERS_NUMBER * sizeof (p3t_pixmap*));

	for (i = 0; i < NUMBERS_NUMBER; i++) {
		pixmaps[P3T_PIXMAP_TYPE_NUMBER][i] = NULL;
	}

	/* Buttons */
	pixmaps[P3T_PIXMAP_TYPE_BUTTON] = (p3t_pixmap**) malloc (BUTTONS_NUMBER * sizeof (p3t_pixmap*));

	for (i = 0; i < BUTTONS_NUMBER; i++) {
		pixmaps[P3T_PIXMAP_TYPE_BUTTON][i] = NULL;
	}

	/* Outlines */
	pixmaps[P3T_PIXMAP_TYPE_OUTLINE] = (p3t_pixmap**) malloc (OUTLINES_NUMBER * sizeof (p3t_pixmap*));

	for (i = 0; i < OUTLINES_NUMBER; i++) {
		pixmaps[P3T_PIXMAP_TYPE_OUTLINE][i] = NULL;
	}

	/* Backgrounds */
	pixmaps[P3T_PIXMAP_TYPE_BACKGROUND] = (p3t_pixmap**) malloc (BACKGROUNDS_NUMBER * sizeof (p3t_pixmap*));

	for (i = 0; i < BACKGROUNDS_NUMBER; i++) {
		pixmaps[P3T_PIXMAP_TYPE_BACKGROUND][i] = NULL;
	}
}

static void
_p3t_pixmapInit (p3t_pixmap      *self,
                 p3t_pixmapType   type,
                 int              identifier)
{
	p3t_pixmapPrivate *priv;
	u16 *data;
	int width;
	int height;

	data = NULL;
	width = 0;
	height = 0;

	/* Boring switch stuff */
	switch (type) {

		case P3T_PIXMAP_TYPE_DIGIT:

			digitInfo (identifier, &data, &width, &height);
			break;

		case P3T_PIXMAP_TYPE_NUMBER:

			numberInfo (identifier, &data, &width, &height);
			break;

		case P3T_PIXMAP_TYPE_BUTTON:

			buttonInfo (identifier, &data, &width, &height);
			break;

		case P3T_PIXMAP_TYPE_OUTLINE:

			outlineInfo (identifier, &data, &width, &height);
			break;

		case P3T_PIXMAP_TYPE_BACKGROUND:

			backgroundInfo (identifier, &data, &width, &height);
			break;
	}

	_p3t_boxInit (P3T_BOX (self), 0, 0, width, height);

	priv = (p3t_pixmapPrivate*) malloc (sizeof (p3t_pixmapPrivate));

	priv->data = data;

	self->priv = priv;

	/* Cache the results for faster lookup */
	pixmaps[type][identifier] = self;
}

p3t_pixmap*
p3t_pixmapGet (p3t_pixmapType  type,
               int             identifier)
{
	p3t_pixmap *self;

	if (!ready) {
		prepare ();
		ready = 1;
	}

	self = pixmaps[type][identifier];

	if (self == NULL) {

		self = (p3t_pixmap*) malloc (sizeof (p3t_pixmap));
		_p3t_pixmapInit (self, type, identifier);
	}

	return self;
}

void
p3t_pixmapDraw (p3t_pixmap  *self,
                p3t_box     *box)
{
	int width;
	int height;
	int boxX;
	int boxY;
	int y;

	boxX = p3t_boxGetX (box);
	boxY = p3t_boxGetY (box);

	width = p3t_boxGetWidth (P3T_BOX (self));
	height = p3t_boxGetHeight (P3T_BOX (self));

	DC_FlushRange (self->priv->data, width * height);

	for (y = 0; y < height; y++) {

		dmaCopy ((void*) &(self->priv->data[y * width]),
		         (void*) &VRAM_A[boxX + ((y + boxY) * SCREEN_WIDTH)],
		         width * 2);
	}
}

static void
digitInfo (int    identifier,
           u16  **data,
           int   *width,
           int   *height)
{
	*width = DIGIT_WIDTH;
	*height = DIGIT_HEIGHT;

	switch (identifier) {

		case P3T_PIXMAP_DIGIT_0:
			*data = (u16*) digit0Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_1:
			*data = (u16*) digit1Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_2:
			*data = (u16*) digit2Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_3:
			*data = (u16*) digit3Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_4:
			*data = (u16*) digit4Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_5:
			*data = (u16*) digit5Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_6:
			*data = (u16*) digit6Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_7:
			*data = (u16*) digit7Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_8:
			*data = (u16*) digit8Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_9:
			*data = (u16*) digit9Bitmap;
			break;
	}
}

static void
numberInfo (int     identifier,
            u16   **data,
            int    *width,
            int    *height)
{
	*width = NUMBER_WIDTH;
	*height = NUMBER_HEIGHT;

	switch (identifier) {

		case P3T_PIXMAP_NUMBER_1:
			*data = (u16*) number1Bitmap;
			break;

		case P3T_PIXMAP_NUMBER_2:
			*data = (u16*) number2Bitmap;
			break;

		case P3T_PIXMAP_NUMBER_3:
			*data = (u16*) number3Bitmap;
			break;

		case P3T_PIXMAP_NUMBER_4:
			*data = (u16*) number4Bitmap;
			break;

		case P3T_PIXMAP_NUMBER_5:
			*data = (u16*) number5Bitmap;
			break;

		case P3T_PIXMAP_NUMBER_6:
			*data = (u16*) number6Bitmap;
			break;

		case P3T_PIXMAP_NUMBER_7:
			*data = (u16*) number7Bitmap;
			break;

		case P3T_PIXMAP_NUMBER_8:
			*data = (u16*) number8Bitmap;
			break;
	}
}

static void
buttonInfo (int    identifier,
            u16  **data,
            int   *width,
            int   *height)
{
	*width = BUTTON_WIDTH;
	*height = BUTTON_HEIGHT;

	switch (identifier) {

		case P3T_PIXMAP_BUTTON_UP:
			*data = (u16*) buttonUpBitmap;
			break;

		case P3T_PIXMAP_BUTTON_DOWN:
			*data = (u16*) buttonDownBitmap;
			break;

		case P3T_PIXMAP_BUTTON_ACTION:
			*data = (u16*) buttonActionBitmap;
			break;
	}
}

static void
outlineInfo (int    identifier,
             u16  **data,
             int   *width,
             int   *height)
{
	switch (identifier) {

		case P3T_PIXMAP_OUTLINE_TIMERWIDGET:

			*width = 122;
			*height = 44;
			*data = (u16*) outlineTimerWidgetBitmap;
			break;

		case P3T_PIXMAP_OUTLINE_BUTTON:

			*width = 44;
			*height = 44;
			*data = (u16*) outlineButtonBitmap;
			break;
	}
}

static void
backgroundInfo (int     identifier,
                u16   **data,
                int    *width,
                int    *height)
{
	*width = BACKGROUND_WIDTH;
	*height = BACKGROUND_HEIGHT;

	switch (identifier) {

		case P3T_PIXMAP_BACKGROUND_DEFAULT:
			*data = (u16*) backgroundDefaultBitmap;
			break;
	}
}
