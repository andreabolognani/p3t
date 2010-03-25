#include <nds.h>
#include <stdio.h>

#include <p3t_clock.h>
#include <p3t_timer.h>
#include <p3t_application.h>

#include <bgEight.h>

volatile u16* backgroundBuffer;
volatile u16* widgetsBuffer;
volatile mode_t mode;

void
initScreen (void)
{
	powerOn (POWER_ALL_2D);

	vramSetBankA (VRAM_A_MAIN_BG_0x06000000);
	vramSetBankB (VRAM_B_MAIN_BG_0x06020000);

	videoSetMode (MODE_5_2D |
	              DISPLAY_BG2_ACTIVE |
	              DISPLAY_BG3_ACTIVE);

	BACKGROUND.control[3] = BG_BMP16_256x256 | BG_BMP_BASE (0);
	BACKGROUND.control[2] = BG_BMP16_256x256 | BG_BMP_BASE (8);

	BACKGROUND.bg3_rotation.xdy = 0;
	BACKGROUND.bg3_rotation.xdx = 1 << 8;
	BACKGROUND.bg3_rotation.ydy = 1 << 8;
	BACKGROUND.bg3_rotation.ydx = 0;
	BACKGROUND.bg2_rotation.xdy = 0;
	BACKGROUND.bg2_rotation.xdx = 1 << 8;
	BACKGROUND.bg2_rotation.ydy = 1 << 8;
	BACKGROUND.bg2_rotation.ydx = 0;

	backgroundBuffer = (u16*) BG_BMP_RAM (0);
	widgetsBuffer = (u16*) BG_BMP_RAM (8);

	dmaCopy (bgEightBitmap,
			 (void *) backgroundBuffer,
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
