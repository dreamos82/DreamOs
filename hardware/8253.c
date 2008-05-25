#include <io.h>
#include <stdio.h>
#include <8253.h>

static unsigned int ticks;
static unsigned int seconds;

void PIT_handler ()
{
    if (++ticks % 100 == 0) {
	ticks = 0;
	if (++seconds > 86400)
	  seconds = 0;
    }
}

void configure_PIT ()
{
    int divisor = 65535;

    asm ("cli");
    ticks = seconds = 0;
    outportb (0x36, PIT_COMREG);
    outportb (divisor & 0xFF, PIT_DATAREG0);
    outportb (divisor >> 8, PIT_DATAREG0);
    asm ("sti");

}

unsigned int sleep (unsigned int secs)
{
    int p = seconds + secs;

    while (ticks != 0);
    while (seconds < p);
    return 0;
}
