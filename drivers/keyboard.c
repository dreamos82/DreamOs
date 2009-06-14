/***************************************************************************
 *            keyboard.c
 *
 *  Sat Mar 31 07:47:55 2007
 *  Copyright  2007  shainer
 *  Email : shainer@debianclan.org
 *  Keyboard handling
 * ***************************************************************************/

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

#include <keyboard.h>
#include <video.h>
#include <stdio.h>
#include <ctype.h>
#include <io.h>
#include <pic8259.h>

#define LSHIFT_BREAK 0x10E
#define RSHIFT_BREAK 0x11A

/* A macro from Ivan to update buffer indexes */
#define STEP(x) ((x) == BUFSIZE-1 ? 0 : x+1)

/* Keyboard maps */
static int key_it_map[] =
 { //ITALIAN ASCII TABLE - by sgurtz
    -1,  27,  49,  50,  51,  52,  53,  54,  55,  56,  57,  48, //11
    39, 141,   8,   9, 113, 119, 101, 114, 116, 121, 117, 105, //23
   111, 112, 138,  43,  13,  -1,  97, 115, 100, 102, 103, 104, //35
   106, 107, 108, 149, 133,  92,  -1, 151, 122, 120,  99, 118, //47
    98, 110, 109,  44,  46,  45,  -1,  42,  -1,  32,  -1,  -1, //59
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  55, //71
    56,  57,  45,  52,  53,  54,  43,  49,  50,  51,  48,  46, //83
    -1,  -1,  60,  -1,  -1                                     //88
 };

static int shifted_it_map[] =
 { //ITALIAN ALTERNATIVE ASCII TABLE - by sgurtz
    -1,  -1,  33,  34, 156,  36,  37,  38,  47,  40,  41,  61, //11
    63,  94,  -1,  -1,  81,  87,  69,  82,  84,  89,  85,  73, //23
    79,  80, 130,  42,  -1,  -1,  65,  83,  68,  70,  71,  72, //35
    74,  75,  76, 128, 167, 124,  -1,  -1,  90,  88,  67,  86, //47
    66,  78,  77,  59,  58,  95,  -1,  -1,  -1,  32,  -1,  -1, //59
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, //71
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, //83
    -1,  -1,  62,  -1,  -1                                     //88
 };

/* UNUSED static unsigned char pad_map[] = { '7', '8', '9', '0', '4', '5', '6', '0', '1', '2', '3', '0', '0' };*/

/* The buffer where keys are stored */
static int circlebuf[BUFSIZE];
static int buf_r=0, buf_w=0;

static int sc;
static int *curmap = key_it_map;
static unsigned short ledstate = 0;

/* Variables to track states */
static int is_shifted;
static int is_tab_pressed;
static int is_num_pressed;
static int is_scroll_pressed;

extern unsigned int last_tab;

/*
 * The keyboard handler
 */
