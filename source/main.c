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

#include <nds.h>
#include <maxmod9.h>

#include <p3t_clock.h>
#include <p3t_application.h>
#include <p3t_point.h>

#include <soundbank.h>
#include <soundbank_bin.h>

int
main (void)
{
	p3t_application *application;
	p3t_point *stylus;
	touchPosition touch;
	int keys;

	powerOn (POWER_ALL_2D | PM_SOUND_AMP | PM_BACKLIGHT_BOTTOM);

	videoSetMode (MODE_FB0);
	vramSetBankA (VRAM_A_LCD);

#ifdef DEVELOPMENT_BUILD
	consoleDemoInit ();
#endif

	lcdMainOnBottom ();

	mmInitDefaultMem ((mm_addr) soundbank_bin);

	p3t_clockInit ();
	application = p3t_applicationNew ();

	while (1) {

		stylus = NULL;

		scanKeys ();
		keys = keysDown ();

		if (keys & KEY_TOUCH) {

			touchRead (&touch);
			stylus = p3t_pointNew (touch.px, touch.py);
		}

		p3t_applicationUpdate (application);
		p3t_widgetTryActivate (P3T_WIDGET (application), stylus);
		p3t_widgetPaint (P3T_WIDGET (application));

		p3t_pointDestroy (stylus);

		swiWaitForVBlank ();
	}

	return 0;
}
