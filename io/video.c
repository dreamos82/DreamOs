	/***************************************************************************
 *            video.c
 *
 *  Sat Mar 31 07:47:55 2007
 *  Copyright  2007  sgurtz
 *  Email
 *  Scrolling features and hexadecimal support added by shainer
 *  _kgetline() and _kprintOK developed by ^Inuyasha^
 *  Thanks to Schizzobau
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
#include <video.h>
#include <stddef.h>
#include <string.h>


char *VIDEO_MEM = (char*) 0xb8000,
     *VIDEO_PTR = (char*) 0xb8000,
    VIDEO_CLR = 0x7;

/* Scrolling buffer */
char upbuffer[_SCR_H][_SCR_W*2];
char downbuffer[_SCR_H][_SCR_W*2];
int is_scrolled=0;
int is_shifted_once=0;
unsigned int last_tab=0;

void _kputc(char c)
{
    /* Print a character on the screen*/
  _kshiftAll();
  *VIDEO_PTR++ = c;
  *VIDEO_PTR++ = VIDEO_CLR;
  _ksetcursauto();
}

void _kputs(char *s)
{
  while(*s!=0) {
    if(*s=='\n')
      _knewline();
    else if(*s=='\033') {
      _kcolor(*++s);
    } else if (*s=='\t')
      _ktab();
    else if(*s=='\b')
      _kbackspace();
    else
      _kputc(*s);
    s++;
  }
}

/*
 * Change text colour
 */
void _kcolor(char color)
{
  VIDEO_CLR = color;
}

/*
 * Pression of the backspace key
 */
void _kbackspace()
{
    if (last_tab) {
      unsigned int i;
      for (i = 0; i < 7; i++) {
        VIDEO_PTR -= 2;
        *VIDEO_PTR = 0x20;
      }
      last_tab--;
    }
    else if (shell_mess_line != _kgetline () || shell_mess_col < _kgetcolumn ()) {
      VIDEO_PTR -= 2;
      *VIDEO_PTR = 0x20; // delete the character
    }
}

/*
 * Pression of the TAB key
 */
void _ktab()
{
  VIDEO_PTR = VIDEO_PTR+(7*2);
}

/*
 * Move the cursor at the position x, y on the screen
 */
void _kgoto(int x, int y)
{
  VIDEO_PTR = VIDEO_MEM + ((y * _SCR_W * 2) + (x * 2));
  _ksetcursauto();
}

/*
 * Needed to clear the screen
 */
void _kclear()
{
  int line, row;
  line = 0;
  row = 0;
  VIDEO_PTR = VIDEO_MEM;

  while(row < _SCR_H){
    while(line < _SCR_W){
      *VIDEO_PTR++ = ' ';
      *VIDEO_PTR++ = 0x7;	
      line++;
    }
    line=0;
    row++;		
  }	
  
  VIDEO_PTR = VIDEO_MEM;
}

/*
 * Move to the following line (the effect of \n character)
 */
void _knewline()
{	
    VIDEO_PTR = VIDEO_MEM + ((((VIDEO_PTR - VIDEO_MEM) / (_SCR_W * 2)) + 1) * (_SCR_W * 2));
    _kshiftAll();
    _ksetcursauto();
}

/*
 * Move the cursor to the correct position
 */
void _ksetcursor(unsigned int x, unsigned int y)
{
   asm(
       "movl  %0, %%eax   \n"
       "movl  %1, %%ebx   \n"
       "movl  $0x50, %%ecx   \n"
       "mul   %%ecx         \n"
       "addl  %%ebx, %%eax   \n"
       "movw  $0x03d4, %%dx  \n"
       "pushl %%eax         \n"
       "movb  $0x0f, %%al    \n"
       "out   %%al, %%dx     \n"
       "popl  %%eax         \n"
       "movw  $0x03d5, %%dx  \n"
       "out   %%al, %%dx     \n"
       "shr   $0x08,%%eax      \n"
       "pushl %%eax         \n"
       "movw  $0x03d4, %%dx  \n"
       "movb  $0x0e, %%al    \n"
       "out   %%al, %%dx     \n"
       "pop   %%eax         \n"
       "movw  $0x03d5, %%dx  \n"
       "out   %%al, %%dx     \n"
      :
      : "g" (x), "g" (y)
      );
}

/*
 * When something is written in another position, update the cursor
 */
void _ksetcursauto()
{
   _ksetcursor( ((VIDEO_PTR - VIDEO_MEM) / 2) / _SCR_W,
                ((VIDEO_PTR - VIDEO_MEM) / 2) % _SCR_W );
}

/*
 * Print [OK] at the current row and column 60
 */
