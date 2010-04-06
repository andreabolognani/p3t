/* P3T - Simple timers for Nintendo DS
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

#include <p3t_widget.h>

#include <stdlib.h>
#include <nds.h>

struct _p3t_widgetPrivate {
	p3t_widgetActivateCallback   activateCallback;
	void                        *activateCallbackData;
	p3t_widgetPaintCallback      paintCallback;
	void                        *paintCallbackData;
};

void
_p3t_widgetInit (p3t_widget  *self,
                int          x,
                int          y,
                int          width,
                int          height)
{
	p3t_widgetPrivate *priv;

	_p3t_boxInit (P3T_BOX (self), x, y, width, height);

	priv = (p3t_widgetPrivate*) malloc (sizeof (p3t_widgetPrivate));

	priv->activateCallback = NULL;
	priv->activateCallbackData = NULL;
	priv->paintCallback = NULL;
	priv->paintCallbackData = NULL;

	self->priv = priv;
}

void
_p3t_widgetFinalize (p3t_widget *self)
{
	free (self->priv);

	_p3t_boxFinalize (P3T_BOX (self));
}

p3t_widget*
p3t_widgetNew (int  x,
               int  y,
               int  width,
               int  height)
{
	p3t_widget *self;

	self = (p3t_widget*) malloc (sizeof (p3t_widget));
	_p3t_widgetInit (self, x, y, width, height);

	return self;
}

void
p3t_widgetDestroy (p3t_widget *self)
{
	_p3t_widgetFinalize (self);
	free (self);
}

void
p3t_widgetSetActivateCallback (p3t_widget                  *self,
                               p3t_widgetActivateCallback   callback,
                               void                        *data)
{
	self->priv->activateCallback = callback;
	self->priv->activateCallbackData = data;
}

void
p3t_widgetSetPaintCallback (p3t_widget               *self,
                            p3t_widgetPaintCallback   callback,
                            void                     *data)
{
	self->priv->paintCallback = callback;
	self->priv->paintCallbackData = data;
}

void
p3t_widgetTryActivate (p3t_widget  *self,
                       p3t_point   *point)
{
	p3t_widgetActivateCallback activateCallback;

	if (p3t_boxContainsPoint (P3T_BOX (self), point)) {

		if (self->priv->activateCallback != NULL) {

			activateCallback = *(self->priv->activateCallback);
			activateCallback (self, point, self->priv->activateCallbackData);
		}
	}
}

void
p3t_widgetPaint (p3t_widget *self)
{
	p3t_widgetPaintCallback paintCallback;
	int x;
	int y;

	x = p3t_boxGetX (P3T_BOX (self));
	y = p3t_boxGetY (P3T_BOX (self));

	/* Don't waste time painting an off-screen widget */
	if (x < 0 || x >= SCREEN_WIDTH) {
		return;
	}
	if (y < 0 || y >= SCREEN_HEIGHT) {
		return;
	}

	if (self->priv->paintCallback != NULL) {

		paintCallback = *(self->priv->paintCallback);
		paintCallback (self, self->priv->paintCallbackData);
	}
}
