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

#include <p3t_point.h>

#include <stdlib.h>

struct _p3t_point {
	int  x;
	int  y;
};

static void
init (p3t_point  *self,
      int         x,
      int         y)
{
	self->x = x;
	self->y = y;
}

static void
finalize (p3t_point *self)
{
	/* Nothing to do */
}

p3t_point*
p3t_pointNew (int  x,
              int  y)
{
	p3t_point *self;

	self = (p3t_point*) malloc (sizeof (p3t_point));
	init (self, x, y);

	return self;
}

void
p3t_pointDestroy (p3t_point *self)
{
	finalize (self);
	free (self);
}

int
p3t_pointGetX (p3t_point *self)
{
	return self->x;
}

int
p3t_pointGetY (p3t_point *self)
{
	return self->y;
}
