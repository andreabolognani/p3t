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

#include <bgEight.h>

#define TYPES_NUMBER 2

#define DIGITS_NUMBER      10
#define BACKGROUNDS_NUMBER 1

#define DIGIT_WIDTH  18
#define DIGIT_HEIGHT 34

#define BACKGROUND_WIDTH  256
#define BACKGROUND_HEIGHT 192

struct _p3t_pixmapPrivate {
	p3t_pixmapType   type;
	u16             *data;
};

static int           *widths;
static int           *heights;
static p3t_pixmap  ***pixmaps;

static int ready = 0;

static u16* digitFromIdentifier      (int identifier);
static u16* backgroundFromIdentifier (int identifier);

static void
prepare (void)
{
	int i;

	widths = (int*) malloc (TYPES_NUMBER * sizeof (int));
	heights = (int*) malloc (TYPES_NUMBER * sizeof (int));
	pixmaps = (p3t_pixmap***) malloc (TYPES_NUMBER * sizeof (p3t_pixmap**));

	widths[P3T_PIXMAP_TYPE_DIGIT] = DIGIT_WIDTH;
	heights[P3T_PIXMAP_TYPE_DIGIT] = DIGIT_HEIGHT;
	pixmaps[P3T_PIXMAP_TYPE_DIGIT] = (p3t_pixmap**) malloc (DIGITS_NUMBER * sizeof (p3t_pixmap*));

	for (i = 0; i < DIGITS_NUMBER; i++) {
		pixmaps[P3T_PIXMAP_TYPE_DIGIT][i] = NULL;
	}

	widths[P3T_PIXMAP_TYPE_BACKGROUND] = BACKGROUND_WIDTH;
	heights[P3T_PIXMAP_TYPE_BACKGROUND] = BACKGROUND_HEIGHT;
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

	_p3t_boxInit (P3T_BOX (self), 0, 0, widths[type], heights[type]);

	priv = (p3t_pixmapPrivate*) malloc (sizeof (p3t_pixmapPrivate));

	/* Boring switch stuff */
	switch (type) {

		case P3T_PIXMAP_TYPE_DIGIT:

			priv->data = digitFromIdentifier (identifier);
			break;

		case P3T_PIXMAP_TYPE_BACKGROUND:

			priv->data = backgroundFromIdentifier (identifier);
			break;
	}

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
                p3t_box     *box,
                u16         *video)
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
		         (void*) &video[boxX + ((y + boxY) * SCREEN_WIDTH)],
		         width * 2);
	}
}

static u16*
digitFromIdentifier (int identifier)
{
	u16 *data;

	switch (identifier) {

		case P3T_PIXMAP_DIGIT_0:
			data = (u16*) digit0Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_1:
			data = (u16*) digit1Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_2:
			data = (u16*) digit2Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_3:
			data = (u16*) digit3Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_4:
			data = (u16*) digit4Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_5:
			data = (u16*) digit5Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_6:
			data = (u16*) digit6Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_7:
			data = (u16*) digit7Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_8:
			data = (u16*) digit8Bitmap;
			break;

		case P3T_PIXMAP_DIGIT_9:
			data = (u16*) digit9Bitmap;
			break;
	}

	return data;
}

static u16*
backgroundFromIdentifier (int identifier)
{
	u16 *data;

	switch (identifier) {

		case P3T_PIXMAP_BACKGROUND_EIGHT:
			data = (u16*) bgEightBitmap;
			break;
	}

	return data;
}
