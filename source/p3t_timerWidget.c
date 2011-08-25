/* P3T - Coundown timers for Nintendo DS
 * Copyright (C) 2010-2011  Andrea Bolognani <eof@kiyuko.org>
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

#include <p3t_timerWidget.h>
#include <p3t_pixmap.h>

#include <stdlib.h>

struct _p3t_timerWidgetPrivate {
	p3t_timer  *timer;
};

static void
paintCallback (p3t_widget  *widget,
               void        *data)
{
	p3t_timerWidget *self;
	p3t_timer *timer;
	p3t_pixmap *pixmap;
	p3t_box *box;
	char *remaining;

	self = P3T_TIMERWIDGET (widget);
	timer = p3t_timerWidgetGetTimer (self);
	remaining = p3t_timerGetRemainingTime (timer);

	/* Outline */
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_OUTLINE,
	                        P3T_PIXMAP_OUTLINE_TIMERWIDGET);
	p3t_pixmapDraw (pixmap, P3T_BOX (self));

	/* Timer number */
	box = p3t_boxNew (3, 3, 22, 26);
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_NUMBER,
	                        p3t_timerGetNumber (timer) - 1);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap, box);
	p3t_boxDestroy (box);

	/* Last digit */
	box = p3t_boxNew (98, 5, 18, 34);
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_DIGIT,
	                        remaining[4] - 48);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap, box);
	p3t_boxDestroy (box);

	/* Third digit */
	box = p3t_boxNew (78, 5 ,18, 34);
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_DIGIT,
	                        remaining[3] - 48);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap, box);
	p3t_boxDestroy (box);

	/* Second digit */
	box = p3t_boxNew (50, 5 ,18, 34);
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_DIGIT,
	                        remaining[1] - 48);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap, box);
	p3t_boxDestroy (box);

	/* First digit */
	box = p3t_boxNew (30, 5 ,18, 34);
	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_DIGIT,
	                        remaining[0] - 48);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap, box);
	p3t_boxDestroy (box);

	free (remaining);
}

void
_p3t_timerWidgetInit (p3t_timerWidget  *self,
                     int               x,
                     int               y,
                     int               width,
                     int               height)
{
	p3t_timerWidgetPrivate *priv;

	_p3t_widgetInit (P3T_WIDGET (self), x, y, width, height);

	priv = malloc (sizeof (p3t_timerWidgetPrivate));

	priv->timer = NULL;

	self->priv = priv;

	p3t_widgetSetPaintCallback (P3T_WIDGET (self),
	                            &paintCallback,
	                            NULL);
}

void
_p3t_timerWidgetFinalize (p3t_timerWidget *self)
{
	if (self->priv->timer != NULL) {
		p3t_timerDestroy (self->priv->timer);
	}
	free (self->priv);

	_p3t_widgetFinalize (P3T_WIDGET (self));
}

p3t_timerWidget*
p3t_timerWidgetNew (int  x,
                    int  y,
                    int  width,
                    int  height)
{
	p3t_timerWidget *self;

	self = (p3t_timerWidget*) malloc (sizeof (p3t_timerWidget));
	_p3t_timerWidgetInit (self, x, y, width, height);

	return self;
}

void
p3t_timerWidgetDestroy (p3t_timerWidget *self)
{
	_p3t_timerWidgetFinalize (self);
	free (self);
}

void
p3t_timerWidgetSetTimer (p3t_timerWidget  *self,
                         p3t_timer        *timer)
{
	if (self->priv->timer != NULL) {
		p3t_timerDestroy (self->priv->timer);
	}

	self->priv->timer = timer;
}

p3t_timer*
p3t_timerWidgetGetTimer (p3t_timerWidget *self)
{
	return self->priv->timer;
}
