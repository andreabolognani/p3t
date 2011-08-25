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

#include <p3t_clock.h>

#include <nds.h>

#define TICKS_PER_SECOND 60

static int ticks;
static int seconds;

static void
tick (void)
{
	ticks++;

	if (ticks >= TICKS_PER_SECOND) {
		seconds++;
		ticks = 0;
	}
}

void
p3t_clockInit (void)
{
	ticks = 0;
	seconds = 0;

	irqSet (IRQ_VBLANK, tick);
}

int
p3t_clockGetSeconds (void)
{
	return seconds;
}
