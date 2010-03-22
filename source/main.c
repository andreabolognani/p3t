#include <nds.h>
#include <stdio.h>

#include <background.h>

volatile int ticks;
volatile int seconds;
volatile u16* video_buffer;

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

	video_buffer = (u16*) BG_BMP_RAM (0);

	consoleDemoInit ();

	lcdMainOnBottom ();
}

int
main (void)
{
	touchPosition touch;
	u16 color;
	int held;
	int redraw;
	int i;

	initClock ();
	initScreen ();

	while (1) {

		redraw = 0;

		scanKeys ();
		held = keysDown ();

		if (held & KEY_TOUCH) {

			touchRead (&touch);

			printf ("[%d] Touched %d %d\n",
			        seconds,
			        touch.px,
			        touch.py);

			redraw = 1;
		}

		swiWaitForVBlank ();

		if (redraw) {
			dmaCopy (backgroundBitmap,
			         (void *) video_buffer,
			         backgroundBitmapLen);

			color = RGB15 (31, 0, 0) | BIT (15);

			i = (SCREEN_WIDTH * touch.py) + touch.px;
			video_buffer[i] = color;
		}
	}

	return 0;
}
