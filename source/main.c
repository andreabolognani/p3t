#include <nds.h>
#include <stdio.h>

#include <p3t_clock.h>
#include <p3t_timer.h>

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
	p3t_timer timer;
	int keys;

	p3t_clockInit ();
	initScreen ();

	p3t_timerInit (&timer, 1);
	p3t_timerStart (&timer);
	printf ("[%d] Timer %d started\n",
	        p3t_clockGetSeconds (),
	        p3t_timerGetNumber (&timer));

	while (1) {

		scanKeys ();
		keys = keysDown ();

		if (keys & KEY_TOUCH) {

			p3t_timerPause (&timer);

			printf ("[%d] Timer %d paused, %d seconds elapsed\n",
			        p3t_clockGetSeconds (),
			        p3t_timerGetNumber (&timer),
			        p3t_timerGetElapsed (&timer));

			mode = !mode;
		}

		swiWaitForVBlank ();

		switch (mode) {
			case MODE_EIGHT:
				dmaCopy (bgEightBitmap,
						 (void *) videoBuffer,
						 bgEightBitmapLen);
				break;
			case MODE_ONE:
				dmaCopy (bgOneBitmap,
						 (void *) videoBuffer,
						 bgOneBitmapLen);
				break;
		}
	}

	return 0;
}
