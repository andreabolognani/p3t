#include <nds.h>

#include <p3t_clock.h>
#include <p3t_application.h>
#include <p3t_point.h>

int
main (void)
{
	p3t_application *application;
	p3t_point *stylus;
	touchPosition touch;
	int keys;

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

		p3t_widgetTryActivate (P3T_WIDGET (application), stylus);
		p3t_widgetPaint (P3T_WIDGET (application));

		p3t_pointDestroy (stylus);

		swiWaitForVBlank ();
	}

	return 0;
}
