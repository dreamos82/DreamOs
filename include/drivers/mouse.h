#ifndef _MOUSE_H_
#define _MOUSE_H_

#define MOUSE_ENABLE_PACKET 0xF4
#define MOUSE_DISABLE_PACKET 0xF5
#define MOUSE_USE_DEFAULT_SETTINGS 0xF6

/**
 * mouse_init()
 * @description inizializza il mouse 
 **/
void mouse_init();
/**
 * mouse_send_data()
 * @description manda dati al mouse
 * @param data:
 * i dati da mandare
 **/
void mouse_waitcmd(unsigned char type);

void mouse_write (unsigned char a_write);
/**
 * mouse_receive_data()
 * @description riceve dati dal mouse
 * @return i dati ricevuti
 **/
unsigned char mouse_read();
/**
 * mouse_waitcmd()
 * @description fa attendere il mouse per un comando
 * @param type:
 * 1 per mandare
 * 0 per ricevere
 **/

void mouse_IRQhandler();
#endif /* _MOUSE_H_ */