void keyboard_isr (void)
{
    sc = inportb (0x60); // take scancode from the port    
    /* error handling */
    if (sc == 0x00 || sc == 0xFF) {
	_kputs ("Keyboard error\n");
	goto end;
    }

    /* The right map is selected */
    if (is_shifted == 0 && is_tab_pressed == 0)
	curmap = key_it_map;
    else if (is_shifted == 0 && is_tab_pressed)
	curmap = shifted_it_map;
    else if (is_shifted == 1 && is_tab_pressed == 0)
	curmap = shifted_it_map;
    else if (is_shifted == 1 && is_tab_pressed)
	curmap = key_it_map;

    /* In case of useless break codes, switch controls...*/
    if (sc > CODE_BREAK && sc != (KEY_LSHIFT|CODE_BREAK) && sc != (KEY_RSHIFT|CODE_BREAK)){
        if (sc==KEY_ENTER+128)
            outportb(MASTER_PORT, EOI);
	    goto end;
        }

    switch (sc) {
    case KEY_LSHIFT:
	is_shifted = 1;
	break;

    case KEY_PGDOWN:
        _kscrolldown ();
	break;

    case KEY_PGUP:
	_kscrollup ();
        break;


    case KEY_RSHIFT:
	is_shifted = 1;
	break;

    case KEY_LSHIFT|CODE_BREAK:
    case KEY_RSHIFT|CODE_BREAK:
	is_shifted = 0;
	break;

    case CAPS_LED:
	if (is_tab_pressed == 0) {
            _ksetleds (1,-1,-1);
	    is_tab_pressed = 1;
	} else {
	    is_tab_pressed = 0;
	    _ksetleds (0,-1,-1);
	}
	break;

    case NUM_LED:
	if (is_num_pressed == 0) {
	    _ksetleds (-1,1,-1);
            is_num_pressed = 1;
	} else {
	    _ksetleds (-1,0,-1);
	    is_num_pressed = 0;
	}
	break;

    case SCROLL_LED:
	if (is_scroll_pressed == 0) {
	    _ksetleds (-1,-1,1);
            is_scroll_pressed = 1;
	} else {
	    _ksetleds (-1,-1,0);
	    is_scroll_pressed = 0;
	}
	break;

    case KEY_ESCAPE:
        break;

    case KEY_BACKSPACE:
	if (STEP(buf_w) == buf_r)
	  buf_r = STEP(buf_r);
	circlebuf[buf_w] = '\b';
	buf_w = STEP(buf_w);
	_kbackspace();
        _ksetcursauto();
	break;

    case KEY_ENTER:
	if (STEP(buf_w) == buf_r)
	    buf_r = STEP(buf_r);
	circlebuf[buf_w] = '\n';
	buf_w = STEP(buf_w);
	_knewline();
	_ksetcursauto();
	last_tab = 0;
    outportb(MASTER_PORT, EOI);
	break;

    case KEY_TAB:
	if (STEP(buf_w) == buf_r)
	    buf_r = STEP(buf_r);
	circlebuf[buf_w] = '\t';
	buf_w = STEP(buf_w);
	_ktab();
	_ksetcursauto();
	last_tab++;
	break;

    /*case KEY_PGUP:
        _kputs ("Tada");
         _kscrollup();
	break;*/

    /*case 813:
        _kputs ("tutu");
	_kscrolldown();
	break;*/

    default:
	if (isdigit(key_it_map[sc]) && is_tab_pressed == 1)
	    curmap = key_it_map;

        //printf ("%d", sc);
        putchar (curmap[sc]);

        /* Update buffer */
	if (STEP(buf_w) == buf_r)
            buf_r = STEP(buf_r);
	circlebuf[buf_w] = curmap[sc];
        buf_w = STEP(buf_w);
    }

end:
    /* Send acknowledge */
    //printf ("Prego");
    outportb (MASTER_PORT, EOI);
    return;
}

/*
 * Leds handler
 * 1=Set the led on
 * 0=Set the led off
 * -1=Leave it unchanged
 */
void _ksetleds(int capslock, int numlock, int scrlock)
{
    if (numlock == 1) {
	numlock = 2;
	ledstate |= numlock;
    } if (capslock == 1) {
	capslock = 4;
	ledstate |= capslock;
    } if (scrlock == 1)
	ledstate |= scrlock;

    if (numlock == 0) {
        numlock = 2;
	ledstate ^= numlock;
    }
    if (capslock == 0) {
        capslock = 4;
	ledstate ^= capslock;
    }
    if (scrlock == 0) {
        scrlock = 1;
	ledstate ^= scrlock;
    }

    outportb (0x60, 0xED);
    outportb (0x60, ledstate);
}

/* Need explanations? */
void keyboard_enable (void)
{
    outportb (0x60, 0xF4);
}

void keyboard_disable (void)
{
    outportb (0x60, 0xF5);
}

/*
 * Get a char from the buffer
 * looping until there is something new to read
 */
int _kgetch (void) {
    int c = 0;

loop:
    if (buf_r != buf_w) {
	c = circlebuf[buf_r];
	buf_r = STEP(buf_r);
    }
    else
        goto loop;
    return c;
}
