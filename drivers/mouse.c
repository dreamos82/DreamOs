/* 
 * Driver for *PS2* Mouses.
 * 	greez to Osiris
 *	  & DT (Contributor)
 */

#include <mouse.h>
#include <video.h>
#include <io.h>
#include <handlers.h>
#include <pic8259.h>
#include <stdio.h>

void mouse_init()
{
	printf("Inizializzo il driver del mouse..");
	mouse_waitcmd(1);
	outportb(0x64,0xA8);
	mouse_waitcmd(1);
	outportb(0x64,0x20);
	unsigned char status_byte;
	mouse_waitcmd(0);
	status_byte = (inportb(0x60) | 2);
	mouse_waitcmd(1);
	outportb(0x64, 0x60);
	mouse_waitcmd(1);
	outportb(0x60, status_byte);
	mouse_write (0xF6);
	mouse_read();
	mouse_write (0xF4);
	mouse_read();
	add_IRQ_handler(12, mouse_IRQhandler);
	enable_IRQ(12);
}

void mouse_waitcmd(unsigned char type)
{
	register unsigned int _time_out=100000;
	if(type==0)
	{
		while(_time_out--) // dati
		{
			if((inportb(0x64) & 1)==1)
			{
				return;
			}
		}
		return;
	}
	else
	{
		while(_time_out--) // e segnali
		{
			if((inportb(0x64) & 2)==0)
			{
				return;
			}
		}
		return;
	}
}

void mouse_write (unsigned char a_write)
{
	mouse_waitcmd(1);
	outportb(0x64, 0xD4);
	mouse_waitcmd(1);
	outportb(0x60, a_write);
}

unsigned char mouse_read()
{
	mouse_waitcmd(0);
	return inportb(0x60);
}

void mouse_IRQhandler(struct regs *a_r)
{
	static unsigned char cycle = 0;
	static char mouse_bytes[3];
	mouse_bytes[cycle++] = inportb(0x60);
	char x, y;
 
	// Debug, se viene visualizzato questo messagio, il driver worka!
//	_kputs("\nCi sono!\n");

	if (cycle == 3) {
	cycle = 0; 

	if ((mouse_bytes[0] & 0x80) || (mouse_bytes[0] & 0x40))
	  return;
	if (!(mouse_bytes[0] & 0x20))
	   y |= 0xFFFFFF00;
	if (!(mouse_bytes[0] & 0x10))
	  x |= 0xFFFFFF00;
	if (mouse_bytes[0] & 0x4)
	    printf("\n[+] Mouse driver: rilevato tasto centrale premuto!");
	if (mouse_bytes[0] & 0x2)
	    printf("\n[+] Mouse driver: rilevato tasto destro premuto!");
	if (mouse_bytes[0] & 0x1)
	    printf("\n[+] Mouse driver: rilevato tasto sinistro premuto!");
	}
} //End

