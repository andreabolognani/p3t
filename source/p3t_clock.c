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
