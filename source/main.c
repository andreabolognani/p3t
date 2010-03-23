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

	while (1) {

		scanKeys ();
		keys = keysDown ();

		if (keys & KEY_TOUCH) {

			switch (p3t_timerGetState (&timer)) {

				case P3T_TIMER_STATE_STOPPED:

					p3t_timerStart (&timer);
					printf ("[%d] Timer %d started\n",
							p3t_clockGetSeconds (),
							p3t_timerGetNumber (&timer));
					break;

				case P3T_TIMER_STATE_RUNNING:

					p3t_timerPause (&timer);
					printf ("[%d] Timer %d paused, %d seconds elapsed\n",
							p3t_clockGetSeconds (),
							p3t_timerGetNumber (&timer),
							p3t_timerGetElapsed (&timer));
					break;

				case P3T_TIMER_STATE_PAUSED:

					p3t_timerContinue (&timer);
					printf ("[%d] Timer %d restarted, %d seconds elapsed\n",
							p3t_clockGetSeconds (),
							p3t_timerGetNumber (&timer),
							p3t_timerGetElapsed (&timer));
					break;
			}
		}

		swiWaitForVBlank ();
	}

	return 0;
}