void _kprintOK() {
    _kgoto(60, _kgetline());
    _kputs("\033\001 [\033\012OK\033\001]\033\007\n");
}

/*
 * Get the current column number
 */
int _kgetcolumn()
{
  unsigned int column;
  column = VIDEO_PTR - VIDEO_MEM;
  column = (column % (_SCR_W*2));
  return column/2;
}

/*
 * Get the current row number
 */
int _kgetline()
{
  unsigned int linea;
  linea = VIDEO_PTR - VIDEO_MEM;
  linea = (linea / (_SCR_W*2));
  return linea;
}

/*
 * Convert a number into an hexadecimal string
 */
void _kntohex (char *buffer, unsigned int decnum)
{
  int shift=0;
  int tempnum = decnum;

  while (tempnum>=16) {
    tempnum >>= 4;
    shift++;
  }

  for (; shift>=0; shift--) {
    tempnum = decnum;
    tempnum >>= (4*shift);
    tempnum &= 0xF;

    if (tempnum<10)
      *buffer++ = '0'+tempnum;
    else
      *buffer++ = 'A'+tempnum-10;
  }
  *buffer = '\0';
}


/** @author Lisa
  * @version 1.0
  * @param  buffer (char*) La stringa che contiene il numero
  * @param  num Il numero da convertire (intero)
  * @param  base la base numerica in cui convertire (per ora 0, 16,10)
  *
  * Move the number "num" into a string
  */
void _kntos(char *buffer,unsigned int num, int base)
{
  int mod;
  char *p, *pbase;

  p = pbase = buffer;

  if (base==16)
     _kntohex (buffer, num);
  else {
    if (num == 0)
      *p++ = 48;

    while(num > 0)
    {
      mod = num % base;
      *p++ = mod + 48;
      num = num / base;
    }

    *p-- = 0;
    while(p > pbase)
    {
      char tmp;
      tmp = *p;
      *p = *pbase;
      *pbase = tmp;
    
      p--; pbase++;
    }
  }
}

/***************************************
 *                                     *
 * Functions regarding video scrolling *
 *                                     *
 **************************************/


/*
 * First the simplest one: when the cursor reaches the last position of the
 * screen, the whole screen is shifted up by one line
 */
void _kshiftAll (void)
{
  char *i;
  int index;

  if (VIDEO_PTR >= VIDEO_MEM + ((_SCR_H) * _SCR_W * 2)) {

    /*
     * we save the line to be lost in a buffer
     * this will be useful for scrolling
     */
    _krotate_buffer();
    for (index=0; index < _SCR_W*2; index++)
      upbuffer[_SCR_H-1][index] = *(VIDEO_MEM + index);

    for (i=VIDEO_MEM ; i<= (VIDEO_MEM + ((_SCR_H) * _SCR_W * 2) + (_SCR_W * 2));i++)
      *i = i[_SCR_W * 2];
    VIDEO_PTR = VIDEO_MEM + ((((VIDEO_PTR - VIDEO_MEM) / (_SCR_W * 2)) - 1) * (_SCR_W * 2));
    is_shifted_once=1;
  }
}

/*
 * The scrolling buffer is updated to contain the screen up the current one
 * The oldest line is lost to make space for the new one
 */
void _krotate_buffer()
{
  int y=1;
  int x=0;

  for (y=1; y<_SCR_H; y++) {
    for (x=0; x<_SCR_W*2; x++)
      upbuffer[y-1][x] = upbuffer[y][x];
  }
}


/*
 * Called by the pression of the PAGEUP key
 * The screen up the current one is printed and the current one is saved in
 * downbuffer for being restored in future
 */
void _kscrollup ()
{
  int y=0, x=0;
  char *ptr = VIDEO_MEM;

  if (is_scrolled==1 || is_shifted_once==0)
      return;

  for (; y<_SCR_H;y++) {
    for (x=0; x<_SCR_W*2; x++) {
      downbuffer[y][x] = *ptr;
      *ptr++ = upbuffer[y][x];
    }
  }
  is_scrolled=1;
}

/*
 * Called by the pression of the PAGEDOWN key
 * The content of downbuffer (that is, the screen present when you pressed
 * PAGEUP) is printed again
 */
void _kscrolldown ()
{
  int y=0, x=0;
  char *ptr = VIDEO_MEM;

  /*
   * If PAGEUP hasn't been pressed, it's useless to go down, there is nothing
   */
  if (is_scrolled==0)
    return;

  for (y=0; y<_SCR_H;y++) {
    for (x=0; x<_SCR_W*2; x++)
      *ptr++ = downbuffer[y][x];
  }
}

/* EOF */
