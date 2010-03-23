#include <nds.h>
#include <stdio.h>

#include <p3t_clock.h>
#include <p3t_timer.h>
#include <p3t_application.h>

#include <bgEight.h>
#include <bgOne.h>

enum mode_t {
	MODE_EIGHT = 0,
	MODE_ONE   = 1
};

volatile u16* videoBuffer;
volatile mode_t mode;

void
initScreen (void)
{
	powerOn (POWER_ALL_2D);

	vramSetBankA (VRAM_A_MAIN_BG_0x06000000);
	vramSetBankB (VRAM_B_MAIN_BG_0x06020000);

	videoSetMode (MODE_5_2D |
	              DISPLAY_BG3_ACTIVE);

	BACKGROUND.control[3] = BG_BMP16_256x256 | BG_BMP_BASE (0);

	BACKGROUND.bg3_rotation.xdy = 0;
	BACKGROUND.bg3_rotation.xdx = 1 << 8;
	BACKGROUND.bg3_rotation.ydy = 1 << 8;
	BACKGROUND.bg3_rotation.ydx = 0;

	videoBuffer = (u16*) BG_BMP_RAM (0);

	dmaCopy (bgEightBitmap,
			 (void *) videoBuffer,
			 bgEightBitmapLen);

	consoleDemoInit ();

	lcdMainOnBottom ();
}

int
main (void)
{
	p3t_application *application;
	int keys;

	p3t_clockInit ();
	initScreen ();

	application = p3t_applicationNew ();

	while (1) {

		scanKeys ();
		keys = keysDown ();

		p3t_applicationUpdate (application, keys);

		swiWaitForVBlank ();
	}

	return 0;
}
