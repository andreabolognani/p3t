#include <nds.h>

#include <p3t_clock.h>
#include <p3t_application.h>

#define DEVELOPMENT_BUILD

int
main (void)
{
	p3t_application *application;
	int keys;

	p3t_clockInit ();

	application = p3t_applicationNew ();

	while (1) {

		scanKeys ();
		keys = keysDown ();

		p3t_applicationUpdate (application, keys);

		swiWaitForVBlank ();
	}

	return 0;
}
