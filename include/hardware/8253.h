#ifndef _8253_H
#define _8253_H

#define PIT_COMREG 0x43
#define PIT_DATAREG0 0x40

void PIT_handler ();
void configure_PIT ();
unsigned int sleep (unsigned int);

#endif
