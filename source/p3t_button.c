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

#include <p3t_button.h>
#include <p3t_pixmap.h>

#include <stdlib.h>

struct _p3t_buttonPrivate {
	p3t_pixmapButtonType  buttonType;
};

static void
paintCallback (p3t_widget  *widget,
               void        *data)
{
	p3t_button *self;
	p3t_pixmap *pixmap;
	p3t_box *box;

	self = P3T_BUTTON (widget);

	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_OUTLINE,
	                        P3T_PIXMAP_OUTLINE_BUTTON);
	p3t_pixmapDraw (pixmap, P3T_BOX (self));

	pixmap = p3t_pixmapGet (P3T_PIXMAP_TYPE_BUTTON,
	                        p3t_buttonGetType (self));
	box = p3t_boxNew (2, 2, 40, 40);
	p3t_boxMakeAbsolute (box, P3T_BOX (self));
	p3t_pixmapDraw (pixmap, box);
	p3t_boxDestroy (box);
}

void
_p3t_buttonInit (p3t_button            *self,
                 int                    x,
                 int                    y,
                 int                    width,
                 int                    height,
                 p3t_pixmapButtonType   buttonType)
{
	p3t_buttonPrivate *priv;

	_p3t_widgetInit (P3T_WIDGET (self), x, y, width, height);

	priv = (p3t_buttonPrivate*) malloc (sizeof (p3t_buttonPrivate));

	priv->buttonType = buttonType;

	self->priv = priv;

	p3t_widgetSetPaintCallback (P3T_WIDGET (self),
	                            &paintCallback,
	                            NULL);
}

void
_p3t_buttonFinalize (p3t_button *self)
{
	free (self->priv);

	_p3t_widgetFinalize (P3T_WIDGET (self));
}

p3t_button*
p3t_buttonNew (int                   x,
               int                   y,
               int                   width,
               int                   height,
               p3t_pixmapButtonType  buttonType)
{
	p3t_button *self;

	self = (p3t_button*) malloc (sizeof (p3t_button));
	_p3t_buttonInit (self, x, y, width, height, buttonType);

	return self;
}

void
p3t_buttonDestroy (p3t_button *self)
{
	_p3t_buttonFinalize (self);
	free (self);
}

p3t_pixmapButtonType
p3t_buttonGetType (p3t_button *self)
{
	return self->priv->buttonType;
}
