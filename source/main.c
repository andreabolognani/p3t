#include <nds.h>
#include <stdio.h>

#include <bgEight.h>
#include <bgOne.h>

enum mode_t {
	MODE_EIGHT = 0,
	MODE_ONE   = 1
};

volatile int ticks;
volatile int seconds;
volatile u16* videoBuffer;
volatile mode_t mode;

void
tick (void)
{
	ticks++;

	if (ticks >= 60) {
		seconds++;
		ticks = 0;
	}
}

void
initClock (void)
{
	ticks = 0;
	seconds = 0;

	irqSet (IRQ_VBLANK, tick);
}

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
	touchPosition touch;
	int keys;

	initClock ();
	initScreen ();

	while (1) {

		scanKeys ();
		keys = keysDown ();

		if (keys & KEY_TOUCH) {

			touchRead (&touch);

			printf ("[%d] Touched %d %d\n",
			        seconds,
			        touch.px,
			        touch.py);

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
